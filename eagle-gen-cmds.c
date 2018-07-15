#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#if !defined(_WIN32) && !(defined(__MSYS__) && __MSYS__ == 1)
#include <libgen.h>
#endif
#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/cb.h"
#include "lib/cbmap.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include <libxml/SAX.h>
/**
 * section: Parsing
 * synopsis: Parse an XML document in memory to a tree and free it
 * purpose: Demonstrate the use of xmlReadMemory() to read an XML file
 *          into a tree and and xmlFreeDoc() to free the resulting tree
 * usage: parse3
 * test: parse3
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <stdio.h>

#define node_name(n) ((char*)(n)->name)
#define node_prop(n, p) ((char*)xmlGetProp((xmlNode*)(n), (xmlChar*)p))
#define node_is_elem(n) (((xmlNode*)(n))->type == 1)
#define node_attrs(n) (void*)(node_is_elem(n) ? ((xmlElement*)(n))->attributes : NULL)

struct pad {
  stralloc name;
  double x, y;
};

struct package {
  stralloc name;
  array pads; /**< list of struct pad */
};

//struct pinlist {
//  stralloc name;
//  array pins;
//};
struct pin {
  stralloc name;
  double x, y, r;
  bool visible;
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
  cbmap_t devices; /**< map of struct pinmapping */
};

struct part {
  stralloc name;
  stralloc value;
  double x, y;
  struct package* pkg;
  struct deviceset* dset;
};

struct ref {
  struct part* part;
  int pad;
};

struct net {
  stralloc name;
  array contacts; /**<  list of struct ref */
};

static cbmap_t devicesets, packages, parts, nets, symbols;

/**
 * Reads a real-number value from the element/attribute given
 */
double
getdouble(xmlNode* node, const char* key) {
  double ret = 0.0;
  const char* dstr = NULL;
  if(xmlHasProp(node, (xmlChar*)key)) {

    dstr = (const char*)xmlGetProp(node, (xmlChar*)key);
    if(scan_double(dstr, &ret) <= 0)
      ret = DBL_MAX;
  }
  return ret;
}

/**
 * Reads an integer number value from the element/attribute given
 */
int
getint(xmlNode* node, const char* key) {
  long ret = INT_MIN;
  const char* istr =  node_prop(node, key);
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
xmlElement*
get_parent(void* n, const char* parent) {
  xmlNode* node = n;

  for(node = n; node; node = node->parent) {
    if(node->name == NULL) continue;
    if(str_equal(node_name(node), parent))
      break;
  }
  return (xmlElement*)node;
}

/**
 * Gets a child element by name.
 */
xmlNode*
get_child(xmlNode* node, const char* name) {
  for(node = node->children; node; node = node->next) {
    if(!str_diff(node_name(node), name))
      return node;
  }
  return NULL;
}

/**
 * Gets or creates a cbmap_t element.
 */
void*
get_or_create(cbmap_t m, const char* name, size_t datasz) {
  void* ptr = NULL;
  size_t len = datasz;
  size_t klen = str_len(name) + 1;
  if(!cbmap_get(m, name, klen, &ptr, &len)) {
    char* data = alloca(datasz);
    byte_zero(data, datasz);
    if(!cbmap_insert(m, name, klen, data, datasz))
      return NULL;
    cbmap_get(m, name, klen, &ptr, &len);
  }
  return ptr;
}

// struct package*
// get_package(const char* name) {
//  struct package* ret = NULL;
//  size_t len;
//  cbmap_get(packages, (void*)name, str_len(name)+1, (void**)&ret, &len);
//  return ret;
//}

/**
 * Index a cbmap
 */
void*
at(cbmap_t map, const char* key) {
  size_t len = str_len(key) + 1;
  void* ret = NULL;

  cbmap_get(map, (void*)key, len, &ret, &len);
  return ret;
}

/**
 * Outputs name/value pair
 */
void
put_name_value(buffer* b, const char* name, const char* value) {
  buffer_putm(b, name, ": ", value ? value : "(null)", NULL);
  buffer_putnlflush(b);
}

/**
 * Build structures from <part> or <element> element
 */
void
build_part(xmlNode* part) {
  char* name = node_prop(part, "name");
  char* pkgname = node_prop(part, "package");

  if(!name || str_len(name) == 0)
    return;
  if(!pkgname || str_len(pkgname) == 0)
    return;

  //  put_name_value(buffer_2, "part", name);

  struct part p;
  byte_zero(&p, sizeof(struct part));

  stralloc_copys(&p.name, name);

  char* val = node_prop(part, "value");
  if(val)
    stralloc_copys(&p.value, val);

  p.x = getdouble(part, "x") / 0.127;
  p.y = getdouble(part, "y") / 0.127;

  p.pkg = at(packages, pkgname);

  //  buffer_puts(buffer_2, " (0x");
  //  buffer_putxlong(buffer_2, (unsigned long)p.pkg);
  //  buffer_puts(buffer_2, ")");

  //  buffer_putnlflush(buffer_2);

  char* dsname = node_prop(part, "deviceset");

  if(dsname)
    p.dset = at(devicesets, dsname);

  cbmap_insert(parts, (void*)name, str_len(name) + 1, &p, sizeof(struct part));
}


/**
 * Build structures from <symbol> element
 */
void
build_sym(xmlNode* part) {
  char* name = node_prop(part, "name");

  if(!name || str_len(name) == 0)
    return;

  struct symbol *sym = get_or_create(symbols, name, sizeof(struct symbol));

  stralloc_copys(&sym->name, name);

  size_t i = 0;

  for(xmlNode* pin = part->children; pin; pin = pin->next) {
    if(pin->type != XML_ELEMENT_NODE) continue;
    if(str_diff(node_name(pin), "pin")) continue;
    char* pin_name = node_prop(pin, "name");
    if(pin_name == NULL) continue;

    struct pin* p = array_allocate(&sym->pins, sizeof(struct ref), i++);

    stralloc_copys(&p->name, pin_name);
    p->x = getdouble(pin, "x");
    p->y = getdouble(pin, "y");
    p->r = (double)getint(pin, "rot") * M_PI / 180;
    p->visible = str_diff(node_prop(pin, "visible"), "off");


//    put_name_value(buffer_2, "pin.name", pin_name);
//    put_name_value(buffer_2, "pin.x", node_prop(pin, "x"));
//    put_name_value(buffer_2, "pin.y", node_prop(pin, "y"));
//    put_name_value(buffer_2, "pin.visible", node_prop(pin, "visible"));


  }
}

/**
 * Build reference list from <contactref> or <pinref>
 */
void
build_ref(xmlNode* ref) {
  xmlNode* sn;

  if(!(sn = get_parent(ref, "signal")))
    sn = get_parent(ref, "net");

  char* sign = node_prop(sn, "name");

  if(sign == NULL) return;

  put_name_value(buffer_2, "signal name:", sign);

  struct net* n = get_or_create(nets, sign, sizeof(struct net));

  //  struct ref r;
  //  byte_zero(&r, sizeof(struct ref));

  //  stralloc_copys(&p.name, name);
}

/**
 * Build structures from <package> element
 */
void
build_package(xmlNode* set) {
  char* name = node_prop(set, "name");

  // put_name_value(buffer_2, "package: ", name);

  struct package pkg;
  byte_zero(&pkg, sizeof(struct package));

  stralloc_copys(&pkg.name, name);

  for(xmlNode* node = set->children; node; node = node->next) {
    if(node->type != XML_ELEMENT_NODE || str_diff(node_name(node), "pad"))
      continue;

    char* pn = node_prop(node, "name");
    if(!pn || str_len(pn) == 0)
      continue;

    struct pad p;

    byte_zero(&p, sizeof(struct pad));

    p.x = getdouble(node, "x");
    p.y = getdouble(node, "y");
    stralloc_copys(&p.name, pn);

    array_catb(&pkg.pads, (const void*)&p, sizeof(struct pad));
  }

  cbmap_insert(packages, name, str_len(name) + 1, &pkg, sizeof(struct package));
}

/**
 * Build structures from <deviceset> element
 */
void
build_deviceset(xmlNode* set) {

  char* name = node_prop(set, "name");
  put_name_value(buffer_2, "deviceset", name);

  struct deviceset d;
  byte_zero(&d, sizeof(struct deviceset));

  stralloc_copys(&d.name, name);

  d.devices = cbmap_new();

  xmlNode* gates = get_child(set, "gates");
  xmlNode* devices = get_child(set, "devices");

  for(xmlNode* node = gates->children; node; node = node->next) {
    if(node->type != XML_ELEMENT_NODE)
      continue;
    struct gate g;
    byte_zero(&g, sizeof(struct gate));
    stralloc_copys(&g.name, node_prop(node, "name"));
    stralloc_copys(&g.symbol, node_prop(node, "symbol"));

    array_catb(&d.gates, (const void*)&g, sizeof(struct gate));
  }
  for(xmlNode* node = devices->children; node; node = node->next) {
    if(node->type != XML_ELEMENT_NODE)
      continue;
    struct pinmapping pm;
    char* name = node_prop(node, "name");
    if(!name || str_len(name) == 0)
      continue;
    char* package = node_prop(node, "package");
    byte_zero(&pm, sizeof(struct pinmapping));

    struct package* pkg;
    size_t len;
    cbmap_get(packages, package, str_len(package), (void**)&pkg, &len);

    pm.pkg = pkg;

    cbmap_insert(d.devices, name, str_len(name) + 1, &pm, sizeof(struct pinmapping));
  }

  cbmap_insert(devicesets, name, str_len(name) + 1, &d, sizeof(struct deviceset));
}

const char* document = "<doc/>";
static const char* xq = "//net";

void
node_print(xmlNode* node);
int
node_depth(xmlNode* node);
int
str_ischarset(const char* s, const char* set);
int
str_isfloat(const char* s);
int
str_isspace(const char* s);
void
print_element_attrs(xmlNode* a_node);

xmlXPathObject*
getnodeset(void* n, const char* xpath) {

  xmlNode* node = n;
  xmlXPathContext* context;
  xmlXPathObject* result;

  context = xmlXPathNewContext(node->doc);

  if(node != (xmlNode*)node->doc)
    xmlXPathSetContextNode(node, context);

  if(context == NULL) {
    buffer_putsflush(buffer_2, "Error in xmlXPathNewContext\n");
    return NULL;
  }
  result = xmlXPathEvalExpression((xmlChar*)xpath, context);
  xmlXPathFreeContext(context);
  if(result == NULL) {
    buffer_putsflush(buffer_2, "Error in xmlXPathEvalExpression\n");
    return NULL;
  }
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
    xmlXPathFreeObject(result);
    buffer_putsflush(buffer_2, "No result\n");
    return NULL;
  }

  buffer_puts(buffer_2, "nodes: ");
  buffer_putulong(buffer_2, result && result->nodesetval ? xmlXPathNodeSetGetLength(result->nodesetval) : 0);
  buffer_putnlflush(buffer_2);

  return result;
}

strlist
getparts(xmlDoc* doc) {
  strlist ret;
  strlist_init(&ret);

  xmlXPathObject* nodes = getnodeset(doc, "//part");
  if(!nodes || !nodes->nodesetval)
    return ret;

  for(int i = 0; i < xmlXPathNodeSetGetLength(nodes->nodesetval); ++i) {
    xmlNode* node = xmlXPathNodeSetItem(nodes->nodesetval, i);

    strlist_push(&ret, node_prop(node, "name"));
  }
  return ret;
}

void
for_set(xmlNodeSet* set, void (*fn)(xmlNode*)) {
  if(!set)
    return;
  for(int i = 0; i < xmlXPathNodeSetGetLength(set); ++i) {
    xmlNode* node = xmlXPathNodeSetItem(set, i);
    fn(node);
  }
}

const char*
x_for_parent(xmlElement* node, const char* parent, const char* attr_name) {

  node = get_parent(node, parent);
  if(node)
    return node_prop(node, attr_name);
  return NULL;
}

xmlElement*
net_for_node(xmlNode* node) {
  return get_parent(node, "net");
}

void
nodeset_topleft(xmlNodeSet* s, double* x, double* y) {
  int len = xmlXPathNodeSetGetLength(s);

  if(len == 0)
    return;

  xmlNode* node = xmlXPathNodeSetItem(s, 0);

  *x = getdouble(node, "x");
  *y = getdouble(node, "y");

  for(int i = 1; i < len; ++i) {

    node = xmlXPathNodeSetItem(s, i);
    double nx = getdouble(node, "x");
    double ny = getdouble(node, "y");
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
tree_topleft(xmlElement* elem, const char* elems, double* x, double* y) {
  xmlNode* node = elem->children;
  if(node == 0)
    return;

  while(node && node->type != XML_ELEMENT_NODE && str_diff(node_name(node), elems))
    node = node->next;

  *x = getdouble(node, "x");
  *y = getdouble(node, "y");

  while((node = node->next)) {

    if(node->type != XML_ELEMENT_NODE || str_diff(node_name(node), elems))
      continue;

    double nx = getdouble(node, "x");
    double ny = getdouble(node, "y");
    if(nx < *x)
      *x = nx;
    if(ny < *y)
      *y = ny;
  }
}

void
dump_package(xmlElement* elem) {
  int i = 0;

  double dx, dy;
  tree_topleft(elem, "pad", &dx, &dy);

  char* name = str_dup(node_prop(elem, "name"));
  str_lower(name);

  buffer_puts(buffer_1, name);
  buffer_puts(buffer_1, "\t");

  for(xmlNode* node = elem->children; node; node = node->next) {

    //  xmlXPathObject* set = getnodeset(elem, "./pad");

    if(!str_diff(node_name(node), "pad")) {

      double xpos = round((getdouble(node, "x") - dx) / 0.254) * 0.1;
      double ypos = round((getdouble(node, "") - dy) / 0.254) * 0.1;

      buffer_putdouble(buffer_1, xpos);
      buffer_puts(buffer_1, ",");
      buffer_putdouble(buffer_1, ypos);
      if(node->next)
        buffer_puts(buffer_1, " ");

      ++i;
    }
  }
  buffer_putnlflush(buffer_1);
}

void
dump_part(xmlElement* elem) {}

/**
 *  node_print: Prints XML node
 */
void
node_print(xmlNode* node) {
  buffer_putm(buffer_1, "<", node->name, NULL);

  print_element_attrs(node);

  buffer_putm(buffer_1, ">", NULL);
  buffer_putnlflush(buffer_1);
}

/**
 *  hashmap_dump: Gets depth of node in hierarchy
 */
int
node_depth(xmlNode* node) {
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
  return (const char*)end > s;
}

int
str_isspace(const char* s) {
  return str_ischarset(s, "\t\r\n\v ");
}

/**
 *  hashmap_dump: Outputs hashmap to stdout
 */
void
print_element_name(xmlNode* a_node) {
  const char* name = node_name(a_node);

  if(a_node->parent) {
    xmlNode* p = a_node->parent;
    ;
    const char* pn = node_name(p);

    if(pn && !str_diffn(pn, name, str_len(name))) {
      p = p->parent;
    }

    print_element_name(p);
  }

  if(node_name(a_node)) {

    if(str_diff(node_name(a_node), "eagle") && str_diff(node_name(a_node), "drawing")) {

      buffer_putm(buffer_1, a_node->parent ? "/" : "", node_name(a_node), NULL);

      if(xmlHasProp(a_node, (xmlChar*)"name")) {
        char* p = (char*)xmlGetProp(a_node, (xmlChar*)"name");
        if(str_len(p))
          buffer_putm(buffer_1, "[@name='", p, "']", NULL);
      }
    }
  }
}

/**
 *  print_element_attrs: Prints all element attributes to stdout
 */
void
print_element_attrs(xmlNode* a_node) {

  if(!node_is_elem(a_node))
    return;

  for(xmlNode* a = node_attrs(a_node); a; a = a->next) {
    const char* v = node_prop(a_node, a->name);
    buffer_putm(buffer_1, " ", node_name(a), str_isdoublenum(v) ? "=" : "=\"", v, str_isdoublenum(v) ? "" : "\"", NULL);
  }
}

void
print_element_content(xmlNode* a_node) {
  xmlChar* s;
  if((s = xmlNodeGetContent(a_node))) {

    if(str_isspace((const char*)s))
      s = (xmlChar*)"";

    buffer_putm(buffer_1, " \"", s, "\"", NULL);
  }
}

/**
 *  print_element_children: Prints all element attributes to stdout
 */
void
print_element_children(xmlNode* a_node) {

  if(!a_node->children)
    return;

  for(xmlNode* node = a_node->children; node; node = node->next) {

    if(!node_is_elem(node))
      continue;

    print_element_name(node);
    print_element_attrs(node);
    print_element_content(node);
    buffer_putnlflush(buffer_1);

    print_element_children(node);
  }
}

/**
 *  hashmap_dump: Outputs hashmap to stdout
 */
void
hashmap_dump(HMAP_DB* db, const char* name) {
  int i = 0;
  TUPLE* tuple = NULL;
  tuple = db->tuple;
  buffer_putm(buffer_1, name, ": ", NULL);
  for(i = 0; i < db->bucket_size; i++) {
    switch(tuple->data_type) {
      case HMAP_DATA_TYPE_CHARS:
        buffer_putm(buffer_1, " ", tuple->key, "=", tuple->data, NULL);
        buffer_putnlflush(buffer_1);
        break;
      case HMAP_DATA_TYPE_DOUBLE: {
        char dbl[100];
        fmt_double(dbl, *(double*)tuple->data, sizeof(dbl), -1);
        buffer_putm(buffer_1, " ", tuple->key, "=", dbl, NULL);
        buffer_putnlflush(buffer_1);
        break;
      }
    }
  }
}

HMAP_DB*
element_to_hashmap(xmlElement* elm) {
  HMAP_DB* hash;
  hmap_init(1024, &hash);
  for(xmlNode* ptr = node_attrs(elm); ptr; ptr = ptr->next) {
    char* name = node_name(ptr);
    if(name && str_len(name)) {
      char* content = node_prop(elm, name);
      if(content && str_len(content) && !str_isspace(content)) {
        if(str_isfloat(content)) {
          hmap_add(&hash, name, str_len(name), 1, HMAP_DATA_TYPE_DOUBLE, strtod(content, NULL) / 2.54);
        } else {
          size_t i, len = str_len(content);
          char *in = content, *dest = malloc(len * 4 + 1);
          for(i = 0; *in; ++in) {
            i += fmt_escapecharquotedprintableutf8(&dest[i], *in);
          }
          //        dest[i] = '\0';
          hmap_add(&hash, name, str_len(name), 0, HMAP_DATA_TYPE_CHARS, dest, i);
          //  free(dest);
        }
      }
    }
  }
  //  hashmap_dump(hash);
  return hash;
}

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
void
print_element_names(xmlNode* cur_node) {
  for(; cur_node; cur_node = cur_node->next) {
    if(node_is_elem(cur_node)) {
      print_element_name(cur_node);

      if(node_depth(cur_node) >= 1) {

        print_element_attrs(cur_node);

        //      HMAP_DB* hmap = element_to_hashmap(elm);
        //      hashmap_dump(hmap, elm->name);
        //      hmap_destroy(&hmap);
        //      for(xmlAttribute* attr = elm->attributes; attr; attr = attr->next) {
        //        xmlChar* prop = xmlGetProp(cur_node, attr->name);
        //        buffer_putm(buffer_1, " ", attr->name, str_isfloat(prop) ? "=" : "=\"", prop, str_isfloat(prop) ? "" : "\"", NULL);
        //      }
      }
      buffer_putnlflush(buffer_1);
      //      printf("node type: Element, name: %s\n", cur_node->name);
    }
    print_element_names(cur_node->children);
  }
}

/**
 * example3Func:
 * @content: the content of the document
 * @length: the length in bytes
 *
 * Parse the in memory document and free the resulting tree
 */

int
buffer_read(void* ptr, char* buf, int len) {
  return buffer_get(ptr, buf, len);
}

xmlDoc*
read_xml_tree(const char* filename, buffer* in) {
  xmlDoc* doc; /* the resulting document tree */
  /*
   * The document being in memory, it have no base per RFC 2396,
   * and the "noname.xml" argument will serve as its base.
   */
  doc = xmlReadFile(filename, "UTF-8", XML_PARSE_RECOVER);

  //  doc = xmlReadIO(buffer_read, (void*)buffer_close, in, XML_XML_NAMESPACE,  "UTF-8", XML_PARSE_RECOVER);

  if(doc == NULL) {
    buffer_puts(buffer_2, "Failed to parse document");
    buffer_putnlflush(buffer_2);
    return NULL;
  }
  return doc;
}
void
xpath_query(xmlDoc* doc, const char* q) {

  buffer_putm(buffer_1, "XPath query: ", q, "\n", NULL);
  buffer_flush(buffer_1);

  xmlXPathObject* nodes = getnodeset(doc, q);

  if(!nodes || !nodes->nodesetval)
    return;

  for(int i = 0; i < xmlXPathNodeSetGetLength(nodes->nodesetval); ++i) {
    xmlNode* node = xmlXPathNodeSetItem(nodes->nodesetval, i);

    print_element_name(node);
    print_element_attrs(node);
    buffer_putnlflush(buffer_1);

    if(!str_diff(node_name(node), "package")) {
      dump_package((xmlElement*)node);
      continue;
    }

    print_element_children(node);
    buffer_putnlflush(buffer_1);

    if(0) { //! str_diff(q, xq)) {
      stralloc query;
      stralloc_init(&query);
      const char* elem_name = &q[2];

      elem_name = "*";

      for(xmlAttr* a = node_attrs(node); a; a = a->next) {
        const char* attr_name = node_name(a);
        const char* v = node_prop(node, attr_name);

        if(!v || str_len(v) == 0)
          continue;

        if(!str_diff(attr_name, "name")) {
          elem_name = "*";
          attr_name = node_name(node);
        } else {
          elem_name = attr_name;
          attr_name = "name";
          //          attr_name = "*";
        }

        stralloc_copym(&query, "//", elem_name, "[@", attr_name, "='", v, "']", NULL);
        stralloc_0(&query);

        //        buffer_putm(buffer_1, " ", node_name(a), str_isdoublenum(v) ? "=" : "=\"", v, str_isdoublenum(v) ? "" : "\"", NULL);

        xpath_query(doc, query.s);

        strlist part_names = getparts(doc);

        strlist_dump(buffer_1, &part_names);
      }

      // stralloc_catm_internal(&query, "//*[@", elem_name, "='", xmlGetProp(node, (xmlChar*)"name"), "']", NULL);
    }

    // title[@lang='en']
  }
}

int
main(int argc, char* argv[]) {

  devicesets = cbmap_new();
  packages = cbmap_new();
  parts = cbmap_new();
  nets = cbmap_new();
  symbols = cbmap_new();

  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION
  if(!argv[1]) {
    argv[1] = "C:/Users/roman/Documents/Sources/an-tronics/eagle/40106-4069-Synth.brd";

  } else if(argv[2]) {
    xq = argv[2];
  }

  //  size_t mapsz;
  //  void* ptr = mmap_private(argv[1], &mapsz);

  buffer input;
  buffer_mmapprivate(&input, argv[1]);
  buffer_skip_until(&input, "\r\n", 2);

  xmlDoc* doc = read_xml_tree(argv[1], &input);
  //  xmlNode* node = xmlDocGetRootElement(doc);
  //  size_t child_count = xmlChildElementCount(node);
  //  print_element_names(node);
  //  xmlNode* child;
  //  for(child = node->children; child; child = child->next) {
  //    if(child->type == XML_ELEMENT_NODE) {
  //      // if(child->type == XML_ELEMENT_TYPE_ANY)
  //      HMAP_DB* db = element_to_hashmap((xmlElement*)child);
  //      hashmap_dump(db, node_name(child));
  //      hmap_destroy(&db);
  //    }
  //    // print_node(child);
  //  }

  xpath_query(doc, xq);

  xmlXPathObject* xpo;

  xpo = getnodeset(doc, "//package");
  for_set(xpo->nodesetval, build_package);

  xpo = getnodeset(doc, "//deviceset");
  if(xpo && xpo->nodesetval)
    for_set(xpo->nodesetval, build_deviceset);

  xpo = getnodeset(doc, "//part");
  if(!xpo || !xpo->nodesetval)
    xpo = getnodeset(doc, "//element");

  for_set(xpo->nodesetval, build_part);

  xpo = getnodeset(doc, "//contactref");
  if(!xpo || !xpo->nodesetval)
    xpo = getnodeset(doc, "//pinref");

  if(xpo && xpo->nodesetval)
    for_set(xpo->nodesetval, build_ref);

  xpo = getnodeset(doc, "//symbol");

  if(xpo && xpo->nodesetval)
    for_set(xpo->nodesetval, build_sym);

  /*
   * Cleanup function for the XML library.
   */
  xmlCleanupParser();
  /*
   * this is to debug memory for regression tests
   */
  //    xmlMemoryDump();
  return (0);
}
