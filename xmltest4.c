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

#include "lib/array.h"
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

#ifdef _MSC_VER
#define alloca _alloca
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

const char* document = "<doc/>";
const char* xq = "net";
void node_print(xmlnode* node);
int node_depth(xmlnode* node);
int str_ischarset(const char* s, const char* set);
int str_isfloat(const char* s);
int str_isspace(const char* s);
void print_attrs(xmlnode* a_node);
void print_element_attrs(xmlnode* a_node);

/**
 * Reads a real-number v from the element/attribute given
 */
double
get_double(xmlnode* node, const char* k) {
  double ret = 0.0;
  const char* dstr = NULL;

  if(xml_has_attribute(node, (char*)k)) {
    dstr = (const char*)xml_get_attribute(node, (char*)k);
    if(scan_double(dstr, &ret) <= 0) ret = DBL_MAX;
  }
  return ret;
}

/**
 * Reads an integer number v from the element/attribute given
 */
int
get_int(xmlnode* node, const char* k) {
  long ret = INT_MIN;
  const char* istr = xml_get_attribute(node, k);

  if(istr) {
    while(*istr && str_chr("-0123456789", *istr) >= 11) ++istr;
    if(!scan_long(istr, &ret)) ret = INT_MAX;
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
    if(node->name == NULL) continue;
    if(str_equal(node->name, parent)) break;
  }
  return (xmlnode*)node;
}

/**
 * Gets a child element by name.
 */
xmlnode*
get_child(xmlnode* node, const char* name) {

  for(node = node->children; node; node = node->next) {
    if(!str_diff(node->name, name)) return node;
  }
  return NULL;
}

/**
 * Gets a cbmap_t element.
 */
void*

get(cbmap_t m, char* name, size_t datasz) {
  void* ptr;
  size_t len = datasz;
  if(!cbmap_get(m, name, str_len(name) + 1, &ptr, &len)) ptr = NULL;
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
    if(cbmap_insert(m, name, str_len(name) + 1, data, datasz)) ptr = get(m, name, datasz);
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
get_entry(cbmap_t map, const char* k) {
  size_t len = str_len(k) + 1;
  void* ret = NULL;
  cbmap_get(map, (void*)k, len, &ret, &len);
  return ret;
}

/**
 * Outputs name/v pair
 */
void
print_name_value(buffer* b, const char* name, const char* v) {
  buffer_putm_3(b, name, ": ", v ? v : "(null)");
}

/**
 * Iterate through a node-set, calling a functor for every item
 */
void
for_set(xmlnodeset* ns, void (*fn)(xmlnode*)) {
  xmlnodeset_iter_t it, e;
  if(!ns) return;
  for(it = xmlnodeset_begin(ns), e = xmlnodeset_end(ns); it != e; ++it) fn(*it);
}

/**
 *  node_print: Prints XML node
 */
void
node_print(xmlnode* node) {
  buffer_putm_2(buffer_1, "<", node->name);
  print_element_attrs(node);
  buffer_puts(buffer_1, ">");
  buffer_putnlflush(buffer_1);
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
    if(set[str_chr(set, *s)] == '\0') return 0;
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
  if(!(name = a_node->name)) return;

  if(str_diff(name, "eagle") && str_diff(name, "drawing")) {
    buffer_putm_2(buffer_1, a_node->parent ? "/" : "", name);
    if(!(name = xml_get_attribute(a_node, "name"))) return;
    if(str_len(name)) buffer_putm_3(buffer_1, "[@name='", name, "']");
  }
}

/**
 *  print_element_attrs: Prints all element attributes to stdout
 */
void
print_attrs(xmlnode* node) {
  TUPLE* a;

  for(a = xml_attributes(node); a; a = a->next) {
    char* v = a->vals.val_chars;
    const char* quot = str_isdoublenum(v) ? "" : "\"";
    buffer_putm_6(buffer_1, " ", a->key_len, "=", quot, v, quot);
  }
}

void
print_element_attrs(xmlnode* a_node) {

  if(a_node->type == XML_ELEMENT) {
    xmlnode* e = (xmlnode*)a_node;
    print_attrs(e);
  }
}

void
print_element_content(xmlnode* node) {
  char* s;

  if((s = node->name)) {
    if(str_isspace(s)) s = "";

    if(str_len(s)) {
      stralloc sa;
      stralloc_init(&sa);
      xml_escape(s, str_len(s), &sa);
      /* stralloc_fmt(&sa, s, str_len(s), fmt_escapecharxml); */
      buffer_puts(buffer_1, " \"");
      buffer_putsa(buffer_1, &sa);
      buffer_puts(buffer_1, "\"");
      stralloc_free(&sa);
    }
  }
}

/**
 *  print_element_children: Prints all element attributes to stdout
 */
void
print_element_children(xmlnode* a_node) {
  xmlnode* node;
  if(!a_node->children) return;

  for(node = a_node->children; node; node = node->next) {
    if(node->type != XML_ELEMENT) continue;
    print_element_name(node);
    print_element_attrs(node);
    print_element_content(node);
    buffer_putnlflush(buffer_1);
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

  for(; node; node = node->next) {
    if(node->type != XML_ELEMENT) continue;
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
xmlnode*
read_xml_tree(const char* filename, buffer* in) {
  xmlnode* doc; /* the resulting document tree */

  if((doc = xml_read_tree(in)) == NULL) {
    buffer_puts(buffer_2, "Failed to parse document");
    buffer_putnlflush(buffer_2);
    return NULL;
  }
  return doc;
}

void
xml_query(xmlnode* doc, const char* elem_name, const char* name) {
  size_t i, n;
  xml_predicate_fn* pred;
  xmlnodeset xr;
  buffer_putm_2(buffer_1, "XML query (element=", elem_name);
  if(name) buffer_putm_2(buffer_1, ", name=", name);
  buffer_puts(buffer_1, ")");
  buffer_putnlflush(buffer_1);
  pred = name ? (void*)xml_match_name_and_attr : (void*)xml_match_name;
  xr = xml_find_all_3(doc, pred, elem_name, "name", name);
  if((n = xmlnodeset_size(&xr)) == 0) return;

  for(i = 0; i < n; ++i) {
    xmlnode* node = xmlnodeset_item(&xr, i);
    print_element_name(node);
    print_element_attrs(node);
    buffer_putnlflush(buffer_1);
    print_element_children(node);
    buffer_putnlflush(buffer_1);

    if(0) {
      TUPLE* a;
      elem_name = "*";

      for(a = xml_attributes(node); a; a = a->next) {
        const char *v, *attr_name = a->key;
        if(!str_equal(attr_name, "name")) continue;
        v = xml_get_attribute(node, attr_name);
        if(!v || str_len(v) == 0) continue;
        xml_query(doc, elem_name, v);
      }
    }
  }
}

/**
 * Executes XPath query and for every resulting element calls a function
 */
int
xml_foreach(xmlnode* doc, const char* elem, void (*fn)(xmlnode*)) {
  xmlnodeset xpo = xml_find_all_1(doc, xml_match_name, elem);

  if(!xmlnodeset_empty(&xpo)) {
    for_set(&xpo, fn);
    return 1;
  }
  return 0;
}

int
main(int argc, char* argv[]) {
  buffer input;
  xmlnode* doc;
  static xmlnodeset ns;
  xmlnodeset_iter_t it, e;
  size_t i = 0;

  if(!argv[1]) {
    argv[1] = "mediathek-list.cbp";
  } else if(argv[2]) {
    xq = argv[2];
  }
  buffer_mmapprivate(&input, argv[1]);
  buffer_skip_until(&input, "\r\n", 2);
  doc = xml_read_tree(&input);
  xml_print(doc->children, buffer_1);

  for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
    xmlnode* n = *it;
    buffer_puts(buffer_1, "NODESET[");
    buffer_putlong(buffer_1, i++);
    buffer_puts(buffer_1, "]: ");
    xml_debug(n, buffer_1);
    buffer_putnlflush(buffer_1);
  }
  ns = xml_find_all_1(doc, xml_match_name, "Unit");
  xml_print_nodeset(&ns, buffer_1);
  buffer_putlong(buffer_1, xmlnodeset_size(&ns));
  buffer_putnlflush(buffer_1);
  /*
   * Cleanup function for the XML library.
   */
  xml_free(doc);
  return (0);
}
