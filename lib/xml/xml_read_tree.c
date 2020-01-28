#include "../byte.h"
#include "../fmt.h"
#include "../scan.h"
#include "../str.h"
#include "../xml.h"

static size_t
xml_unescape(const char* x, size_t n, stralloc* out) {
  return byte_scan(x, n, out, scan_xmlescape);
}

static int
xml_read_node(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  xmlnode** nptr;
  stralloc text;
  stralloc_init(&text);

  switch(id) {
    case XML_ATTRIBUTE: {
#ifdef XML_DEBUG
      buffer_putm_internal(buffer_2, "reading attribute '", name->s, "' value '", value->s, "'", 0);
      buffer_putnlflush(buffer_2);
#endif /* defined XML_DEBUG */
      break;
    }
    case XML_TEXT: {
      xmlnode* tnode;
      const char* x = name && name->s ? name->s : value && value->s ? value : "";
      size_t i, n = name && name->len ? name->len : value && value->len ? value->len : 0;

      i = scan_whitenskip(x, n);

      x += i;
      n -= i;

      if(n > 0) {
        tnode = xml_newnode(XML_TEXT);

        stralloc_zero(&text);
        xml_unescape(x, n, &text);
        stralloc_nul(&text);

        tnode->name = text.s;
        text.s = 0;
        tnode->parent = (reader->parent && reader->parent->parent) ? reader->parent->parent : 0;
        tnode->next = 0;

        nptr = &reader->parent->next;
        while(*nptr) nptr = &(*nptr)->next;
        *nptr = tnode;
      }
      break;
    }
    case XML_ELEMENT:
    default: {
#ifdef XML_DEBUG
      buffer_putm_internal(buffer_2, "reading element '", name->s, "'", 0);
      buffer_putnlflush(buffer_2);
#endif /* defined XML_DEBUG */

      if(reader->closing) {
        xmlnode *node, *parent = reader->parent;

        if(parent) {
          reader->ptr = &parent->next;
          reader->parent = parent->parent;
        }
        node = xml_newnode(XML_ELEMENT);
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
  xml_reader_init(&r, b);
  xml_read_callback(&r, xml_read_node);
  return r.doc;
}
