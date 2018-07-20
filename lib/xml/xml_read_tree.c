#include "../xml.h"

static int
xml_read_node(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  switch(id) {
    case XML_NODE_ATTRIBUTE: {
      break;
    }
    case XML_NODE_ELEMENT:
    default: {
      if(reader->closing) {
        xmlnode* parent = reader->parent; //((xmlnode*)(r->ptr))->parent;

        if(parent) {
          reader->ptr = &parent->next;
          reader->parent = parent->parent;
        }
      } else {
        xmlnode* node = xml_newnode(id);
        stralloc_nul(name);
        node->name = name->s;

        if(reader->parent)
          node->parent = reader->parent;

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
  xml_read_callback(&r, b, xml_read_node);
  return r.doc;
}
