#include "lib/unix.h"
#include "lib/byte.h"
#include "lib/array.h"
#include "lib/open.h"
#include "lib/buffer.h"
#include "lib/cb.h"
#include "lib/cbmap.h"
#include "lib/errmsg.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/xml.h"

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

#ifdef _MSC_VER
#define alloca _alloca
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#include "lib/round.c"
#define iround(f) ((double)((long)(f + 0.5)))

#define mm2in(mm) ((mm) / 25.4)
#define in2mm(in) ((mm)*25.4)

/**
 * section: Parsing
 * synopsis: Parse an XML document in
 * memory to a tree and free it purpose:
 * Demonstrate the use of
 * xmlReadMemory() to read an XML file
 *          into a tree and and
 * xml_free() to free the resulting tree
 * usage: parse3
 * test: parse3
 * author: Daniel Veillard
 * copy: see Copyright for the status of
 * this software.
 */
#include "lib/str.h"
#include <stdio.h>

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
  array gates;     /**< list of struct gate */
  cbmap_t devices; /**< map of struct
                      pinmapping */
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
typedef struct pos {
  double x, y;
} xy;
struct wire {
  double x1, y1, x2, y2;
};
typedef struct wire wire;
// typedef struct wire rect;

typedef struct pos point;

union vec2 {
  struct {
    double x1, y1;
    double x2, y2;
  };
  point p[2];
  struct {
    point a, b;
  };
};
typedef union vec2 rect;
struct net {
  stralloc name;
  array contacts; /**<  list of struct
                     ref */
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

void
dump_strarray(const char* name, strarray* stra) {
  char** p;
  size_t n = 0;
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ":\n  ");
  strarray_foreach(stra, p) {
    if(n)
      buffer_puts(buffer_2, ", ");
    buffer_puts(buffer_2, "#");
    buffer_putulong(buffer_2, n++);
    buffer_puts(buffer_2, ": ");
    buffer_puts(buffer_2, *p ? *p : "(null)");
  }
  buffer_putnlflush(buffer_2);
}
void print_xy(buffer* b, const char* name, double x, double y);

int dump_net(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data);
cbmap_t devicesets, packages, parts, nets, symbols;
static strarray layers;
static int measures_layer = -1, bottom_layer = -1;
static array wires;
static rect bounds;

static int do_list_layers, do_draw_measures, do_align_coords, print_comments;
static const char* current_layer = "Bottom";
static int current_layer_id = -1;
static const char* current_signal = NULL;

static double alignment = 2.54; // millimeters

static rect wire_bounds;
static xy translate;
static rect contour;

static const char* base;
static int active_layer = -1;
static stralloc current_alignment;

static struct { xmlnode *parts, *sheets, *elements; } nodes;

static double
human_to_mm(const char* measure) {
  double d = 0;
  size_t i;

  if((i = scan_double(measure, &d)) > 0) {
    if(str_start(&measure[i], "in") || measure[i] == '"') {
      d *= 2.54;
    } else if(str_start(&measure[i], "mil")) {
      d *= 0.0254;
    } else if(str_start(&measure[i], "cm")) {
      d *= 10;
    }
  }
  return d;
}

static struct pos
xy_neg(const struct pos p) {
  struct pos r;
  r.x = -p.x;
  r.y = -p.y;
  return r;
}
static void
xy_add(struct pos* d, const struct pos* p) {
  d->x += p->x;
  d->y += p->y;
}

static inline double
round_to(double d, double rounding) {
  d /= rounding;
  d = (long)(d + 0.5);
  d *= rounding;
  return d;
}

void
xy_align(double* x, double* y) {
  *x = round_to(*x, alignment);
  *y = round_to(*y, alignment);
}

/**
 * Reads a real-number value from the
 * element/attribute given
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
 * Reads an integer number value from
 * the element/attribute given
 */
int
get_int(xmlnode* node, const char* key) {
  long ret = INT_MIN;
  const char* istr = xml_get_attribute(node, key);

  if(istr) {
    while(*istr && str_chr("-0123456789", *istr) >= 11)
      ++istr;
    if(!scan_long(istr, &ret))
      ret = INT_MAX;
  }
  return ret;
}

/**
 * Gets a parent element by name.
 */
xmlnode*
get_parent(void* n, const char* parent) {
  xmlnode* node = n;

  for(node = n; node; node = node->parent) {
    if(node->name == NULL)
      continue;
    if(str_equal(node->name, parent))
      break;
  }
  return (xmlnode*)node;
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

wire
get_wire(xmlnode* node) {
  wire ret;
  stralloc p;
  ret.x1 = get_double(node, "x1");
  ret.y1 = get_double(node, "y1");
  ret.x2 = get_double(node, "x2");
  ret.y2 = get_double(node, "y2");

#ifdef DEBUG_REFLIST
  stralloc_init(&p);
  xml_path(node, &p);
  buffer_putsa(buffer_2, &p);
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}

/**
 * Gets a cbmap_t element.
 */
void*

get(cbmap_t m, char* name, size_t datasz) {
  void* ptr;
  size_t len = datasz;
  if(!cbmap_get(m, name, str_len(name) + 1, &ptr, &len))
    ptr = NULL;
  return ptr;
}

/**
 * Gets or creates a cbmap_t element.
 */
void*
get_or_create(cbmap_t m, char* name, size_t datasz) {
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
    if(cbmap_insert(m, name, str_len(name) + 1, data, datasz))
      ptr = get(m, name, datasz);
#if !defined(HAVE_ALLOCA) && !defined(HAVE_DYNSTACK)
    free(data);
#endif
  }
  return ptr;
}

//}
/**
 * Index a cbmap
 */
void*
get_entry(cbmap_t map, const char* key) {
  size_t len = str_len(key) + 1;
  void* ret = NULL;
  cbmap_get(map, (void*)key, len, &ret, &len);
  return ret;
}
void
print_base(buffer* b) {
  buffer_putm_internal(b, base, ": ", 0);
}

/**
 * Outputs name/value pair
 */
void
print_name_value(buffer* b, const char* name, const char* value) {
  print_base(b);
  if(name)
    buffer_putm_internal(b, name, ": ", 0);
  buffer_puts(b, value ? value : "(null)");
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

int
layer_id(const char* str) {
  int id = -1;
  if(scan_uint(str, (unsigned int*)&id)) {
    // if(id >= 0 && id <
    // strarray_size(&layers))
    return id;
  }
  return -1;
}

int
get_layer(const char* str) {
  long i, n = strarray_size(&layers);

  if((i = strarray_index_of(&layers, str)) < n)
    return i;

  if((i = layer_id(str)) != -1)
    return i;
  if(scan_long(str, &i) > 0)
    return i;

  return -1;
}

const char*
layer_name(int i) {
  return strarray_at(&layers, i);
}

const char*
layer_by_id(const char* str) {
  int id;
  if((id = get_layer(str)) != -1)
    return layer_name(id);
  return NULL;
}
const char*
layer_index(long idx) {
  return strarray_at(&layers, idx);
}

void
build_layers(xmlnode* layer) {
  const char* layerNum = xml_get_attribute(layer, "number");

  if(layerNum) {
    long num = -1;
    const char* name = xml_get_attribute(layer, "name");

    if(name && scan_long(layerNum, &num) > 0)

      strarray_set(&layers, num, name);
  }

  // xml_print(layer, buffer_2);
}

void
rect_translate(rect* r, struct pos* p) {
  xy_add(&r->a, p);
  xy_add(&r->b, p);
}

int
node_align(xmlnode* node) {
  static const char* const attr_names[] = {"x", "y", "x1", "y1", "x2", "y2"};
  size_t i;
  int ret = 0;

  for(i = 0; i < (sizeof(attr_names) / sizeof(attr_names[0])); ++i) {

    if(xml_has_attribute(node, attr_names[i])) {
      double newval, oldval = get_double(node, attr_names[i]);

      newval = round_to(oldval, alignment);

      if(fabs(newval - oldval) >= DBL_EPSILON) {
        ret = 1;

        xml_set_attribute_double(node, attr_names[i], newval, 3);
      }
    }
  }

  if(ret) {
    stralloc np;
    stralloc_init(&np);
    xml_path(node, &np);

    buffer_puts(buffer_2, "aligned ");
    buffer_putsa(buffer_2, &np);
    buffer_puts(buffer_2, " ");
    xml_print_attributes(node->attributes, buffer_2, " ", "=", "'");
    buffer_putnlflush(buffer_2);

    stralloc_free(&np);
  }

  return ret;
}

void
rect_update(rect* r, double x, double y) {
  if(x < r->x1 || r->x1 == DBL_MAX)
    r->x1 = x;
  if(x > r->x2 || r->x2 == DBL_MAX)
    r->x2 = x;
  if(y < r->y1 || r->y1 == DBL_MAX)
    r->y1 = y;
  if(y > r->y2 || r->y2 == DBL_MAX)
    r->y2 = y;
}

void
rect_round(rect* r, double prec) {
  r->x1 = round_to(r->x1, prec);
  r->y1 = round_to(r->y1, prec);
  r->x2 = round_to(r->x2, prec);
  r->y2 = round_to(r->y2, prec);
}
void
rect_mult(rect* r, double f) {
  r->x1 *= f;
  r->y1 *= f;
  r->x2 *= f;
  r->y2 *= f;
}

void
rect_outset(rect* r, double offset) {
  r->x1 -= offset;
  r->x2 += offset;
  r->y1 -= offset;
  r->y2 += offset;
}

void
rect_zero(rect* r) {
  r->x1 = 0;
  r->y1 = 0;
  r->x2 = 0;
  r->y2 = 0;
}

void
rect_init(rect* r, double d) {
  r->x1 = d;
  r->y1 = d;
  r->x2 = d;
  r->y2 = d;
}

void
wire_zero(wire* r) {
  r->x1 = 0;
  r->y1 = 0;
  r->x2 = 0;
  r->y2 = 0;
}

wire
line(xy a, xy b) {
  wire ret;

  ret.x1 = a.x;
  ret.y1 = a.y;

  ret.x2 = b.x;
  ret.y2 = b.y;
  return ret;
}

xy
vec(double x, double y) {
  xy ret;
  ret.x = x;
  ret.y = y;
  return ret;
}

void
print_wire(buffer* b, const struct wire w) {
  // print_base(b);
  buffer_puts(b, "Wire ");
  print_xy(b, NULL, w.x1, w.y1);
  buffer_puts(b, " ");
  print_xy(b, NULL, w.x2, w.y2);
  /*
    print_vertex(b, vec(w.x1, w.y1));
    buffer_puts(b, " ");
    print_vertex(b, vec(w.x2, w.y2));
   */
}

void
check_wire(xmlnode* node) {
  const char* layerId;

  if((layerId = xml_get_attribute(node, "layer"))) {
    int layer = atoi(layerId);
    wire w = get_wire(node);

    if(layer == measures_layer) {

      array_catb(&wires, &w, sizeof(wire));

      xml_delete(node);
    } else if(layer == bottom_layer || layer == get_layer(current_layer)) {
      rect_update(&wire_bounds, w.x1, w.y1);
      rect_update(&wire_bounds, w.x2, w.y2);
    }
  }
}

/**
 * Build structures from <part> or
 * <element> element
 */
void
build_part(xmlnode* part) {
  char *val, *dsname;
  char* name = xml_get_attribute(part, "name");
  char* pkgname = xml_get_attribute(part, "package");
  size_t pins;
  struct part p;
  if(!name || str_len(name) == 0)
    return;
  byte_zero(&p, sizeof(struct part));
  stralloc_copys(&p.name, name);
  val = xml_get_attribute(part, "value");
  if(val)
    stralloc_copys(&p.value, val);
  p.x = get_double(part, "x") / 0.127;
  p.y = get_double(part, "y") / 0.127;

  if(pkgname && str_len(pkgname)) {
    p.pkg = get_entry(packages, pkgname);
  }
  // assert(p.pkg);
  if(p.pkg) {
    pins = array_length(&p.pkg->pads, sizeof(struct net*));
    p.pins = calloc(sizeof(struct net*), pins);
  }
  dsname = xml_get_attribute(part, "deviceset");
  if(dsname)
    p.dset = get_entry(devicesets, dsname);
  cbmap_insert(parts, (void*)name, str_len(name) + 1, &p, sizeof(struct part));
}

/**
 * Build structures from <symbol>
 * element
 */
void
build_sym(xmlnode* part) {
  size_t i = 0;
  struct symbol* sym;
  xmlnode* pin;
  char* name = xml_get_attribute(part, "name");
  if(!name || str_len(name) == 0)
    return;
  sym = get_or_create(symbols, name, sizeof(struct symbol));
  stralloc_copys(&sym->name, name);

  for(pin = part->children; pin; pin = pin->next) {
    char* pin_name;
    struct pin* p;
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

/**
 * @param node   Parent is the 'net' or
 * 'signal' element
 */
void
build_reflist(xmlnode* node, struct net* n, int* index) {

  for(; node; node = node->next) {
    char *nn, *part_name;
    struct ref* r;
    int is_pin;
    if(node->type != XML_ELEMENT)
      continue;
    nn = node->name;

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

#ifdef DEBUG_REFLIST
    print_name_value(buffer_2, nn, part_name);
    buffer_putnlflush(buffer_2);
#endif

    if(r->part->pkg) {
      r->pin = package_pin(r->part->pkg, xml_get_attribute(node, "pad"));
    }

#ifdef DEBUG_REFLIST
    print_name_value(buffer_2, nn, part_name);
    buffer_putc(buffer_2, '\t');
    print_element_attrs(node);
    buffer_putnlflush(buffer_2);
#endif
  }
}

/**
 * Build reference list from
 *
 *      <contactref element="IC1"
 * pad="4"/>
 *
 *      <pinref part="IC1" gate="B"
 * pin="O"/>
 */
void
build_nets(xmlnode* node) {
  int i;
  struct net* ptr;
  xmlnode* sn;
  char *sign, *name = node->name;
  assert(str_equal(name, "net") || str_equal(name, "signal"));
  if(!(sign = xml_get_attribute(node, "name")))
    return;

#ifdef DEBUG_REFLIST
  print_name_value(buffer_2, name, sign);
  buffer_putnlflush(buffer_2);
#endif

  ptr = get_or_create(nets, sign, sizeof(struct net));
  stralloc_copys(&ptr->name, sign);
  i = 0;
  build_reflist(node->children, ptr, &i);
}

/**
 * Build structures from <package>
 * element
 */
void
build_package(xmlnode* set) {
  xmlnode* node;
  char *pn, *name = xml_get_attribute(set, "name");
  struct package pkg;
  byte_zero(&pkg, sizeof(struct package));
  if(name == NULL)
    name = "";
  stralloc_copys(&pkg.name, name);

  for(node = set->children; node; node = node->next) {
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
  cbmap_insert(packages, name, str_len(name) + 1, &pkg, sizeof(struct package));
}

/**
 * Build structures from <deviceset>
 * element
 */
void
build_deviceset(xmlnode* set) {
  struct deviceset d;
  xmlnode *gates, *devices, *node;
  char* name = xml_get_attribute(set, "name");

#ifdef DEBUG_REFLIST
  print_name_value(buffer_2, "deviceset", name);
  buffer_putnlflush(buffer_2);
#endif

  byte_zero(&d, sizeof(struct deviceset));
  stralloc_copys(&d.name, name ? name : "(null)");
  d.devices = cbmap_new();
  gates = get_child(set, "gates");
  devices = get_child(set, "devices");

  for(node = gates ? gates->children : NULL; node; node = node->next) {
    struct gate g;
    if(node->type != XML_ELEMENT)
      continue;
    byte_zero(&g, sizeof(struct gate));
    stralloc_copys(&g.name, xml_get_attribute(node, "name"));
    stralloc_copys(&g.symbol, xml_get_attribute(node, "symbol"));
    array_catb(&d.gates, (const void*)&g, sizeof(struct gate));
  }

  for(node = devices ? devices->children : NULL; node; node = node->next) {
    char *name, *package;
    struct pinmapping pm;
    struct package* pkg;
    size_t len;
    if(node->type != XML_ELEMENT)
      continue;
    name = xml_get_attribute(node, "name");
    if(!name || str_len(name) == 0)
      continue;
    package = xml_get_attribute(node, "package");
    byte_zero(&pm, sizeof(struct pinmapping));

    cbmap_get(packages, package, str_len(package), (void**)&pkg, &len);
    pm.pkg = pkg;
    cbmap_insert(d.devices, name, str_len(name) + 1, &pm, sizeof(struct pinmapping));
  }
  //  cbmap_insert(devicesets, name,
  //  str_len(name) + 1, &d,
  //  sizeof(struct deviceset));
}

/**
 * Run an XPath query and return a XPath
 * object
 */
xmlnodeset

getnodeset(void* n, const char* xpath) {
  return xml_find_all_1(n, (xml_pred_t*)&xml_match_name, (void*)xpath);
}

/**
 * Retrieve all <part> (schematic) or
 * <element> (board) objects
 */
strlist

getparts(xmlnode* doc) {
  xmlnodeset ns;
  xmlnodeset_iter_t it, e;
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
 * Iterate through a node-set, calling a
 * functor for every item
 */
void
for_set(xmlnodeset* ns, void (*fn)(xmlnode*)) {
  xmlnodeset_iter_t it, e;
  if(!ns)
    return;

  for(it = xmlnodeset_begin(ns), e = xmlnodeset_end(ns); it != e; ++it) {

    fn(*it);
  }
}

/**
 * Get the top-leftmost x and y
 * coordinate from a set of nodes.
 */
void
nodeset_topleft(xmlnodeset* s, double* x, double* y) {
  xmlnode* node;
  int i, len = xmlnodeset_size(s);
  if(len == 0)
    return;
  node = xmlnodeset_item(s, 0);
  *x = get_double(node, "x");
  *y = get_double(node, "y");

  for(i = 1; i < len; ++i) {
    double nx, ny;
    node = xmlnodeset_item(s, i);
    nx = get_double(node, "x");
    ny = get_double(node, "y");
    if(nx < *x)
      *x = nx;
    if(ny < *y)
      *y = ny;
  }
}

/**
 * get extrema from x/y attrs
 */
void
tree_topleft(xmlnode* elem, const char* elems, double* x, double* y) {
  xmlnode* node = elem->children;
  if(node == 0)
    return;
  while(node && node->type != XML_ELEMENT && str_diff(node->name, elems))
    node = node->next;
  *x = get_double(node, "x");
  *y = get_double(node, "y");

  while((node = node->next)) {
    double nx, ny;
    if(node->type != XML_ELEMENT || str_diff(node->name, elems))
      continue;
    nx = get_double(node, "x");
    ny = get_double(node, "y");
    if(nx < *x)
      *x = nx;
    if(ny < *y)
      *y = ny;
  }
}

int
dump_package(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  int64 i;
  const struct package* pkg = value;
  buffer_puts(buffer_1, "dump_package: ");
  buffer_putsa(buffer_1, &pkg->name);
  buffer_puts(buffer_1, " [");

  for(i = 0; i < array_length(&pkg->pads, sizeof(struct pad)); ++i) {
    const struct pad* p = array_get(&pkg->pads, sizeof(struct pad), i);
    buffer_putspace(buffer_1);
    buffer_putsa(buffer_1, &p->name);
  }
  buffer_puts(buffer_1, " ]");
  buffer_putnlflush(buffer_1);
  return 1;
}

int
dump_part(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  struct part* ptr = (struct part*)value;
  assert(ptr->name.s);
  buffer_puts(buffer_2, "dump_part: ");
  buffer_putsa(buffer_2, &ptr->name);

  if(ptr->pkg) {
    buffer_puts(buffer_2, " package: ");
    buffer_putsa(buffer_2, &ptr->pkg->name);
  }
  cbmap_visit_all(nets, dump_net, ptr);

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

int
dump_net(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  struct net* n = (struct net*)value;
  struct part* p = user_data;
  struct ref* rc;
  int64 i, len;
  if(!(rc = net_connects(n, p, -1)))
    return 1;
  len = array_length(&n->contacts, sizeof(struct ref));

  for(i = 0; i < len; ++i) {
    struct ref* r = array_get(&n->contacts, sizeof(struct ref), i);
    if(r == rc)
      continue;
    assert(r->part);
    buffer_putsa(buffer_1, &p->name);
    buffer_puts(buffer_1, ".");
    buffer_putlong(buffer_1, rc->pin + 1);
    buffer_puts(buffer_1, "\t");
    buffer_putsa(buffer_1, &r->part->name);
    buffer_puts(buffer_1, ".");
    buffer_putlong(buffer_1, r->pin + 1);
    buffer_putnlflush(buffer_1);
  }
  return 1;
}

/**
 *  node_print: Prints XML node
 */
void
node_print(xmlnode* node) {
  buffer_putm_internal(buffer_1, "<", node->name, 0);
  print_element_attrs(node);
  buffer_puts(buffer_1, ">");
  buffer_putnlflush(buffer_1);
}

/**
 *  hashmap_dump: Gets depth of node in
 * hierarchy
 */
int
node_depth(xmlnode* node) {
  size_t i = 0;
  while((node = node->next))
    ++i;
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
str_isfloat(const char* s) {
  return str_ischarset(s, "0123456789.-+Ee");
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

  /*  if(a_node->parent) {
     xmlnode* p = a_node->parent;
     const char* pn = p->name;

     if(pn && !str_diffn(pn, name,
   str_len(name))) { p = p->parent;
     }
     print_element_name(p);
   } */
  if(!(name = a_node->name))
    return;

  if(str_diff(name, "eagle") && str_diff(name, "drawing")) {
    buffer_putm_internal(buffer_1, a_node->parent ? "/" : "", name, 0);
    if(!(name = xml_get_attribute(a_node, "name")))
      return;
    if(str_len(name))
      buffer_putm_internal(buffer_1, "[@name='", name, "']", 0);
  }
}

/**
 *  print_element_attrs: Prints all
 * element attributes to stdout
 */
void
print_attrs(HMAP_DB* a) {
  TUPLE* t;

  for(t = a->list_tuple; t; t = t->next) {
    char* v = t->vals.val_chars;
    buffer_putm_internal(buffer_1, " ", t->key, str_isdoublenum(v) ? "=" : "=\"", v, str_isdoublenum(v) ? "" : "\"", 0);
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
      buffer_putm_internal(buffer_1, " \"", s, "\"", 0);
  }
}

/**
 *  print_element_children: Prints all
 * element attributes to stdout
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
    buffer_putnlflush(buffer_1);
    print_element_children(node);
  }
}

/**
 * print_element_names:
 * @a_node: the initial xml node to
 * consider.
 *
 * Prints the names of the all the xml
 * elements that are siblings or
 * children of a given xml node.
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
    buffer_putnlflush(buffer_1);
    print_element_names(node->children);
  }
}

void
print_xy(buffer* b, const char* name, double x, double y) {
  if(name) {
    print_base(b);
    buffer_putm_internal(b, name, " ", 0);
  }
  buffer_puts(b, "(");
  buffer_putdouble(b, x / 10, 4);
  buffer_puts(b, " ");
  buffer_putdouble(b, y / 10, 4);
  buffer_puts(b, ")");

  if(name)
    buffer_putnlflush(b);
  else
    buffer_flush(b);
}

void
print_vertex(buffer* b, const xy v) {
  buffer_putc(b, '(');
  buffer_putdouble(b, v.x, 4);
  buffer_putspace(b);
  buffer_putdouble(b, v.y, 4);
  buffer_putc(b, ')');
}

void
print_rect(buffer* b, const char* name, const rect* r) {
  if(name) {
    print_base(b);
    buffer_putm_internal(b, name, " ", 0);
  }
  print_vertex(b, r->a);
  buffer_putspace(b);
  print_vertex(b, r->b);
  if(name)
    buffer_putnlflush(b);
}

void
print_bounds(buffer* b, const rect r, const char* sep) {
  xy point[4];
  wire w[4];
  int i;

  point[0] = vec(r.x1, r.y1);
  point[1] = vec(r.x2, r.y1);
  point[2] = vec(r.x2, r.y2);
  point[3] = vec(r.x1, r.y2);

  w[0] = line(point[0], point[1]);
  w[1] = line(point[1], point[2]);
  w[2] = line(point[2], point[3]);
  w[3] = line(point[3], point[0]);

  for(i = 0; i < 4; i++) {
    if(i > 0)
      buffer_puts(b, sep ? sep : "; ");
    print_wire(buffer_1, w[i]);
    buffer_puts(b, " 0.01");
  }
}

void
print_xml_xy(buffer* b, xmlnode* e) {
  double x = xml_get_attribute_double(e, "x");
  double y = xml_get_attribute_double(e, "y");
  xy_align(&x, &y);
  buffer_puts(b, "(");
  buffer_putdouble(b, x / 25.4, 2);
  buffer_puts(b, " ");
  buffer_putdouble(b, y / 25.4, 2);
  buffer_puts(b, ")");
}

void
print_xml_rect(buffer* b, xmlnode* e) {
  const char *x1, *y1, *x2, *y2;
  x1 = xml_get_attribute(e, "x1");
  y1 = xml_get_attribute(e, "y1");
  x2 = xml_get_attribute(e, "x2");
  y2 = xml_get_attribute(e, "y2");
  buffer_putm_internal(b,
                       "(",
                       x1 ? x1 : "<null>",
                       " ",
                       y1 ? y1 : "<null>",
                       ") (",
                       x2 ? x2 : "<null>",
                       " ",
                       y2 ? y2 : "<null>",
                       ")",
                       NULL);
  buffer_flush(b);
}

void
print_xml_layer(buffer* b, xmlnode* e) {
  const char* layer = xml_get_attribute(e, "layer");

  if(layer) {
    long layer_id = xml_get_attribute_long(e, "layer");

    if(layer_id != current_layer_id) {
      const char* layerName = layer_index(layer_id);
      buffer_putm_internal(b, "Layer ", layerName ? layerName : layer, ";\n", NULL);

      current_layer_id = layer_id;
    }
  }
}

void
print_script(buffer* b, xmlnode* e) {
  stralloc cmd;
  stralloc_init(&cmd);
  if(e->name)
    stralloc_copys(&cmd, e->name);
  if(cmd.len)
    cmd.s[0] = toupper(cmd.s[0]);
  stralloc_append(&cmd, " ");
  stralloc_nul(&cmd);

  if(xml_has_attribute(e, "layer")) {
    int layer = get_layer(xml_get_attribute(e, "layer"));
    if(layer != -1 && layer != active_layer) {
      buffer_putm_internal(b, "Layer ", layer_name(layer), "; ", 0);
      active_layer = layer;
    }
  }

  if(xml_has_attribute(e, "align")) {
    const char* a;
    stralloc align;
    stralloc_init(&align);
    // xml_get_attribute_sa(e, &align,
    // "align");

    if((a = xml_get_attribute(e, "align"))) {
      stralloc_subst(&align, a, str_len(a), "-", " ");
      if(align.len) {
        if(!stralloc_case_equal(&align, &current_alignment)) {
          stralloc_copy(&current_alignment, &align);
          stralloc_nul(&current_alignment);
          buffer_putm_internal(b, "CHANGE ALIGN ", current_alignment.s, "; ", 0);
        }
      }
    }
  }

  if(str_equal(e->name, "element")) {
    const char* rot = xml_get_attribute(e, "rot");
    buffer_putm_internal(b, "MOVE ", xml_get_attribute(e, "name"), " ", NULL);

    print_xml_xy(b, e);
    if(rot) {
      buffer_putm_internal(b, "; ROTATE ", rot, " '", xml_get_attribute(e, "name"), "'", NULL);
    }

  } else if(str_equal(e->name, "wire")) {
    print_xml_layer(b, e);

    buffer_putsa(b, &cmd);
    if(current_signal)
      buffer_putm_internal(b, "'", current_signal, "' ", 0);
    buffer_putm_internal(b, xml_get_attribute(e, "width"), " ", 0);

    print_xml_rect(b, e);
  } else if(str_equal(e->name, "via")) {
    buffer_putsa(b, &cmd);
    if(current_signal)
      buffer_putm_internal(b, "'", current_signal, "' ", 0);

    buffer_putm_internal(b, xml_get_attribute(e, "extent"), " ", xml_get_attribute(e, "shape"), " ", 0);
    print_xml_xy(b, e);
  } else if(str_equal(e->name, "pad")) {
    buffer_putm_internal(b,
                         cmd.s,
                         "'",
                         xml_get_attribute(e, "name"),
                         "'",
                         " ",
                         xml_get_attribute(e, "diameter"),
                         " ",
                         xml_get_attribute(e, "shape"),
                         " ",
                         xml_get_attribute(e, "orientation"),
                         " ",
                         0);
    print_xml_xy(b, e);
  } else if(str_equal(e->name, "hole")) {
    buffer_putm_internal(b, cmd.s, xml_get_attribute(e, "diameter"), " ", 0);
    print_xml_xy(b, e);
  } else if(str_equal(e->name, "circle")) {
    buffer_putm_internal(b, cmd.s, xml_get_attribute(e, "width"), " ", 0);
    print_xml_xy(b, e);
  } else if(str_equal(e->name, "rectangle")) {
    buffer_putm_internal(b, cmd.s, xml_get_attribute(e, "orientation"), " ", 0);
    print_xml_rect(b, e);
  } else if(str_equal(e->name, "text")) {

    buffer_putm_internal(b, cmd.s, "'", xml_content(e), "' ", xml_get_attribute(e, "orientation"), " ", 0);

    print_xml_xy(b, e);
  } else {
#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "No such element: ", e->name, 0);
    buffer_putnlflush(buffer_2);
#endif /* defined DEBUG_OUTPUT */
    return;
  }
  buffer_putc(b, ';');

  buffer_putnlflush(b);
}

int
buffer_read(void* ptr, char* buf, int len) {
  return buffer_get(ptr, buf, len);
}

void
match_query(xmlnode* doc, const char* q) {
  xmlnodeset ns;
  xmlnodeset_iter_t it, e;

#ifdef DEBUG_QUERY
  print_name_value(buffer_1, "XPath query", q);
  buffer_putnlflush(buffer_1);
#endif
  ns = getnodeset(doc, q);

  for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
    xmlnode* node = *it;
    print_element_name(node);
    print_element_attrs(node);
    buffer_putnlflush(buffer_1);
    print_element_children(node);
    buffer_putnlflush(buffer_1);

    if(0) { //! str_diff(q, xq)) {
      TUPLE* a;
      char* elem_name = (char*)&q[2];
      stralloc query;
      stralloc_init(&query);
      elem_name = "*";

      for(a = hmap_begin(node->attributes); a; a = hmap_next(node->attributes, a)) {
        strlist part_names;
        char* attr_name = a->key;
        char* v = a->vals.val_chars;
        strlist_init(&part_names, ' ');
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

        strlist_dump(buffer_1, &part_names);
      }
    }
  }
}

void
draw_measures(xmlnode* doc) {
  xmlnode *left, *right, *bottom, *top, *gnd_signal, *polygon, *n;

  xmlnode* plain = xml_find_element(doc, "plain");

  if(plain->type != XML_ELEMENT) {
    buffer_putsflush(buffer_2, "element 'plain'\n");
    exit(1);
  }

  left = xml_element("wire");
  xml_set_attribute_double(left, "x1", bounds.x1, 3);
  xml_set_attribute_double(left, "x2", bounds.x1, 3);
  xml_set_attribute_double(left, "y1", bounds.y1, 3);
  xml_set_attribute_double(left, "y2", bounds.y2, 3);
  xml_set_attribute_double(left, "layer", measures_layer, 3);

  right = xml_element("wire");
  xml_set_attribute_double(right, "x1", bounds.x2, 3);
  xml_set_attribute_double(right, "x2", bounds.x2, 3);
  xml_set_attribute_double(right, "y1", bounds.y1, 3);
  xml_set_attribute_double(right, "y2", bounds.y2, 3);
  xml_set_attribute_double(right, "layer", measures_layer, 3);

  bottom = xml_element("wire");
  xml_set_attribute_double(bottom, "x1", bounds.x1, 3);
  xml_set_attribute_double(bottom, "x2", bounds.x2, 3);
  xml_set_attribute_double(bottom, "y1", bounds.y1, 3);
  xml_set_attribute_double(bottom, "y2", bounds.y1, 3);
  xml_set_attribute_double(bottom, "layer", measures_layer, 3);

  top = xml_element("wire");
  xml_set_attribute_double(top, "x1", bounds.x1, 3);
  xml_set_attribute_double(top, "x2", bounds.x2, 3);
  xml_set_attribute_double(top, "y1", bounds.y2, 3);
  xml_set_attribute_double(top, "y2", bounds.y2, 3);
  xml_set_attribute_double(top, "layer", measures_layer, 3);

  xml_add_child(plain, left);
  xml_add_child(plain, top);
  xml_add_child(plain, right);
  xml_add_child(plain, bottom);

  print_script(buffer_1, left);
  print_script(buffer_1, top);
  print_script(buffer_1, right);
  print_script(buffer_1, bottom);

  gnd_signal = xml_find_element_attr(doc, "signal", "name", "GND");
  polygon = xml_find_element(gnd_signal, "polygon");

  /*  if(polygon == NULL) {
     polygon = xml_element("polygon");

     xml_set_attribute(polygon, "width",
   "0.254"); xml_set_attribute(polygon,
   "layer", "16");
     xml_set_attribute(polygon,
   "isolate", "1.27");
     xml_set_attribute(polygon,
   "thermals", "yes");

     xml_add_child(gnd_signal, polygon);
   }
    if(polygon->children) {
     xml_free(polygon->children);
     polygon->children = NULL;
   }

   n = xml_element("vertex");
   xml_set_attribute_double(n, "x",
   bounds.x1, 3);
   xml_set_attribute_double(n, "y",
   bounds.y2, 3); xml_add_child(polygon,
   n); n = xml_element("vertex");
   xml_set_attribute_double(n, "x",
   bounds.x1, 3);
   xml_set_attribute_double(n, "y",
   bounds.y1, 3); xml_add_child(polygon,
   n); n = xml_element("vertex");
   xml_set_attribute_double(n, "x",
   bounds.x2, 3);
   xml_set_attribute_double(n, "y",
   bounds.y1, 3); xml_add_child(polygon,
   n); n = xml_element("vertex");
   xml_set_attribute_double(n, "x",
   bounds.x2, 3);
   xml_set_attribute_double(n, "y",
   bounds.y2, 3); xml_add_child(polygon,
   n);
   */
}

/**
 * Executes XPath query and for every
 * resulting element calls a function
 */
#define match_foreach(doc, q, fn) \
  do { \
    xmlnodeset ns = getnodeset(doc, q); \
    if(xmlnodeset_size(&ns)) { \
      for_set(&ns, fn); \
    } \
  } while(0);

void
usage(char* progname) {
  buffer_putm_internal(buffer_1, "Usage: ", progname, " [OPTIONS] [PACKAGES...]\n", 0);
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1,
              "  --help, -h                    "
              "    show this help\n");
  buffer_puts(buffer_1,
              "  --layer, -l NUM       "
              "Layer name/number\n");
  buffer_puts(buffer_1,
              "  --layers, -L          "
              "List layers\n");
  buffer_puts(buffer_1,
              "  --draw, -d            "
              "Draw measures\n");
  buffer_puts(buffer_1,
              "  --align-by, -A NUM    Align "
              "to (inch, mm, mil)\n");
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int c;
  int index = 0;
  rect extent, extent2;
  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'},
                                {"layer", 1, NULL, 'l'},
                                {"layers", 0, NULL, 'L'},
                                {"draw", 0, NULL, 'd'},
                                {"align", 0, NULL, 'a'},
                                {"align-by", 0, NULL, 'A'},
                                {"comments", 0, NULL, 'c'},
                                {0, 0, 0, 0}};

  for(;;) {
    c = unix_getopt_long(argc, argv, "LdhaA:l:c", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'L': do_list_layers = 1; break;
      case 'd': do_draw_measures = 1; break;
      case 'a': do_align_coords = 1; break;
      case 'c': print_comments = 1; break;
      case 'A':
        do_align_coords = 1;
        alignment = human_to_mm(unix_optarg);
        break;
      case 'l': current_layer = unix_optarg; break;
      default: usage(argv[0]); return 1;
    }
  }
  rect_zero(&bounds);
  rect_zero(&wire_bounds);

  {
    buffer input, out;
    xmlnode* doc;
    xmlnode* root;
    xmlnode *plain, *left, *right, *bottom, *top, *gnd_signal, *polygon, *n;
    stralloc path;
    stralloc_init(&path);

    devicesets = cbmap_new();
    packages = cbmap_new();
    parts = cbmap_new();
    nets = cbmap_new();
    symbols = cbmap_new();

    if(!argv[unix_optind]) {
      argv[unix_optind] = "/home/roman/Sources/"
                          "an-tronics/eagle/"
                          "40106-4069-Synth.brd";
    } else if(argv[unix_optind + 1]) {
      xq = argv[unix_optind + 1];
    }

    base = str_basename(argv[unix_optind]);
    buffer_mmapprivate(&input, argv[unix_optind]);
    buffer_skip_until(&input, "\r\n", 2);
    doc = xml_read_tree(&input);

    match_foreach(doc, "layer", build_layers);

    dump_strarray("layers", &layers);

    if(do_list_layers) {

      size_t i, n = strarray_size(&layers);
      for(i = 0; i < n; ++i) {
        const char* s = strarray_at(&layers, i);

        if(s) {
          stralloc num;
          stralloc_init(&num);
          stralloc_catlong(&num, i);
          stralloc_nul(&num);

          print_name_value(buffer_2, num.s, s);
          buffer_putnlflush(buffer_2);
        }
      }

      return 0;
    }

    match_query(doc, xq);

    match_foreach(doc, "package", build_package);

    print_base(buffer_2);
    buffer_puts(buffer_2, "items in packages: ");
    buffer_putulong(buffer_2, cbmap_count(packages));
    buffer_putnlflush(buffer_2);

    match_foreach(doc, "deviceset", build_deviceset);
    match_foreach(doc, "part|element", build_part);

    print_base(buffer_2);
    buffer_puts(buffer_2, "items in parts: ");
    buffer_putulong(buffer_2, cbmap_count(parts));
    buffer_putnlflush(buffer_2);

    match_foreach(doc, "net|signal", build_nets);
    match_foreach(doc, "symbol", build_sym);

    /* cbmap_visit_all(packages,
     * dump_package, "package"); */
    /* cbmap_visit_all(parts, dump_part,
     * "part"); */

    /*  for(size_t i = 0; i <
     * array_length(&layers.a,
     * sizeof(char*)); ++i) { */
    /*    buffer_puts(buffer_2, "layer
     * "); */
    /*    buffer_putlong(buffer_2, i);
     */
    /*    const char* name =
     * *(char**)array_get(&layers.a,
     * sizeof(char*), i); */
    /*    buffer_putm_internal(buffer_2,
     * " \"", name ? name : "", "\"",
     * 0); */
    /*    buffer_putnlflush(buffer_2);
     */
    /*  } */

    measures_layer = strarray_index_of(&layers, "Measures");
    bottom_layer = strarray_index_of(&layers, "Bottom");

    print_base(buffer_2);
    buffer_puts(buffer_2, "Measures layer: ");
    buffer_putlong(buffer_2, measures_layer);
    buffer_putnlflush(buffer_2);

    match_foreach(doc, "wire", check_wire);

    {
      int n;
      double top_y, right_x;
      xmlnodeset_iter_t it, e;
      xmlnodeset ns;
      stralloc layer_str;
      int num_aligned = 0;

      // libraries =
      // xml_find_element(doc,
      // "libraries");

      stralloc_init(&layer_str);

      stralloc_catlong(&layer_str, get_layer(current_layer));
      stralloc_nul(&layer_str);

      print_base(buffer_2);
      buffer_putm_internal(buffer_2, "layer str: ", layer_str.s, 0);
      buffer_putnlflush(buffer_2);

      rect_zero(&extent);
      rect_init(&extent2, DBL_MAX);
      // ns = xml_find_with_attrs(doc,
      // "x|y|x1|y1|x2|y2");

      nodes.elements = xml_find_element(doc, "elements");
      nodes.parts = xml_find_element(doc, "parts");
      nodes.sheets = xml_find_element(doc, "sheets");

      root = nodes.sheets ? nodes.sheets : nodes.elements;

      ns = xml_find_all_attrs(root, "x|y|x1|y1");

      n = xmlnodeset_size(&ns);

      buffer_puts(buffer_2, "Nodes with coordinates: ");
      buffer_putulong(buffer_2, n);
      buffer_putnlflush(buffer_2);

      for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
        xmlnode* node = xmlnodeset_iter_ref(it);
        double x1, x2, y1, y2;

        if(print_comments) {
          buffer_putm_internal(buffer_1, "\n# Element '", node->name, "' XPath: ", NULL);
          buffer_puts(buffer_1, "# ");
          xml_path(node, &path);
          buffer_putsa(buffer_1, &path);
        }
        // if(xml_has_attribute(node,
        // "layer")) layer =
        // xml_get_attribute(node,
        // "layer");

        if(do_align_coords) {
          if(get_parent(node, "libraries") == 0)
            num_aligned += node_align(node);
        }

        if(xml_has_attribute(node, "x1") && xml_has_attribute(node, "y1"))
          rect_update(&extent, get_double(node, "x1"), get_double(node, "y1"));
        if(xml_has_attribute(node, "x2") && xml_has_attribute(node, "x2"))
          rect_update(&extent, get_double(node, "x2"), get_double(node, "x2"));

        if(str_equal(node->name, "element")) {
          if(xml_has_attribute(node, "x") && xml_has_attribute(node, "y"))
            rect_update(&extent2, get_double(node, "x"), get_double(node, "y"));
        }

        {
          xmlnode* named = node;

          while((named = xml_find_parent_attr(named, "name"))) {
            const char* name = xml_get_attribute(named, "name");

            if(str_equal(named->name, "signal")) {
              if(current_signal == NULL || str_diff(name, current_signal)) {
                current_signal = name;

                if(print_comments) {
                  buffer_puts(buffer_1, "[@");

                  xml_print_attributes(named->attributes, buffer_1, ", @", "=", "'");
                  buffer_puts(buffer_1, "]");
                }
              }
            }
          }

          //      print_xy(buffer_2,
          //      layer, x1, y1);
        }
        if(print_comments)
          buffer_putsflush(buffer_1, "\n\n");
        print_script(buffer_1, node);
      }

      buffer_flush(buffer_1);

      print_rect(buffer_2, "extent", &extent);

      translate = xy_neg(extent.a);

      print_xy(buffer_2, "translate", translate.x, translate.y);

      contour = extent;
      rect_translate(&contour, &translate);

      print_rect(buffer_2, "contour", &contour);

      if(do_draw_measures || num_aligned) {
        if(do_draw_measures)
          draw_measures(doc);

        buffer_puts(buffer_2, "Aligned ");
        buffer_putlong(buffer_2, num_aligned);
        buffer_putm_internal(buffer_2, " nodes\n", "Saving to '", base, "'...", NULL);
        buffer_putnlflush(buffer_2);
        {
          stralloc filename;
          stralloc_init(&filename);
          stralloc_copys(&filename, base);
          stralloc_cats(&filename, ".new");
          stralloc_nul(&filename);

          buffer_truncfile(&out, filename.s);

          xml_print(doc, &out, 0);
          stralloc_free(&filename);
        }
      }

      /*    print_xy(buffer_2,
         "extent.1", extent.x1,
         extent.y1); print_xy(buffer_2,
         "extent.2", extent.x2,
         extent.y2);
      */
      // xml_print_nodeset(&wires,
      // buffer_1);

      //      buffer_puts(buffer_2,
      //      "top_y: ");
      //      buffer_putdouble(buffer_2,
      //      top_y, 1);
      //      buffer_putnlflush(buffer_2);
      //      buffer_puts(buffer_2,
      //      "right_x: ");
      //      buffer_putdouble(buffer_2,
      //      right_x, 1);
      //      buffer_putnlflush(buffer_2);
    }

    /*  cbmap_visit_all(nets, dump_net,
     * "nets"); */

    //
    const double scale = 1.0 / 2.54;

    wire_bounds.x2 += 2.54;
    wire_bounds.y2 += 2.54;
    // rect_outset(&bounds, 1.27);

    rect_mult(&wire_bounds, scale);
    rect_round(&wire_bounds, 0.1);

    // print_rect(buffer_2, "Wire",
    // &wire_bounds);

    buffer_puts(buffer_1, "--- WIRE BOUNDS ---\n");
    buffer_puts(buffer_1, "\nLayer Measures;\n");
    print_bounds(buffer_1, wire_bounds, ";\n");
    buffer_putsflush(buffer_1, ";\n\n");
    /*  print_wire(buffer_1,
     line(vec(99, 99), vec(33, 33)));
     print_wire(buffer_1, line(c, d));
     print_wire(buffer_1, line(d, a));
  */

    rect_outset(&bounds, 2.54);
    rect_mult(&bounds, 1 / 2.54);
    rect_round(&bounds, 0.1);

    print_rect(buffer_2, "bounds", &bounds);

    rect_mult(&extent2, 1 / 2.54);
    rect_round(&extent2, 0.05);

    buffer_puts(buffer_1, "--- ELEMENT BOUNDS ---\n");
    buffer_puts(buffer_1, "\nLayer Measures;\n");
    print_bounds(buffer_1, extent2, ";\n");
    buffer_putsflush(buffer_1, ";\n\n");
    // print_bounds(buffer_1, bounds);

    /*
     * Cleanup function for the XML
     * library.
     */
    xml_free(doc);
  }
  /*
   * this is to debug memory for
   * regression tests
   */
  return (0);
}
