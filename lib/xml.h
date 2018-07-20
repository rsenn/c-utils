#ifndef LIB_XML_H_
#define LIB_XML_H_

#include "buffer.h"

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


xmlnode* xml_newnode(xmlnodeid type);
void xml_read(xmlreader* r, buffer* b);

#endif /* LIB_XML_H_ */
