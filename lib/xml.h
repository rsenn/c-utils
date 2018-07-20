#ifndef LIB_XML_H_
#define LIB_XML_H_

#include "buffer.h"
#include "stralloc.h"

typedef enum xmlnodeid {
    XML_NODE_DOCUMENT,
    XML_NODE_ELEMENT,
    XML_NODE_ATTRIBUTE,
    XML_NODE_TEXT,
} xmlnodeid;

typedef struct xmlnode {
    enum xmlnodeid type;
    struct xmlnode* next;
    struct xmlnode* children;
    struct xmlnode* parent;
} xmlnode;


typedef struct xmlreader {
    buffer* b;
    xmlnode* doc;
    xmlnode** ptr;
    int closing:1;
    int self_closing:1;
} xmlreader;

typedef int (xml_read_fn)(xmlreader* r, xmlnodeid id, stralloc* name,  stralloc* value);


xmlnode* xml_newnode(xmlnodeid type);
void xml_read(xmlreader* r, buffer* b, xml_read_fn* fn);

#endif /* LIB_XML_H_ */
