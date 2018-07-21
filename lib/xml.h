#ifndef LIB_XML_H_
#define LIB_XML_H_

#include "buffer.h"
#include "hmap.h"
#include "stralloc.h"
#include "textbuf.h"

typedef enum xmlnodeid {
  XML_NODE_DOCUMENT,
  XML_NODE_ELEMENT,
  XML_NODE_ATTRIBUTE,
  XML_NODE_TEXT,
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
  int closing : 1;
  int self_closing : 1;
  HMAP_DB* attrmap;
} xmlreader;

typedef struct xmlxpathctx {
  xmlnode* doc;
} xmlxpathctx;

typedef struct xmlxpathobj {
  xmlnode** nodeset;
} xmlxpathobj;

#define node_is_closing(n) ((n)->name[0] == '/')

typedef int(xml_read_callback_fn)(xmlreader *r, xmlnodeid id, stralloc *name,
                         stralloc *value, HMAP_DB **attrs);

xmlnode *xml_attrnode(const char *name, size_t name_len, const char *value,
                      size_t value_len);
void xml_print(xmlnode *node, buffer *b);
xmlnode *xml_newnode(xmlnodeid type);
void xml_read_callback(xmlreader *r,  xml_read_callback_fn *fn);
xmlnode *xml_read_tree(buffer *b);
xmlnode *xml_textnode(const char *text, size_t len);
int xml_path(xmlnode *node, stralloc *out);
void xml_free(xmlnode *node);
void xml_print_attributes(xmlnode *node, buffer *b, const char *sep, const char *eq, const char *quot);
void xml_reader_init(xmlreader *r, buffer *b);
size_t xml_escape(const char *s, size_t n, stralloc *out);
void xml_debug(xmlnode *node, buffer *b);
xmlnode *xml_find_element(xmlnode *node, const char *tag);
xmlnode *xml_find_element_attr(xmlnode *node, const char *tag, const char *attr, const char *value);
const char *xml_get_attribute(xmlnode *node, const char *attr);
int xml_has_attribute(xmlnode *node, const char *attr);
xmlnode *xml_find_pred(xmlnode *node, int (*pred)(xmlnode*, void*), ...);
xmlnode *xml_get_document(xmlnode *node);

#define xml_attribute_list(node) ((node)->attributes ? (node)->attributes->list_tuple : NULL)

#define xml_nodeset_item(nodeset, i) ((nodeset[i]))
static inline size_t
xml_nodeset_length(xmlnode** nodeset) {
  size_t i = 0;
  if(nodeset) {
    while(nodeset[i])
      ++i;
  }
  return i;
}

#define XML_READ_BUFFER 4096
#define XML_HMAP_BUCKETS 48

#endif /* LIB_XML_H_ */
