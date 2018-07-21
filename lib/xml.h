#ifndef LIB_XML_H_
#define LIB_XML_H_

#include <sys/types.h>
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

typedef struct xmlnodeset {
  xmlnode** nodes;
  size_t count;
} xmlnodeset;

#define node_is_closing(n) ((n)->name[0] == '/')

typedef int(xml_read_callback_fn)(xmlreader *r, xmlnodeid id, stralloc *name,
                         stralloc *value, HMAP_DB **attrs);

xmlnode*   xml_attrnode           (const char*, size_t, const char*, size_t);
void       xml_debug              (xmlnode*, buffer*);
size_t     xml_escape             (const char*, size_t, stralloc*);
xmlnodeset xml_find_all           (xmlnode*, int (*pred)(), ...);
xmlnode*   xml_find_element       (xmlnode*, const char*);
xmlnode*   xml_find_element_attr  (xmlnode*, const char*, const char*, const char*);
xmlnode*   xml_find_pred          (xmlnode*, int (*pred)(), ...);
void       xml_free               (xmlnode*);
char*      xml_get_attribute      (xmlnode*, const char*);
xmlnode*   xml_get_document       (xmlnode*);
int        xml_has_attribute      (xmlnode*, const char*);
int        xml_match_name         (xmlnode*, const char*);
int        xml_match_name_and_attr(xmlnode*, const char*, const char*, const char*);
xmlnode*   xml_newnode            (xmlnodeid);
int        xml_path               (const xmlnode*, stralloc*);
void       xml_print              (xmlnode*, buffer*);
void       xml_print_attributes   (xmlnode*, buffer*, const char*, const char*, const char*);
void       xml_print_nodeset      (const xmlnodeset*, buffer*);
void       xml_reader_init        (xmlreader*, buffer*);
void       xml_read_callback      (xmlreader*, xml_read_callback_fn*);
xmlnode*   xml_read_tree          (buffer*);
xmlnode*   xml_textnode           (const char*, size_t);



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
