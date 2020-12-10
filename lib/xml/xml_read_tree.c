#include "../byte.h"
#include "../fmt.h"
#include "../buffer.h"
#include "../scan.h"
#include "../str.h"
#include "../xml.h"
#include "../alloc.h"
#include <assert.h>

static void
xmlreader_push(xmlreader* r, xmlnode* parent) {
  xmlreader* old = (xmlreader*)alloc_zero(sizeof(xmlreader));
  byte_copy(old, sizeof(xmlreader), r);

  r->stack = old;
  r->parent = parent;
  r->ptr = &parent->children;
}

static void
xmlreader_pop(xmlreader* r) {
  xmlreader* prev;
  assert(r->stack);
  prev = r->stack;
  byte_copy(r, sizeof(xmlreader), r->stack);
  alloc_free(prev);
}

static size_t
xml_unescape(const char* x, size_t n, stralloc* out) {
  return byte_scan(x, n, out, scan_xmlescape);
}

static int
xml_read_node(xmlreader* r, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  xmlnode** nptr;
  stralloc text;
  stralloc_init(&text);

  switch(id) {
    case XML_ATTRIBUTE: {
#if DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "reading attribute '", name->s, "' value '", value->s, "'", NULL);
      buffer_putnlflush(buffer_2);
#endif /* defined XML_DEBUG */
      break;
    }

    case XML_TEXT: {
      xmlnode *pnode, *tnode, **nptr;
      const char* x = name && name->s ? (const char*)name->s : /* value && (const char*)value->s ? (const char*)value->s :  */ "";
      size_t i, n = name && name->len ? name->len : /*  value && value->len ? value->len : */ 0;

      i = scan_whitenskip(x, n);

      x += i;
      n -= i;

      if(n > 0 && (pnode = r->parent) && pnode->type == XML_ELEMENT && (&r->parent->children == r->ptr)) {
        tnode = xml_newnode(XML_TEXT);

        stralloc_zero(&text);
        xml_unescape(x, n, &text);
        stralloc_nul(&text);

        tnode->name = text.s;
        text.s = 0;
        tnode->parent = r->parent;
        tnode->next = *r->ptr;

        *r->ptr = tnode;
      }
      break;
    }

    case XML_ELEMENT:
    default: {
      xmlnode* node = 0;
#if DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "reading element '", name->s, "'", NULL);
      buffer_putnlflush(buffer_2);
#endif /* defined XML_DEBUG */
      if(*r->ptr)
        r->ptr = &(*r->ptr)->next;

      if(r->closing)
        stralloc_insertb(name, "/", 0, 1);

      stralloc_nul(name);

      if(!r->closing || r->self_closing) {
        node = xml_element(name->s);
        node->children = NULL;
        node->parent = r->parent;
        node->attributes = *attrs;
        *attrs = NULL;
        *r->ptr = node;
      }

      if(!r->closing && !r->self_closing) {
        xmlreader_push(r, node);
        break;
      }

      if(r->closing && !r->self_closing) {
        xmlreader_pop(r);
        break;
      }
    }
  }

  return 1;
}

xmlnode*
xml_read_tree(buffer* b) {
  xmlreader r;
  xml_reader(&r, b);

  xml_read_callback(&r, xml_read_node);
  return r.doc;
}
