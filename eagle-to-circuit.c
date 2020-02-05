#include "lib/windoze.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/cb.h"
#include "lib/cbmap.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/xml.h"
#include "lib/array.h"
#include "lib/open.h"

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#if !defined(_WIN32) && !(defined(__MSYS__) && __MSYS__ == 1)
#include <libgen.h>
#endif

#include "map.h"

#ifdef _MSC_VER
#define alloca _alloca
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

typedef int(cmp_fn_t)(const void*, const void*);

struct pad {
    stralloc name;
    double x, y;
};
struct package {
    stralloc name;
    array pads; /**< list of struct pad */
};
//};
struct pin {
    stralloc name;
    double x, y, r;
    int visible;
};
struct symbol {
    stralloc name;
    array pins; /**< list of struct pin */
};
struct pinmapping {
    struct package* pkg;
    array map;
};
struct gate {
    stralloc name;
    stralloc symbol;
};
struct deviceset {
    stralloc name;
    array gates;   /**< list of struct gate */
    MAP_T devices; /**< map of struct pinmapping */
};
struct part {
    stralloc name;
    stralloc value;
    struct package* pkg;
    struct device* dev;
    struct deviceset* dset;
    struct net* pins;
    double x, y;
};
struct ref {
    struct part* part;
    int gate;
    union {
        int pad;
        int pin;
    };
};
struct net {
    stralloc name;
    array contacts; /**<  list of struct ref */
};
struct part_ref {
    strlist* list;
    struct part* part;
};

const char* document = "<doc/>";
const char* xq = "net";
void node_print(xmlnode* node);
int node_depth(xmlnode* node);
int str_ischarset(const char* s, const char* set);
int str_isfloat(const char* s);
int str_isspace(const char* s);
void print_attrs(HMAP_DB* a_node);
void print_element_attrs(xmlnode* a_node);
int output_net(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data);
MAP_T devicesets;
MAP_T packages;
MAP_T parts;
MAP_T nets;
MAP_T symbols;
strlist connections;
buffer input, output;
static struct {
    struct {
        int x, y;
    } min, max;
} bounds;

/**
 * Reads a real-number value from the element/attribute given
 */
double
get_double(xmlnode* node, const char* key) {
    double ret = 0.0;
    const char* dstr = NULL;

    if(xml_has_attribute(node, key)) {
        dstr = xml_get_attribute(node, key);
        if(scan_double(dstr, &ret) <= 0)
            ret = DBL_MAX;
    }
    return ret;
}

/**
 * Reads an integer number value from the element/attribute given
 */
int
get_int(xmlnode* node, const char* key) {
    long ret = INT_MIN;
    const char* istr = xml_get_attribute(node, key);

    if(istr) {
        while(*istr && str_chr("-0123456789", *istr) >= 11) ++istr;
        if(!scan_long(istr, &ret))
            ret = INT_MAX;
    }
    return ret;
}

/**
 * Gets a child element by name.
 */
xmlnode*
get_child(xmlnode* node, const char* name) {

    for(node = node->children; node; node = node->next) {
        if(!str_diff(node->name, name))
            return node;
    }
    return NULL;
}

/**
 * Gets a MAP_T element.
 */
void*

get(MAP_T m, char* name, size_t datasz) {
    return MAP_GET(m, name, str_len(name) + 1);
}

/**
 * Gets or creates a MAP_T element.
 */
void*
get_or_create(MAP_T m, char* name, size_t datasz) {
    void* ptr = get(m, name, datasz);

    if(!ptr) {
#ifdef HAVE_ALLOCA
        char* data = alloca(datasz);
#elif defined(HAVE_DYNSTACK)
        char data[datasz];
#else
        char* data = malloc(datasz);
#endif
        byte_zero(data, datasz);
        if(MAP_INSERT(m, name, str_len(name) + 1, data, datasz))
            ptr = get(m, name, datasz);
#if !defined(HAVE_ALLOCA) && !defined(HAVE_DYNSTACK)
        free(data);
#endif
    }
    return ptr;
}

void*
get_entry(MAP_T map, const char* key) {
    return MAP_GET(map, (void*)key, str_len(key) + 1);
}

/**
 * Outputs name/value pair
 */
void
print_name_value(buffer* b, const char* name, const char* value) {
    buffer_putm_3(b, name, ": ", value ? value : "(null)");
}

int
package_pin(struct package* pkg, const char* name) {
    size_t nlen = str_len(name);
    int64 i, len = array_length(&pkg->pads, sizeof(struct pad));

    for(i = 0; i < len; ++i) {
        struct pad* p = array_get(&pkg->pads, sizeof(struct pad), i);
        if(p->name.len == nlen && !str_diffn(p->name.s, name, str_len(name)))
            return i;
    }
    return -1;
}

void
update_bounds(int x, int y) {
    if(bounds.min.x > x)
        bounds.min.x = x;
    if(bounds.max.x < x)
        bounds.max.x = x;
    if(bounds.min.y > y)
        bounds.min.y = y;
    if(bounds.max.y < y)
        bounds.max.y = y;
}

/**
 * Build structures from <part> or <element> element
 */
void
build_part(xmlnode* part) {
    size_t pins;
    struct part p;
    char *dsname, *val, *name = xml_get_attribute(part, "name");
    char* pkgname = xml_get_attribute(part, "package");
    if(!name || str_len(name) == 0)
        return;
    byte_zero(&p, sizeof(struct part));
    stralloc_copys(&p.name, name);
    val = xml_get_attribute(part, "value");
    if(val)
        stralloc_copys(&p.value, val);

    p.x = get_double(part, "x");
    p.y = get_double(part, "y");

    update_bounds(round(p.x / 2.54), round(p.y / 2.54));

    if(pkgname && str_len(pkgname)) {
        p.pkg = get_entry(packages, pkgname);
    }
    assert(p.pkg);

    pins = array_length(&p.pkg->pads, sizeof(struct net*));
    p.pins = calloc(sizeof(struct net*), pins);
    dsname = xml_get_attribute(part, "deviceset");
    if(dsname)
        p.dset = get_entry(devicesets, dsname);
    MAP_INSERT(parts, (void*)name, str_len(name) + 1, &p, sizeof(struct part));
}

/**
 * Build structures from <symbol> element
 */
void
build_sym(xmlnode* part) {
    size_t i = 0;
    xmlnode* pin;
    struct symbol* sym;
    char* name = xml_get_attribute(part, "name");
    if(!name || str_len(name) == 0)
        return;
    sym = get_or_create(symbols, name, sizeof(struct symbol));
    stralloc_copys(&sym->name, name);

    for(pin = part->children; pin; pin = pin->next) {
        struct pin* p;
        char* pin_name;
        if(pin->type != XML_ELEMENT)
            continue;
        if(str_diff(pin->name, "pin"))
            continue;
        pin_name = xml_get_attribute(pin, "name");
        if(pin_name == NULL)
            continue;
        p = array_allocate(&sym->pins, sizeof(struct ref), i++);
        byte_zero(p, sizeof(struct ref));
        stralloc_copys(&p->name, pin_name);
        p->x = get_double(pin, "x");
        p->y = get_double(pin, "y");
        p->r = (double)get_int(pin, "rot") * M_PI / 180;
        p->visible = str_diff(xml_get_attribute(pin, "visible"), "off");
    }
}

static int
compare_ref(const struct ref* a, const struct ref* b) {
    int ret = stralloc_diff(&a->part->name, &b->part->name);

    if(ret)
        return ret;

    return a->pin - b->pin;
}

/**
 * @param node   Parent is the 'net' or 'signal' element
 */
void
build_reflist(xmlnode* node, struct net* n, int* index) {

    for(; node; node = node->next) {
        struct ref* r;
        int is_pin;
        char *part_name, *nn = node->name;
        if(node->type != XML_ELEMENT)
            continue;

        if(str_equal(nn, "segment")) {
            build_reflist(node->children, n, index);
            continue;
        }
        is_pin = str_equal(nn, "pinref");
        if(str_diff(nn, is_pin ? "pinref" : "contactref"))
            continue;
        part_name = xml_get_attribute(node, is_pin ? "part" : "element");
        r = array_allocate(&n->contacts, sizeof(struct ref), (*index)++);
        r->part = get(parts, part_name, sizeof(struct part));
        print_name_value(buffer_2, nn, part_name);
        buffer_putnlflush(buffer_2);

        if(r->part->pkg) {
            r->pin = package_pin(r->part->pkg, xml_get_attribute(node, "pad"));
        }
        print_name_value(buffer_2, nn, part_name);
        buffer_putc(buffer_2, '\t');
        print_element_attrs(node);
        buffer_putnlflush(buffer_2);
    }
    qsort(array_start(&n->contacts),
          array_length(&n->contacts, sizeof(struct ref)),
          sizeof(struct ref),
          (cmp_fn_t*)&compare_ref);
}

/**
 * Build reference list from
 *
 *      <contactref element="IC1" pad="4"/>
 *
 *      <pinref part="IC1" gate="B" pin="O"/>
 */
void
build_nets(xmlnode* node) {
    int i = 0;
    struct net* ptr;
    xmlnode* sn;
    char *sign, *name = node->name;

    assert(str_equal(name, "net") || str_equal(name, "signal"));

    if(!(sign = xml_get_attribute(node, "name")))
        return;
    print_name_value(buffer_2, name, sign);
    buffer_putnlflush(buffer_2);
    ptr = get_or_create(nets, sign, sizeof(struct net));
    stralloc_copys(&ptr->name, sign);
    build_reflist(node->children, ptr, &i);
}

/**
 * Build structures from <package> element
 */
void
build_package(xmlnode* set) {
    struct package pkg;
    xmlnode* node;
    char* name = xml_get_attribute(set, "name");
    byte_zero(&pkg, sizeof(struct package));
    stralloc_copys(&pkg.name, name);

    for(node = set->children; node; node = node->next) {
        char* pn;
        struct pad p;
        if(node->type != XML_ELEMENT || str_diff(node->name, "pad"))
            continue;
        pn = xml_get_attribute(node, "name");
        if(!pn || str_len(pn) == 0)
            continue;
        byte_zero(&p, sizeof(struct pad));
        p.x = get_double(node, "x");
        p.y = get_double(node, "y");
        stralloc_copys(&p.name, pn);
        array_catb(&pkg.pads, (const void*)&p, sizeof(struct pad));
    }
    MAP_INSERT(packages, name, str_len(name) + 1, &pkg, sizeof(struct package));
}

/**
 * Build structures from <deviceset> element
 */
void
build_deviceset(xmlnode* set) {
    xmlnode *node, *gates, *devices;
    struct deviceset d;
    char* name = xml_get_attribute(set, "name");

#ifdef DEBUG_REFLIST
    print_name_value(buffer_2, "deviceset", name);
    buffer_putnlflush(buffer_2);
#endif
    byte_zero(&d, sizeof(struct deviceset));
    stralloc_copys(&d.name, name);
    MAP_NEW(d.devices);
    gates = get_child(set, "gates");
    devices = get_child(set, "devices");

    for(node = gates->children; node; node = node->next) {
        struct gate g;
        if(node->type != XML_ELEMENT)
            continue;
        byte_zero(&g, sizeof(struct gate));
        stralloc_copys(&g.name, xml_get_attribute(node, "name"));
        stralloc_copys(&g.symbol, xml_get_attribute(node, "symbol"));
        array_catb(&d.gates, (const void*)&g, sizeof(struct gate));
    }

    for(node = devices->children; node; node = node->next) {
        struct package* pkg;
        size_t len;
        char *name, *package;
        struct pinmapping pm;
        if(node->type != XML_ELEMENT)
            continue;
        name = xml_get_attribute(node, "name");
        if(!name || str_len(name) == 0)
            continue;
        package = xml_get_attribute(node, "package");
        byte_zero(&pm, sizeof(struct pinmapping));

        // MAP_GET(packages, package, str_len(package), (void**)&pkg, &len);
        pm.pkg = MAP_GET(packages, package, str_len(package));

        MAP_INSERT(d.devices, name, str_len(name) + 1, &pm, sizeof(struct pinmapping));
    }
    MAP_INSERT(devicesets, name, str_len(name) + 1, &d, sizeof(struct deviceset));
}

/**
 * Run an XPath query and return a XPath object
 */
xmlnodeset
getnodeset(void* n, const char* xpath) {
    return xml_find_all_1(n, xml_match_name, xpath);
}

/**
 * Retrieve all <part> (schematic) or <element> (board) objects
 */
strlist
getparts(xmlnode* doc) {
    xmlnodeset_iter_t it, e;
    xmlnodeset ns;
    strlist ret;
    strlist_init(&ret, '\0');
    ns = getnodeset(doc, "part|element");

    for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
        xmlnode* node = *it;
        strlist_push(&ret, xml_get_attribute(node, "name"));
    }
    return ret;
}

/**
 * Iterate through a node-set, calling a functor for every item
 */
void
for_set(xmlnodeset* ns, void (*fn)(xmlnode*)) {
    xmlnodeset_iter_t it, e;
    if(!ns)
        return;
    for(it = xmlnodeset_begin(ns), e = xmlnodeset_end(ns); it != e; ++it) fn(*it);
}

int
compare_pads(const struct pad* a, const struct pad* b) {
    long la = LONG_MAX, lb = LONG_MAX;
    if(scan_long(a->name.s, &la) && scan_long(b->name.s, &lb))
        return la - lb;
    return stralloc_diff(&a->name, &b->name);
}

void
clean_pkgname(stralloc* pkgname, const struct package* pkg) {
    size_t idx;
    stralloc_init(pkgname);
    stralloc_copy(pkgname, &pkg->name);
    stralloc_lower(pkgname);

    if(pkgname->len > 4 && pkgname->s[0] == '0' && isdigit(pkgname->s[1]) && isdigit(pkgname->s[2]) &&
            isdigit(pkgname->s[3]) && pkgname->s[4] == '/') {
        // stralloc_remove(pkgname, 0, 1);
        pkgname->s[0] = 'r';
        pkgname->s[1] = 'e';
        pkgname->s[2] = 's';
        pkgname->s[3] = '_';
        stralloc_replaces(pkgname, "-v", "v");
    }

    if(pkgname->s[0] == 'c' || pkgname->s[0] == 'e')
        stralloc_replaces(pkgname, "-", "_");

    stralloc_replaces(pkgname, "_0", "_");
    stralloc_replaces(pkgname, ".", "");
    stralloc_replaces(pkgname, "/", "");
    stralloc_replaces(pkgname, "c0", "c");
    stralloc_replaces(pkgname, "-0", "_");
    stralloc_replaces(pkgname, "x0", "x");
    stralloc_replaces(pkgname, "-", "_n");
    stralloc_replaces(pkgname, "+", "_p");

    for(idx = 0; idx < pkgname->len; idx++) {
        if(!isalnum(pkgname->s[idx]))
            pkgname->s[idx] = '_';
    }
}

int
dump_package(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
    int64 i;
    double x, y;
    const struct package* pkg = value;
    struct pad* first;
    stralloc pkgname;
    clean_pkgname(&pkgname, pkg);

    // buffer_puts(&output, "dump_package: ");
    stralloc_nul(&pkgname);

    buffer_putspad(&output, pkgname.s, 18);
    stralloc_free(&pkgname);
    buffer_putspace(&output);

    first = array_start(&pkg->pads);

    qsort(first, array_length(&pkg->pads, sizeof(struct pad)), sizeof(struct pad), (cmp_fn_t*)&compare_pads);

    x = first->x;
    y = first->y;

    for(i = 0; i < array_length(&pkg->pads, sizeof(struct pad)); ++i) {
        const struct pad* p = array_get(&pkg->pads, sizeof(struct pad), i);
        int ix, iy;

        if(i > 0)
            buffer_putspace(&output);

        ix = round((p->x - x) / 2.54);
        iy = round((p->y - y) / 2.54);

        buffer_putlong(&output, -iy);
        buffer_putc(&output, ',');
        buffer_putlong(&output, -ix);
        //   buffer_putspace(&output);
        // buffer_puts(&output, "\n\t ");
    }
    // buffer_puts(buffer_2, " ]");
    buffer_putnlflush(&output);
    return 1;
}

static int
cmp_ref(const char** a, const char** b) {
    size_t alen = byte_chr(*a, str_len(*a), '\t'), blen = byte_chr(*b, str_len(*b), '\t');
    if(alen != blen)
        return alen - blen;

    return str_diff(*a, *b);
}
int
output_part(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
    struct part* ptr = (struct part*)value;
    struct pad* pad1 = array_start(&ptr->pkg->pads);
    double x, y;

    stralloc name;
    stralloc_init(&name);
    stralloc_copy(&name, &ptr->name);
    stralloc_nul(&name);
    buffer_putspad(&output, name.s, 19);
    stralloc_free(&name);

    clean_pkgname(&name, ptr->pkg);
    stralloc_nul(&name);
    buffer_putspad(&output, name.s, 18);

    x = round((ptr->x + pad1->x) / 2.54);
    y = round((ptr->y + pad1->y) / 2.54);

    buffer_putlong(&output, y + 10);
    buffer_putc(&output, ',');
    buffer_putlong(&output, x + 10);

    buffer_putnlflush(&output);
    return 1;
}

int
dump_part(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
    struct part* ptr = (struct part*)value;
    const char *s, *comment;
    size_t n;
    strlist refs;
    struct part_ref u;
    u.list = &refs;
    u.part = ptr;
    assert(ptr->name.s);
    buffer_puts(&output, "\n# Part: ");
    buffer_putsa(&output, &ptr->name);
    buffer_puts(&output, "\n");

    if(ptr->pkg) {
        buffer_puts(buffer_2, " package: ");
        buffer_putsa(buffer_2, &ptr->pkg->name);
    }

    strlist_init(&refs, '\n');

    MAP_VISIT_ALL(nets, output_net, &u);

    strlist_sort(&refs, (cmp_fn_t*)&cmp_ref);

    strlist_foreach(&refs, s, n) {
        size_t clen;
        size_t i = 0;
        if((clen = byte_chr(s, n, '\t')) + 1 < n) {
            ((char*)s)[clen] = '\0';
            buffer_putspad(&output, s, 7);
            s += clen + 1;
            n -= clen + 1;
            i++;
        }
        buffer_put(&output, s, byte_chrs(s, n, " \t\r#\n", 4));
        buffer_putc(&output, '\n');
    }

    buffer_putnlflush(&output);
    strlist_free(&refs);

    if(ptr->dset) {
        buffer_puts(buffer_2, " deviceset: ");
        buffer_putsa(buffer_2, &ptr->dset->name);
    }
    buffer_putnlflush(buffer_2);
    return 1;
}

struct ref*
net_connects(const struct net* net, struct part* part, int pin) {
    int64 i, len = array_length(&net->contacts, sizeof(struct ref));

    for(i = 0; i < len; ++i) {
        struct ref* r = array_get(&net->contacts, sizeof(struct ref), i);
        if(r->part == part && (r->pin == pin || pin == -1))
            return r;
    }
    return NULL;
}

void
pin_sa(stralloc* sa, const stralloc* name, int pin) {
    stralloc_cat(sa, name);
    stralloc_catc(sa, '.');
    stralloc_catlong(sa, pin);
}

void
conn_sa(stralloc* sa, const stralloc* name1, int pin1, const stralloc* name2, int pin2) {
    pin_sa(sa, name1, pin1);
    stralloc_catc(sa, '\t');
    pin_sa(sa, name2, pin2);
}

void
dump_net(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
    struct net* n = (struct net*)value;
    struct ref* r;

    buffer_puts(&output, "\n# Net ");
    buffer_putsa(&output, &n->name);
    buffer_puts(&output, ":");

    array_foreach(&n->contacts, sizeof(struct ref), r) {
        buffer_putspace(&output);
        buffer_putsa(&output, &r->part->name);
        buffer_putc(&output, '.');
        buffer_putlong(&output, r->pin + 1);
    }

    buffer_putnlflush(&output);
}

int
output_net(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
    struct net* n = (struct net*)value;
    strlist* refs = ((struct part_ref*)user_data)->list;
    struct part* p = ((struct part_ref*)user_data)->part;
    struct ref* rc;

    int64 i, len;
    stralloc conn, rconn;
    stralloc_init(&conn);
    stralloc_init(&rconn);

    if(p) {
        if(!(rc = net_connects(n, p, -1)))
            return 1;
    }

    /* {
      buffer_puts(&output, "## Net ");
      buffer_putsa(&output, &n->name);
      buffer_puts(&output, "\n");
    }*/
    len = array_length(&n->contacts, sizeof(struct ref));

    for(i = 0; i < len; ++i) {
        struct ref* r = array_get(&n->contacts, sizeof(struct ref), i);
        if(r == rc)
            continue;
        assert(r->part);

        stralloc_zero(&conn);
        stralloc_zero(&rconn);

        conn_sa(&conn, &p->name, rc->pin + 1, &r->part->name, r->pin + 1);
        conn_sa(&rconn, &r->part->name, r->pin + 1, &p->name, rc->pin + 1);

        if(strlist_contains_sa(&connections, &conn) || strlist_contains_sa(&connections, &rconn))
            continue;

        strlist_push_sa(&connections, &conn);
        stralloc_cats(&conn, "\t# ");
        stralloc_cat(&conn, &n->name);
        strlist_push_sa(refs, &conn);
    }

    stralloc_free(&conn);
    stralloc_free(&rconn);
    return 1;
}
/**
 *  hashmap_dump: Gets depth of node in hierarchy
 */
int
node_depth(xmlnode* node) {
    size_t i = 0;
    while((node = node->next)) ++i;
    return i;
}

int
str_ischarset(const char* s, const char* set) {

    while(*s) {
        if(set[str_chr(set, *s)] == '\0')
            return 0;
        ++s;
    }
    return 1;
}

int
str_isdoublenum(const char* s) {
    char* end;
    strtod(s, &end);
    return end > s;
}

int
str_isspace(const char* s) {
    return str_ischarset(s, "\t\r\n\v ");
}

/**
 *  print element path/name
 */
void
print_element_name(xmlnode* a_node) {
    char* name = a_node->name;

    if(a_node->parent) {
        xmlnode* p = a_node->parent;
        const char* pn = p->name;

        if(pn && !str_diffn(pn, name, str_len(name))) {
            p = p->parent;
        }
        print_element_name(p);
    }
    if(!(name = a_node->name))
        return;

    if(str_diff(name, "eagle") && str_diff(name, "drawing")) {
        buffer_putm_2(buffer_2, a_node->parent ? "/" : "", name);
        if(!(name = xml_get_attribute(a_node, "name")))
            return;
        if(str_len(name))
            buffer_putm_3(buffer_2, "[@name='", name, "']");
    }
}

/**
 *  print_element_attrs: Prints all element attributes to stdout
 */
void
print_attrs(HMAP_DB* a) {
    TUPLE* t;

    for(t = a->list_tuple; t; t = t->next) {
        char* v = t->vals.val_chars;
        buffer_putm_5(buffer_2, " ", t->key, str_isdoublenum(v) ? "=" : "=\"", v, str_isdoublenum(v) ? "" : "\"");
        if(t->next == a->list_tuple)
            break;
    }
}

void
print_element_attrs(xmlnode* a_node) {
    if(a_node->attributes)
        print_attrs(a_node->attributes);
}

void
print_element_content(xmlnode* node) {
    const char* s;

    if((s = xml_content(node))) {
        if(str_isspace(s))
            s = "";
        if(str_len(s))
            buffer_putm_3(buffer_2, " \"", s, "\"");
    }
}

/**
 *  print_element_children: Prints all element attributes to stdout
 */
void
print_element_children(xmlnode* a_node) {
    xmlnode* node;
    if(!a_node->children)
        return;

    for(node = a_node->children; node; node = node->next) {
        if(node->type != XML_ELEMENT)
            continue;
        print_element_name(node);
        print_element_attrs(node);
        print_element_content(node);
        buffer_putnlflush(buffer_2);
        print_element_children(node);
    }
}

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
void
print_element_names(xmlnode* node) {
    if(node->type == XML_DOCUMENT)
        node = node->children;

    for(; node; node = node->next) {
        if(node->type != XML_ELEMENT)
            continue;
        print_element_name(node);

        if(node_depth(node) >= 1) {
            print_element_attrs(node);
        }
        print_element_content(node);
        buffer_putnlflush(buffer_2);
        print_element_names(node->children);
    }
}

void
match_query(xmlnode* doc, const char* q) {
    xmlnodeset ns;
    xmlnodeset_iter_t it, e;
    print_name_value(buffer_2, "XPath query", q);
    buffer_putnlflush(buffer_2);
    ns = getnodeset(doc, q);

    for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
        xmlnode* node = *it;
        print_element_name(node);
        print_element_attrs(node);
        buffer_putnlflush(buffer_2);
        print_element_children(node);
        buffer_putnlflush(buffer_2);

        if(0) { //! str_diff(q, xq)) {
            TUPLE* a;
            const char* elem_name = &q[2];
            stralloc query;
            stralloc_init(&query);
            elem_name = "*";

            for(a = hmap_begin(node->attributes); a; a = hmap_next(node->attributes, a)) {
                strlist part_names;
                const char* attr_name = a->key;
                const char* v = a->vals.val_chars;
                if(!v || str_len(v) == 0)
                    continue;

                if(!str_diff(attr_name, "name")) {
                    elem_name = "*";
                    attr_name = node->name;
                } else {
                    elem_name = attr_name;
                    attr_name = "name";
                }
                stralloc_zero(&query);
                stralloc_catm_internal(&query, "", elem_name, "[@", attr_name, "='", v, "']", NULL);
                stralloc_0(&query);
                match_query(doc, query.s);
                part_names = getparts(doc);
                strlist_dump(buffer_2, &part_names);
            }
        }
    }
}

/**
 * Executes XPath query and for every resulting element calls a function
 */
int
match_foreach(xmlnode* doc, const char* q, void (*fn)(xmlnode*)) {
    xmlnodeset ns = getnodeset(doc, q);

    if(xmlnodeset_size(&ns)) {
        for_set(&ns, fn);
        return 1;
    }
    return 0;
}

int
main(int argc, char* argv[]) {
    xmlnode* doc;
    int argi = 1, output_fd = 1;
    const char* input_file = "/home/roman/Sources/an-tronics/eagle/40106-4069-Synth.brd";
    const char* output_file = NULL;

    MAP_NEW(devicesets);
    MAP_NEW(packages);
    MAP_NEW(parts);
    MAP_NEW(nets);
    MAP_NEW(symbols);

    strlist_init(&connections, '\0');

    if(argv[argi])
        input_file = argv[argi++];
    if(argv[argi])
        output_file = argv[argi++];
    if(argv[argi])
        xq = argv[argi++];

    if(output_file)
        output_fd = open_trunc(output_file);

    buffer_write_fd(&output, output_fd);

    buffer_mmapprivate(&input, argv[1]);
    buffer_skip_until(&input, "\r\n", 2);
    doc = xml_read_tree(&input);
    match_query(doc, xq);
    match_foreach(doc, "package", build_package);
    buffer_puts(buffer_2, "items in packages: ");
    buffer_putulong(buffer_2, MAP_SIZE(packages));
    buffer_putnlflush(buffer_2);
    match_foreach(doc, "deviceset", build_deviceset);
    match_foreach(doc, "part|element", build_part);
    buffer_puts(buffer_2, "items in parts: ");
    buffer_putulong(buffer_2, MAP_SIZE(parts));
    buffer_putnlflush(buffer_2);
    match_foreach(doc, "net|signal", build_nets);
    match_foreach(doc, "symbol", build_sym);

    buffer_puts(&output, "# Stripboard\n# board <width>,<height>\n\nboard ");

    buffer_putlong(&output, ((bounds.max.x + 9) / 10 + 2) * 10);
    buffer_putc(&output, ',');
    buffer_putlong(&output, ((bounds.max.y + 9) / 10 + 2) * 10);
    buffer_puts(&output, "\n");

    buffer_puts(&output, "\n# Packages\n# <package name>   <pin coordinates relative to pin 0>\n\n");
    MAP_VISIT_ALL(packages, dump_package, "package");

    buffer_puts(&output,
                "\n# Components\n# <component name> <package name>    <absolute position of component pin 0>\n\n");
    MAP_VISIT_ALL(parts, output_part, "part");

    buffer_puts(&output, "\n# Connections\n# <from component name>.<pin index> <to component name>.<pin index>\n\n");

    MAP_VISIT_ALL(parts, dump_part, "part");
    MAP_VISIT_ALL(nets, dump_net, 0);

    hmap_dump(nets, buffer_1);
    hmap_print_list(nets);

    strlist_dump(buffer_2, &connections);

    /*
     * Cleanup function for the XML library.
     */
    xml_free(doc);
    buffer_flush(&output);
    /*
     * this is to debug memory for regression tests
     */
    return (0);
}
