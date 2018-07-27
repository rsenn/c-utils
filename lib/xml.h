#ifndef LIB_XML_H_
#define LIB_XML_H_

#include <sys/types.h>
#include "buffer.h"
#include "hmap.h"
#include "stralloc.h"
#include "textbuf.h"


#define __inl static inline

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

#define node_is_closing(n) ((n)->name[0] == '/')

typedef int(xml_read_callback_fn)(xmlreader *r, xmlnodeid id, stralloc *name,
                                  stralloc *value, HMAP_DB **attrs);

typedef int(xml_predicate_fn)();

xmlnode*    xml_attrnode(const char* name, size_t name_len, const char* value, size_t value_len);
int         xml_content_sa(xmlnode* node, stralloc* sa);
const char* xml_content(xmlnode* node);
void        xml_debug(xmlnode* node, buffer* b);
size_t      xml_escape(const char* s, size_t n, stralloc* out);
xmlnodeset  xml_find_all(xmlnode* node, int (*pred)(xmlnode* , void* ), ...);
xmlnode*    xml_find_element_attr(xmlnode* node, const char* tag, const char* attr, const char* value);
xmlnode*    xml_find_element(xmlnode* node, const char* tag);
xmlnode*    xml_find_pred(xmlnode* node, int (*pred)(xmlnode* , void* ), ...);
void        xml_free(xmlnode* node);
char*       xml_get_attribute(xmlnode* node, const char* attr);
xmlnode*    xml_get_document(xmlnode* node);
int         xml_has_attribute(xmlnode* node, const char* attr);
int         xml_match_name_and_attr(xmlnode* node, const char* name, const char* attr, const char* value);
int         xml_match_name(xmlnode* node, const char* name, const char* attr, const char* value);
xmlnode*    xml_newnode(xmlnodeid type);
int         xml_path(const xmlnode* node, stralloc* out);
void        xml_print_attributes(xmlnode* node, buffer* b, const char* sep, const char* eq, const char* quot);
void        xml_print_nodeset(const xmlnodeset* ns, buffer* b);
void        xml_print(xmlnode* node, buffer* b);
void        xml_reader_init(xmlreader* r, buffer* b);
void        xml_read_callback(xmlreader* r, xml_read_callback_fn* fn);
xmlnode*    xml_read_tree(buffer* b);
xmlnode*    xml_textnode(const char* text, size_t len);


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

#endif /* LIB_XML_H_ */
