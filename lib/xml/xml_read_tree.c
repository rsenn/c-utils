#include "../byte.h"
#include "../fmt.h"
#include "../scan.h"
#include "../xml.h"

static size_t
xml_unescape(const stralloc* in, stralloc* out) {
  return byte_scan(in->s, in->len, out, scan_xmlescape);
}

static int
xml_read_node(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  switch(id) {
    case XML_NODE_ATTRIBUTE: {


      buffer_putm(buffer_2, "reading attribute '", name->s, "' value '", value->s, "'");
      buffer_putnlflush(buffer_2);
      break;
    }
    case XML_NODE_TEXT: {
      stralloc text;
      stralloc_init(&text);
      xml_unescape(value, &text);
      stralloc_nul(&text);
      xmlnode* tnode = xml_newnode(XML_NODE_TEXT);
      tnode->name = text.s;
      tnode->parent = reader->parent;
      *reader->ptr = tnode;
      reader->ptr = &tnode->next;
      break;
    }
    case XML_NODE_ELEMENT:
    default: {
      buffer_putm(buffer_2, "reading element '", name->s, "'");
      buffer_putnlflush(buffer_2);
      if(reader->closing) {
        xmlnode* parent = reader->parent;
        if(parent) {
          reader->ptr = &parent->next;
          reader->parent = parent->parent;
        }
        xmlnode* node = xml_newnode(XML_NODE_ELEMENT);
        *reader->ptr = node;
        reader->ptr = &node->next;
        stralloc_insertb(name, "/", 0, 1);
        stralloc_nul(name);
        node->name = name->s;
        name->s = NULL;
      } else {
        xmlnode* node = xml_newnode(id);
        stralloc_nul(name);
        node->name = name->s;
        name->s = NULL;
        if(reader->parent) node->parent = reader->parent;
        if(reader->self_closing) {
          *(reader->ptr) = node;
          reader->ptr = &node->next;
        } else {
          *(reader->ptr) = node;
          reader->parent = node;
          reader->ptr = &node->children;
        }
        node->attributes = *attrs;
        *attrs = NULL;
        break;
      }
    }
  }
  return 1;
}

xmlnode*
xml_read_tree(buffer* b) {
  xmlreader r;
  xml_reader_init(&r, b);
  xml_read_callback(&r, xml_read_node);
  return r.doc;
}
