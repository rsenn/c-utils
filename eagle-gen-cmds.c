#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
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
#include "lib/iterator.h"

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

#define NODE_NAME(n) ((char*)(n)->name)
#define NODE_IS_ELEMENT(n) (((xmlNode*)(n))->type == 1)
#define NODE_ATTRIBUTES(n) (void*)(NODE_IS_ELEMENT(n) ? ((xmlElement*)(n))->attributes : NULL)
#define NODE_CONTENT(n) ((char*)xmlNodeGetContent(n))
#define NODE_CHILDREN(n) ((xmlNode*)(n))->children
//#define NODE_ATTRIBUTES(n) ((xmlElement*)(n))->attributes
#define NODE_PROPERTY(n, p) ((char*)xmlGetProp(((xmlNode*)(n)), ((xmlChar*)(p))))

struct pad {
  stralloc name;
  double x, y;
};

struct package {
  stralloc name;
  array pads; /**< list of struct pad */
};

// struct pinlist {
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
  struct package* pkg;
  struct device* dev;
  struct deviceset* dset;
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
print_attrs(xmlNode* a_node);
void
print_element_attrs(xmlNode* a_node);

static cbmap_t devicesets, packages, parts, nets, symbols;
/**
 * Reads a real-number value from the element/attribute given
 */
double
get_double(xmlNode* node, const char* key) {
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
get_int(xmlNode* node, const char* key) {
  long ret = INT_MIN;
  const char* istr = NODE_PROPERTY(node, key);

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
    if(node->name == NULL)
      continue;
    if(str_equal(NODE_NAME(node), parent))
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
    if(!str_diff(NODE_NAME(node), name))
      return node;
  }
  return NULL;
}

/**
 * Gets a cbmap_t element.
 */
void*
get(cbmap_t m, char* name, size_t datasz) {
  void* ptr;
  size_t len;

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
    char* data = alloca(datasz);
    byte_zero(data, datasz);

    if(cbmap_insert(m, name, str_len(name) + 1, data, datasz))
      ptr = get(m, name, datasz);
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
get_entry(cbmap_t map, const char* key) {
  size_t len = str_len(key) + 1;
  void* ret = NULL;
  cbmap_get(map, (void*)key, len, &ret, &len);
  return ret;
}

/**
 * Outputs name/value pair
 */
void
print_name_value(buffer* b, const char* name, const char* value) {
  buffer_putm(b, name, ": ", value ? value : "(null)", NULL);
}

/**
 * Build structures from <part> or <element> element
 */
void
build_part(xmlNode* part) {
  char* name = NODE_PROPERTY(part, "name");
  char* pkgname = NODE_PROPERTY(part, "package");

  if(!name || str_len(name) == 0)
    return;

  if(!pkgname || str_len(pkgname) == 0)
    return;
  //  put_name_value(buffer_2, "part", name);
  struct part p;
  byte_zero(&p, sizeof(struct part));
  stralloc_copys(&p.name, name);
  char* val = NODE_PROPERTY(part, "value");

  if(val)
    stralloc_copys(&p.value, val);
  p.x = get_double(part, "x") / 0.127;
  p.y = get_double(part, "y") / 0.127;

  p.pkg = get_entry(packages, pkgname);
  //  buffer_puts(buffer_2, " (0x");
  //  buffer_putxlong(buffer_2, (unsigned long)p.pkg);
  //  buffer_puts(buffer_2, ")");
  //  buffer_putnlflush(buffer_2);
  char* dsname = NODE_PROPERTY(part, "deviceset");

  if(dsname)
    p.dset = get_entry(devicesets, dsname);


  cbmap_insert(parts, (void*)name, str_len(name) + 1, &p, sizeof(struct part));
}

/**
 * Build structures from <symbol> element
 */
void
build_sym(xmlNode* part) {
  char* name = NODE_PROPERTY(part, "name");

  if(!name || str_len(name) == 0)
    return;
  struct symbol* sym = get_or_create(symbols, name, sizeof(struct symbol));
  stralloc_copys(&sym->name, name);
  size_t i = 0;

  for(xmlNode* pin = part->children; pin; pin = pin->next) {
    if(pin->type != XML_ELEMENT_NODE)
      continue;
    if(str_diff(NODE_NAME(pin), "pin"))
      continue;
    char* pin_name = NODE_PROPERTY(pin, "name");
    if(pin_name == NULL)
      continue;
    struct pin* p = array_allocate(&sym->pins, sizeof(struct ref), i++);
    byte_zero(p, sizeof(struct ref));
    stralloc_copys(&p->name, pin_name);
    p->x = get_double(pin, "x");
    p->y = get_double(pin, "y");
    p->r = (double)get_int(pin, "rot") * M_PI / 180;
    p->visible = str_diff(NODE_PROPERTY(pin, "visible"), "off");
    //    put_name_value(buffer_2, "pin.name", pin_name);
    //    put_name_value(buffer_2, "pin.x", NODE_PROPERTY(pin, "x"));
    //    put_name_value(buffer_2, "pin.y", NODE_PROPERTY(pin, "y"));
    //    put_name_value(buffer_2, "pin.visible", NODE_PROPERTY(pin, "visible"));
  }
}

/**
 * @param node   Parent is the 'net' or 'signal' element
 */
void
build_reflist(xmlNode* node,  struct net* n, int* index) {

  for(; node; node = node->next) {
    if(!NODE_IS_ELEMENT(node))
      continue;

    char* nn = NODE_NAME(node);

    if(str_equal(nn, "segment")) {
      build_reflist(node->children, n, index);
      continue;
    }

    bool is_pin = str_equal(nn, "pinref");

    if(str_diff(nn, is_pin ? "pinref" : "contactref"))
      continue;

    char* part_name = NODE_PROPERTY(node, is_pin ? "part" : "element");

    struct ref* r = array_allocate(&n->contacts, sizeof(struct ref), (*index)++);

    r->part = get(parts, part_name, sizeof(struct part));


//    stralloc_copys(&r->part->name, part_name);

    print_name_value(buffer_1, nn, part_name);
    buffer_putc(buffer_1, '\t');
    print_element_attrs(node);
    buffer_putnlflush(buffer_1);
  }

}

/**
 * Build reference list from
 *
 *      <contactref element="IC1" pad="4"/>
 *
 *      <pinref part="IC1" gate="B" pin="O"/>
 */
void
build_nets(xmlNode* node) {
  xmlNode* sn;
  char *sign, *name = NODE_NAME(node);

  assert(str_equal(name, "net") || str_equal(name, "signal"));

  if(!(sign = NODE_PROPERTY(node, "name")))
    return;

  print_name_value(buffer_2, name, sign);
  buffer_putnlflush(buffer_2);

  struct net* ptr = get_or_create(nets, sign, sizeof(struct net));

  stralloc_copys(&ptr->name, sign);

  int i = 0;
  build_reflist(node->children, ptr, &i);

}

/**
 * Build structures from <package> element
 */
void
build_package(xmlNode* set) {
  char* name = NODE_PROPERTY(set, "name");
  // put_name_value(buffer_2, "package: ", name);
  struct package pkg;

  byte_zero(&pkg, sizeof(struct package));
  stralloc_copys(&pkg.name, name);

  for(xmlNode* node = set->children; node; node = node->next) {
    if(node->type != XML_ELEMENT_NODE || str_diff(NODE_NAME(node), "pad"))
      continue;
    char* pn = NODE_PROPERTY(node, "name");
    if(!pn || str_len(pn) == 0)
      continue;
    struct pad p;
    byte_zero(&p, sizeof(struct pad));
    p.x = get_double(node, "x");
    p.y = get_double(node, "y");
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
  char* name = NODE_PROPERTY(set, "name");
  print_name_value(buffer_2, "deviceset", name);
  buffer_putnlflush(buffer_2);

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
    stralloc_copys(&g.name, NODE_PROPERTY(node, "name"));
    stralloc_copys(&g.symbol, NODE_PROPERTY(node, "symbol"));
    array_catb(&d.gates, (const void*)&g, sizeof(struct gate));
  }

  for(xmlNode* node = devices->children; node; node = node->next) {
    if(node->type != XML_ELEMENT_NODE)
      continue;
    struct pinmapping pm;
    char* name = NODE_PROPERTY(node, "name");
    if(!name || str_len(name) == 0)
      continue;
    char* package = NODE_PROPERTY(node, "package");
    byte_zero(&pm, sizeof(struct pinmapping));
    struct package* pkg;
    size_t len;
    cbmap_get(packages, package, str_len(package), (void**)&pkg, &len);
    pm.pkg = pkg;
    cbmap_insert(d.devices, name, str_len(name) + 1, &pm, sizeof(struct pinmapping));
  }
  cbmap_insert(devicesets, name, str_len(name) + 1, &d, sizeof(struct deviceset));
}

/**
 * Run an XPath query and return a XPath object
 */
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
  buffer_putm(buffer_2, "xpath: ", xpath, ", num nodes: ", NULL);
  buffer_putulong(buffer_2, result && result->nodesetval ? xmlXPathNodeSetGetLength(result->nodesetval) : 0);
  buffer_putnlflush(buffer_2);
  return result;
}

/**
 * Retrieve all <part> (schematic) or <element> (board) objects
 */
strlist
getparts(xmlDoc* doc) {
  strlist ret;
  strlist_init(&ret);
  xmlXPathObject* nodes = getnodeset(doc, "//part | //element");

  if(!nodes || !nodes->nodesetval)
    return ret;

  for(int i = 0; i < xmlXPathNodeSetGetLength(nodes->nodesetval); ++i) {
    xmlNode* node = xmlXPathNodeSetItem(nodes->nodesetval, i);
    strlist_push(&ret, NODE_PROPERTY(node, "name"));
  }
  return ret;
}

/**
 * Iterate through a node-set, calling a functor for every item
 */
void
for_set(xmlNodeSet* set, void (*fn)(xmlNode*)) {

  if(!set)
    return;

  for(int i = 0; i < xmlXPathNodeSetGetLength(set); ++i) {
    xmlNode* node = xmlXPathNodeSetItem(set, i);
    fn(node);
  }
}

/**
 * Get the top-leftmost x and y coordinate from a set of nodes.
 */
void
nodeset_topleft(xmlNodeSet* s, double* x, double* y) {
  int len = xmlXPathNodeSetGetLength(s);

  if(len == 0)
    return;

  xmlNode* node = xmlXPathNodeSetItem(s, 0);
  *x = get_double(node, "x");
  *y = get_double(node, "y");

  for(int i = 1; i < len; ++i) {
    node = xmlXPathNodeSetItem(s, i);
    double nx = get_double(node, "x");
    double ny = get_double(node, "y");
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

  while(node && node->type != XML_ELEMENT_NODE && str_diff(NODE_NAME(node), elems))
    node = node->next;
  *x = get_double(node, "x");
  *y = get_double(node, "y");

  while((node = node->next)) {
    if(node->type != XML_ELEMENT_NODE || str_diff(NODE_NAME(node), elems))
      continue;
    double nx = get_double(node, "x");
    double ny = get_double(node, "y");
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
  char* name = str_dup(NODE_PROPERTY(elem, "name"));
  str_lower(name);
  buffer_puts(buffer_1, name);
  buffer_puts(buffer_1, "\t");

  for(xmlNode* node = elem->children; node; node = node->next) {
    //  xmlXPathObject* set = getnodeset(elem, "./pad");
    if(!str_diff(NODE_NAME(node), "pad")) {
      double xpos = round((get_double(node, "x") - dx) / 0.254) * 0.1;
      double ypos = round((get_double(node, "y") - dy) / 0.254) * 0.1;
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
dump_part(struct part* ptr) {
  assert(ptr->name.s);
  buffer_putsa(buffer_1, &ptr->name);
  buffer_putspace(buffer_1);
}

void
dump_net(struct net* n) {

  buffer* b = buffer_1;

  buffer_puts(b, "net['");
  buffer_putsa(b, &n->name);
  buffer_puts(b, "']:");

  int64 i, len = array_length(&n->contacts, sizeof(struct ref));

  for(i = 0; i < len; ++i) {
    struct ref* r = array_get(&n->contacts, sizeof(struct ref), i);

    buffer_putspace(b);
    buffer_putsa(b, &r->part->name);
    buffer_putc(b, '.');
    buffer_putulong(b, r->pin);
  }

  buffer_putnlflush(b);
}



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
 *  print element path/name
 */
void
print_element_name(xmlNode* a_node) {
  char* name = NODE_NAME(a_node);

  if(a_node->parent) {
    xmlNode* p = a_node->parent;
    const char* pn = NODE_NAME(p);
    if(pn && !str_diffn(pn, name, str_len(name))) {
      p = p->parent;
    }
    print_element_name(p);
  }

  if(!(name = NODE_NAME(a_node)))
    return;

  if(str_diff(name, "eagle") && str_diff(name, "drawing")) {

    buffer_putm(buffer_1, a_node->parent ? "/" : "", name, NULL);

    if(!(name = NODE_PROPERTY(a_node, "name")))
      return;

    if(str_len(name))
      buffer_putm(buffer_1, "[@name='", name, "']", NULL);
  }
}

/**
 *  print_element_attrs: Prints all element attributes to stdout
 */
void
print_attrs(xmlNode* a) {

  //  if(!NODE_IS_ELEMENT(a_node))
  //    return;

  for(; a; a = a->next) {
    char* v = NODE_CONTENT(a);
    buffer_putm(buffer_1, " ", NODE_NAME(a), str_isdoublenum(v) ? "=" : "=\"", v, str_isdoublenum(v) ? "" : "\"", NULL);
  }
}

void
print_element_attrs(xmlNode* a_node) {

  if(NODE_IS_ELEMENT(a_node)) {
    xmlElement* e = (xmlElement*)a_node;
    print_attrs(NODE_ATTRIBUTES(e));
  }
}

void
print_element_content(xmlNode* node) {
  char* s;

  if((s = NODE_CONTENT(node))) {
    if(str_isspace(s))
      s = "";

    if(str_len(s))
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
    if(!NODE_IS_ELEMENT(node))
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
    case HMAP_DATA_TYPE_CHARS: {
      buffer_putm(buffer_1, " ", tuple->key, "=", tuple->data, NULL);
      buffer_putnlflush(buffer_1);
      break;
    }
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
element_to_hmap(xmlElement* elm) {
  HMAP_DB* hash;
  hmap_init(1024, &hash);

  for(xmlNode* ptr = NODE_ATTRIBUTES(elm); ptr; ptr = ptr->next) {
    char* name = NODE_NAME(ptr);
    if(name && str_len(name)) {
      char* content = NODE_PROPERTY(elm, name);
      if(content && str_len(content) && !str_isspace(content)) {
        if(str_isfloat(content)) {
          hmap_add(&hash, name, str_len(name), 1, HMAP_DATA_TYPE_DOUBLE, strtod(content, NULL) / 2.54);
        } else {
          size_t i, len = str_len(content);
          char *in = content, *dest = malloc(len * 4 + 1);
          for(i = 0; *in; ++in) {
            i += fmt_escapecharquotedprintableutf8(&dest[i], *in);
          }
          hmap_add(&hash, name, str_len(name), 0, HMAP_DATA_TYPE_CHARS, dest, i);
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
print_element_names(xmlNode* node) {

  for(; node; node = node->next) {
    if(!NODE_IS_ELEMENT(node))
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

int
buffer_read(void* ptr, char* buf, int len) {
  return buffer_get(ptr, buf, len);
}

/**
 * Parse the in memory document and free the resulting tree
 */
xmlDoc*
read_xml_tree(const char* filename, buffer* in) {
  xmlDoc* doc; /* the resulting document tree */

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
  print_name_value(buffer_1, "XPath query", q);
  buffer_putnlflush(buffer_1);
  xmlXPathObject* nodes = getnodeset(doc, q);

  if(!nodes || !nodes->nodesetval)
    return;

  for(int i = 0; i < xmlXPathNodeSetGetLength(nodes->nodesetval); ++i) {
    xmlNode* node = xmlXPathNodeSetItem(nodes->nodesetval, i);
    print_element_name(node);
    print_element_attrs(node);
    buffer_putnlflush(buffer_1);
    if(!str_diff(NODE_NAME(node), "package")) {
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
      for(xmlAttr* a = NODE_ATTRIBUTES(node); a; a = a->next) {
        const char* attr_name = NODE_NAME(a);
        const char* v = NODE_PROPERTY(node, attr_name);
        if(!v || str_len(v) == 0)
          continue;
        if(!str_diff(attr_name, "name")) {
          elem_name = "*";
          attr_name = NODE_NAME(node);
        } else {
          elem_name = attr_name;
          attr_name = "name";
          //          attr_name = "*";
        }
        stralloc_copym(&query, "//", elem_name, "[@", attr_name, "='", v, "']", NULL);
        stralloc_0(&query);

        xpath_query(doc, query.s);
        strlist part_names = getparts(doc);
        strlist_dump(buffer_1, &part_names);
      }
    }
  }
}

/**
 * Executes XPath query and for every resulting element calls a function
 */
bool
xpath_foreach(xmlDoc* doc, const char* q, void (*fn)(xmlNode*)) {
  xmlXPathObject* xpo = getnodeset(doc, q);

  if(xpo && &xpo->nodesetval) {
    for_set(xpo->nodesetval, fn);
    return true;
  }
  return false;
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
    argv[1] = "/home/roman/Sources/an-tronics/eagle/40106-4069-Synth.brd";
  } else if(argv[2]) {
    xq = argv[2];
  }
  //  size_t mapsz;
  //  void* ptr = mmap_private(argv[1], &mapsz);
  buffer input;
  buffer_mmapprivate(&input, argv[1]);
  buffer_skip_until(&input, "\r\n", 2);
  xmlDoc* doc = read_xml_tree(argv[1], &input);

  xpath_query(doc, xq);

  xpath_foreach(doc, "//package", build_package);
  xpath_foreach(doc, "//deviceset", build_deviceset);
  xpath_foreach(doc, "//part | //element", build_part);

  cbmap_visit_all(parts, dump_part, "part");

  xpath_foreach(doc, "//net | //signal", build_nets);


  cbmap_visit_all(nets, dump_net, "nets");


  xpath_foreach(doc, "//symbol", build_sym);

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
