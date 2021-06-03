/**
 * @defgroup   xml
 * @brief      XML module.
 * @{
 */
#ifndef XML_H
#define XML_H

#include <sys/types.h>
#include <assert.h>
#include "buffer.h"
#include "hmap.h"
#include "stralloc.h"
#include "textbuf.h"
#include "uint32.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#define __inl __forceinline
#else
#define __inl static inline
#endif

typedef enum xmltokid {
  XML_EOF = -1,
  XML_DATA = 0,
  XML_TAG_NAME = 1,
  XML_TAG_CLOSE = 2,
  XML_ATTR_NAME = 3,
  XML_ATTR_VALUE = 4,
  XML_COMMENT = 5
} xmltokid;

typedef struct {
  char* x;
  uint32 len;
  xmltokid id;
} xmltoken;

typedef struct xmlscanner {
  buffer* b;
  xmltokid tok;
} xmlscanner;

typedef enum xmlnodeid {
  XML_DOCUMENT = 1,
  XML_ELEMENT = 2,
  XML_ATTRIBUTE = 3,
  XML_TEXT = 4,
} xmlnodeid;

typedef struct xmlnode {
  struct xmlnode* next;
  struct xmlnode* children;
  struct xmlnode* parent;
  enum xmlnodeid type;
  char* name;
  HMAP_DB* attributes;
} xmlnode;

typedef struct xmlreader {
  buffer* b;
  xmlnode* doc;
  xmlnode* parent;
  xmlnode** ptr;
  HMAP_DB* attrmap;
  int closing : 1;
  int self_closing : 1;
  const void* data;
  struct xmlreader* stack;
} xmlreader;

typedef struct xmlnodeset {
  xmlnode** nodes;
  size_t size;
} xmlnodeset;

#define node_is_closing(n) ((n)->type == XML_ELEMENT && (n)->name[0] == '/')

typedef int(xml_read_callback_fn)(xmlreader* r, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs);

typedef int(xml_pred_t)(xmlnode*, const void*, const void*, const void*);
typedef void xml_print_fmt_t(xmlnode*, buffer*, int, char ch, int n);

/*
xmlnode* xml_attrnode(const char*, size_t name_len, const char* value, size_t
value_len); int xml_content_sa(xmlnode*, stralloc* sa); const char*
xml_content(xmlnode*); void xml_debug(xmlnode*, buffer* b); size_t
xml_escape(const char*, size_t n, stralloc* out); xmlnode*
xml_find_element_attr(xmlnode*, const char* tag, const char* attr, const char*
value); xmlnode* xml_find_element(xmlnode*, const char* tag); void
xml_free(xmlnode*); Mchar* xml_get_attribute(xmlnode*, const char* attr);
xmlnode* xml_get_document(xmlnode*);
*/
#ifdef STRLIST_H
xmlnode* xml_find_with_attrs_l(xmlnode* node, const strlist* attrs);
int xml_match_name_and_attr(xmlnode* node, strlist* names, const char* attr, const char* value);
int xml_match_name(xmlnode* node, strlist* names, const char* attr, const char* value);

#endif
void xml_add_child(xmlnode* parent, xmlnode* node);
HMAP_DB* xml_attributes(const char* arg, ...);
xmlnode* xml_attrnode(const char* name, size_t name_len, const char* value, size_t value_len);
xmlnode* xml_child_element_attrs(const char* name, xmlnode* parent, const char* arg, ...);
xmlnode* xml_child_element(const char* name, xmlnode* parent);
xmlnode* xml_child_element_text(const char* name, xmlnode* parent, const char* text);
int xml_content_sa(xmlnode* node, stralloc* sa);
size_t xml_num_children(xmlnode* node);
const char* xml_content(xmlnode* node);
void xml_debug(xmlnode* node, buffer* b);
void xml_delete(xmlnode* node);
xmlnode* xml_element_attrs(const char* name, const char* arg, ...);
xmlnode* xml_element(const char* name);
size_t xml_escape(const char* s, size_t n, stralloc* out);
xmlnodeset xml_find_all_1(xmlnode* node, xml_pred_t*, const void* a1);
xmlnodeset xml_find_all_2(xmlnode* node, xml_pred_t*, const void* a1, const void* a2);
xmlnodeset xml_find_all_3(xmlnode* node, xml_pred_t*, const void* a1, const void* a2, const void* a3);
xmlnodeset xml_find_all_attrs(xmlnode* node, const char* attrs);
xmlnode* xml_find_attr(xmlnode* node, const char* attr);
xmlnode* xml_find_element_attr(xmlnode* node, const char* tag, const char* attr, const char* value);
xmlnode* xml_find_element(xmlnode* node, const char* tag);
xmlnode* xml_find_parent_attr(xmlnode* node, const char* attrs);
xmlnode* xml_find_parent_pred_1(xmlnode* node, xml_pred_t*, const void* arg);
xmlnode* xml_find_parent_pred_2(xmlnode* node, xml_pred_t*, const void* a0, const void* a1);
xmlnode* xml_find_parent_pred_3(xmlnode* node, xml_pred_t*, const void* a0, const void* a1, const void* a2);
xmlnode* xml_find_parent(xmlnode* node, const char* tag);
xmlnode* xml_find_pred_1(xmlnode* node, xml_pred_t*, const void* arg);
xmlnode* xml_find_pred_2(xmlnode* node, xml_pred_t*, const void* a0, const void* a1);
xmlnode* xml_find_pred_3(xmlnode* node, xml_pred_t*, const void* a0, const void* a1, const void* a2);
xmlnode* xml_find_pred_n(xmlnode* node, xml_pred_t*, int n, ...);
xmlnode* xml_find_with_attrs(xmlnode* node, const char* attrs);
void xml_free(xmlnode* node);
double xml_get_attribute_double(xmlnode* node, const char* attr);
long xml_get_attribute_long(xmlnode* node, const char* attr);
int xml_get_attribute_sa(xmlnode* n, stralloc* sa, const char* name);
char* xml_get_attribute(xmlnode* node, const char* attr);
xmlnode* xml_get_document(xmlnode* node);
char* xml_get_text(xmlnode* node, stralloc* out);
int xml_has_attribute_n(xmlnode* node, const char* b, size_t n);
int xml_has_attribute(xmlnode* node, const char* attr);
void xml_insert(xmlnode** nptr, xmlnode* node);
xmlnode* xml_newnode(xmlnodeid type);
int xml_path(const xmlnode* node, stralloc* out);
int xml_path_relative(const xmlnode* node, stralloc* out);
xmlnodeset xml_pfind_all(xmlnode* node, xml_pred_t*, const void* ptr[4]);
xmlnode* xml_pfind_parent(xmlnode* node, xml_pred_t*, const void* ptr[]);
xmlnode* xml_pfind_pred(xmlnode* node, xml_pred_t*, const void* ptr[]);
void xml_print_attributes(HMAP_DB* db, buffer* b, const char* sep, const char* eq, const char* quot);
void xml_print_nodeset(const xmlnodeset* ns, buffer* b);
void xml_print(xmlnode* node, buffer* b, xml_print_fmt_t* fmt);
void xml_read_callback(xmlreader* r, xml_read_callback_fn* fn);
void xml_reader(xmlreader* r, buffer* b);
xmlnode* xml_read_tree(buffer* b);
xmlnode* xml_remove(xmlnode** nptr);
xmlnode* xml_root_element(xmlnode* node);
void xml_set_attribute_double(xmlnode* node, const char* a, double d, int prec);
int xml_set_attributes(xmlnode* node, ...);
void xml_set_attribute(xmlnode* node, const char* a, const char* v);
int xml_tag_pred(xmlnode* node, const void*, const void*, const void*);
xmlnode* xml_textnodeb(const char* text, size_t len);
xmlnode* xml_textnode(const char* s);
void xml_walk(xmlnode* root, int (*func)());

static xmlnode*
xml_add_text(xmlnode* parent, const char* text) {
  xmlnode* n = xml_textnode(text);
  xml_add_child(parent, n);
  return n;
}
static size_t
xml_num_attrs(xmlnode* node) {
  assert(node->type == XML_ELEMENT);
  if(node->attributes == 0)
    return 0;
  return hmap_count(node->attributes);
}

void xml_set_attribute_double(xmlnode*, const char* a, double d, int prec);
void xml_set_attribute(xmlnode*, const char* a, const char* v);
int xml_set_attributes(xmlnode*, ...);
HMAP_DB* xml_attributes(const char* arg, ...);
xmlnode* xml_element(const char*);
xmlnode* xml_child_element(const char*, xmlnode*);

//#define xml_attributes(node) ((node)->attributes ?
//(node)->attributes->list_tuple : NULL)

#define xmlnodeset_item(ns, i) ((ns)->nodes[i])

#define xmlnodeset_clear(ns) byte_zero((ns), sizeof(xmlnodeset))

#define xmlnodeset_foreach(ns, it) for((it) = &(ns)->nodes[0]; (it) != &(ns)->nodes[(ns)->size]; ++(it))

__inl xmlnode**
xmlnodeset_begin(const xmlnodeset* ns) {
  return ns->nodes ? &ns->nodes[0] : NULL;
}
__inl xmlnode**
xmlnodeset_end(const xmlnodeset* ns) {
  return ns->nodes ? &ns->nodes[ns->size] : NULL;
}

__inl size_t
xmlnodeset_size(const xmlnodeset* set) {
  return set->size;
}
#define xmlnodeset_empty(ns) (xmlnodeset_size(ns) == 0)

typedef xmlnode** xmlnodeset_iter_t;

__inl void
xmlnodeset_iter_pp(xmlnode*** itp) {
  ++(*itp);
}
__inl void
xmlnodeset_iter_mm(xmlnode*** itp) {
  --(*itp);
}
__inl xmlnode*
xmlnodeset_iter_ref(xmlnode* const* const itp) {
  return *(itp);
}
__inl int
xmlnodeset_iter_eq(xmlnode*** itp1, xmlnode*** itp2) {
  return (*itp1) == (*itp2);
}
__inl ptrdiff_t
xmlnodeset_iter_dist(xmlnode*** itp1, xmlnode*** itp2) {
  return (*itp2) - (*itp1);
}

#define XML_READ_BUFFER 4096
#define XML_HMAP_BUCKETS 48

typedef int xml_pred_t(xmlnode*, const void*, const void*, const void*);

xmlnode* xml_find_pred(xmlnode*, xml_pred_t*, ...);
xmlnodeset xml_find_all(xmlnode*, xml_pred_t*, ...);
xmlnodeset xml_find_all_1(xmlnode*, xml_pred_t*, const void*);
xmlnodeset xml_find_all_2(xmlnode*, xml_pred_t*, const void*, const void*);
xmlnodeset xml_find_all_3(xmlnode*, xml_pred_t*, const void*, const void*, const void*);

xmlnode* xml_find_pred_1(xmlnode*, xml_pred_t*, const void* arg);
xmlnode* xml_find_pred_2(xmlnode*, xml_pred_t*, const void* a0, const void* a1);
xmlnode* xml_find_pred_3(xmlnode*, xml_pred_t*, const void* a0, const void* a1, const void* a2);

xmlnode* xml_find_element(xmlnode*, const char* tag);
xmlnode* xml_find_pred_n(xmlnode*, xml_pred_t*, int n, ...);

int xml_has_attribute_n(xmlnode*, const char* b, size_t n);
int xml_has_attribute(xmlnode*, const char* attr);

#ifdef STRLIST_H
xmlnode* xml_find_with_attrs_l(xmlnode*, const strlist* attrs);
#endif

xmlnode* xml_find_with_attrs(xmlnode*, const char* attrs);
xmlnodeset xml_find_all_attrs(xmlnode*, const char* attrs);

xmlnode* xml_find_parent_attr(xmlnode*, const char* attrs);
xmlnode* xml_find_parent_pred_1(xmlnode*, xml_pred_t*, const void* arg);
xmlnode* xml_find_parent_pred_2(xmlnode*, xml_pred_t*, const void* a0, const void* a1);
xmlnode* xml_find_parent_pred_3(xmlnode*, xml_pred_t*, const void* a0, const void* a1, const void* a2);
xmlnode* xml_find_parent(xmlnode*, const char* tag);

void xml_walk(xmlnode*, int (*)());
char* xml_get_text(xmlnode* node, stralloc* out);

xmlnode* xml_element_attrs(const char* name, const char* arg, ...);
xmlnode* xml_child_element_attrs(const char* name, xmlnode* parent, const char* arg, ...);
xmlnode* xml_child_element_text(const char* name, xmlnode* parent, const char* text);

size_t xml_scan_comment(const char* x, size_t len);
size_t xml_scan_tag(const char* x, size_t len, size_t* toklen);
size_t xml_scan_attrname(const char* x, size_t len, size_t* toklen);
size_t xml_scan_attrvalue(const char* x, size_t len, size_t* toklen);

void xml_scanner_buf(xmlscanner* s, buffer* b);
void xml_scanner(xmlscanner* s, const char* x, size_t len);
#ifdef STRALLOC_H
void xml_scanner_sa(xmlscanner* s, const stralloc* sa);
#endif
xmltoken xml_read_token(xmlscanner* s);

#ifdef __cplusplus
}
#endif

#endif /* XML_H */
/** @} */
