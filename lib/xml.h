#ifndef LIB_XML_H_
#define LIB_XML_H_

#include <sys/types.h>
#include "buffer.h"
#include "hmap.h"
#include "stralloc.h"
#include "textbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#define __inl __forceinline
#else
#define __inl static inline
#endif

typedef enum xmlnodeid {
  XML_DOCUMENT = 1,
  XML_ELEMENT = 2,
  XML_ATTRIBUTE = 3,
  XML_TEXT = 4,
} xmlnodeid;

typedef struct xmlnode {
  struct xmlnode *next;
  struct xmlnode *children;
  struct xmlnode *parent;
  enum xmlnodeid type;
  char *name;
  HMAP_DB *attributes;
} xmlnode;

typedef struct xmlreader {
  buffer* b;
  xmlnode* doc;
  xmlnode* parent;
  xmlnode** ptr;
  HMAP_DB* attrmap;
  int closing : 1;
  int self_closing : 1;
  void* data;
} xmlreader;

typedef struct xmlnodeset {
  xmlnode** nodes;
  size_t size;
} xmlnodeset;

#define node_is_closing(n) ((n)->type == XML_ELEMENT && (n)->name[0] == '/')

typedef int(xml_read_callback_fn)(xmlreader *r, xmlnodeid id, stralloc *name,
                                  stralloc *value, HMAP_DB **attrs);

typedef int(xml_predicate_fn)();

xmlnode*    xml_attrnode(const char*, size_t name_len, const char* value, size_t value_len);
int         xml_content_sa(xmlnode*, stralloc* sa);
const char* xml_content(xmlnode*);
void        xml_debug(xmlnode*, buffer* b);
size_t      xml_escape(const char*, size_t n, stralloc* out);
xmlnode*    xml_find_element_attr(xmlnode*, const char* tag, const char* attr, const char* value);
xmlnode*    xml_find_element(xmlnode*, const char* tag);
void        xml_free(xmlnode*);
char*       xml_get_attribute(xmlnode*, const char* attr);
xmlnode*    xml_get_document(xmlnode*);
int         xml_match_name_and_attr();
int         xml_match_name();
xmlnode*    xml_newnode(xmlnodeid);
int         xml_path(const xmlnode*, stralloc* out);
void        xml_print_attributes(HMAP_DB*, buffer* b, const char* sep, const char* eq, const char* quot);
void        xml_print_nodeset(const xmlnodeset*, buffer* b);
void        xml_print(xmlnode*, buffer* b);
void        xml_reader_init(xmlreader*, buffer* b);
void        xml_read_callback(xmlreader*, xml_read_callback_fn* fn);
xmlnode*    xml_read_tree(buffer*);
xmlnode*    xml_root_element(xmlnode*);
xmlnode*    xml_textnode(const char*, size_t len);
int         xml_get_attribute_sa(xmlnode*, stralloc* sa, const char* name);
double      xml_get_attribute_double(xmlnode*, const char* attr);
xmlnode*    xml_remove(xmlnode**);
void        xml_delete(xmlnode*);
void        xml_add_child(xmlnode*, xmlnode* node);
void        xml_set_attribute_double(xmlnode*, const char* a, double d);
void        xml_set_attribute(xmlnode*, const char* a, const char* v);
xmlnode*    xml_element(const char*);

#define xml_attributes(node) ((node)->attributes ? (node)->attributes->list_tuple : NULL)

#define xmlnodeset_item(ns, i) ((ns)->nodes[i])

__inl xmlnode** xmlnodeset_begin(const xmlnodeset* ns) { return ns->nodes ? &ns->nodes[0] : NULL; }
__inl xmlnode** xmlnodeset_end(const xmlnodeset* ns) { return ns->nodes ? &ns->nodes[ns->size] : NULL; }

__inl size_t xmlnodeset_size(const xmlnodeset* set ) { return set->size; }
#define xmlnodeset_empty(ns) (xmlnodeset_size(ns) == 0)

typedef xmlnode** xmlnodeset_iter_t;

__inl void xmlnodeset_iter_pp(xmlnode*** itp) { ++(*itp); }
__inl void xmlnodeset_iter_mm(xmlnode*** itp) { --(*itp); }
__inl xmlnode* xmlnodeset_iter_ref(xmlnode*** itp) { return *(*itp); }
__inl int xmlnodeset_iter_eq(xmlnode*** itp1, xmlnode*** itp2) { return (*itp1) == (*itp2); }
__inl ptrdiff_t xmlnodeset_iter_dist(xmlnode*** itp1, xmlnode*** itp2) { return (*itp2) - (*itp1); }

#define XML_READ_BUFFER 4096
#define XML_HMAP_BUCKETS 48

xmlnode* xml_find_pred(xmlnode*, int (*pred)(xmlnode*, void* ), ...);
xmlnodeset xml_find_all(xmlnode*, int (*pred)(xmlnode*, void* ), ...);
xmlnodeset xml_pfind_all(xmlnode*, int (*pred)(xmlnode*, void* ), void* args[]);
xmlnodeset xml_find_all_1(xmlnode*, int (*pred)(), void*);
xmlnodeset xml_find_all_2(xmlnode*, int (*pred)(), void*, void*);
xmlnodeset xml_find_all_3(xmlnode*, int (*pred)(), void*, void*, void*);

xmlnode* xml_find_pred_1(xmlnode*, int (*pred)(), void* arg);
xmlnode* xml_find_pred_2(xmlnode*, int (*pred)(), void* a0, void* a1);
xmlnode* xml_find_pred_3(xmlnode*, int (*pred)(), void* a0, void* a1, void* a2);

xmlnode* xml_find_element(xmlnode*, const char* tag);
xmlnode* xml_find_pred_n(xmlnode*, int (*pred)(xmlnode*, void* ), int n, ...);
xmlnode* xml_pfind_pred(xmlnode*, int (*pred)(), void* vptr[]);

int xml_has_attribute_n(xmlnode*, const char* b, size_t n);
int xml_has_attribute(xmlnode*, const char* attr);

#ifdef __cplusplus
}
#endif
#endif /* LIB_XML_H_ */
