#ifndef LIB_XML_H_
#define LIB_XML_H_

#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/hmap.h"

typedef enum xmlnodeid {
    XML_NODE_DOCUMENT,
    XML_NODE_ELEMENT,
    XML_NODE_ATTRIBUTE,
    XML_NODE_TEXT,
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
    int closing:1;
    int self_closing:1;
    HMAP_DB* attrmap;
} xmlreader;

typedef int (xml_read_fn)(xmlreader* r, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs);


xmlnode* xml_newnode(xmlnodeid type);
xmlnode *xml_attrnode(const char *name, size_t name_len, const char *value, size_t value_len);
xmlnode *xml_textnode(const char *text, size_t len);

void xml_read(xmlreader* r, buffer* b, xml_read_fn* fn);

#endif /* LIB_XML_H_ */
