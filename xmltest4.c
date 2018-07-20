#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include <assert.h>

void
xml_print_attrs(xmlnode* node) {
  TUPLE* tpl = node->attributes->list_tuple;

  while(tpl) {

    buffer_putm(buffer_1, " ", tpl->key, "=\"");

    switch(tpl->data_type) {
      case HMAP_DATA_TYPE_INT:
        buffer_putlong(buffer_1, tpl->vals.val_int);
        break;
      case HMAP_DATA_TYPE_UINT:
        buffer_putulong(buffer_1, tpl->vals.val_uint);
        break;
      case HMAP_DATA_TYPE_INT64:
        buffer_putlonglong(buffer_1, tpl->vals.val_int64);
        break;
      case HMAP_DATA_TYPE_UINT64:
        buffer_putulonglong(buffer_1, tpl->vals.val_uint64);
        break;
      case HMAP_DATA_TYPE_DOUBLE:
        buffer_putdouble(buffer_1, tpl->vals.val_double);
        break;
      case HMAP_DATA_TYPE_CHARS:
        buffer_put(buffer_1, tpl->vals.val_chars, tpl->data_len);
        break;
      case HMAP_DATA_TYPE_CUSTOM:
        buffer_putptr(buffer_1, tpl->vals.val_custom);
        break;
    }

    buffer_putc(buffer_1, '"');

    if(tpl->next == node->attributes->list_tuple)
      break;

    tpl = tpl->next;
  }
}

void
xml_dump_node(xmlnode* node, buffer* b, int depth) {
  do {
    buffer_putnspace(b, depth * 2);
    buffer_putm(b, "<", node->name);

    if(node->attributes)
      xml_print_attrs(node);

    if(node->children) {
      buffer_puts(b, ">\n");
//      buffer_putnspace(b, depth * 2);
      xml_dump_node(node->children, depth + 1);
      buffer_putnspace(b, depth  * 2);
      buffer_putm(b, "</", node->name, ">");
    } else {
      buffer_puts(b, "/>");
    }



//    buffer_puts(b, " next: ");
//    buffer_putptr(b, node->next);
//    buffer_puts(b, " parent: ");
//    buffer_putptr(b, node->parent);
    buffer_putnlflush(b);
//    if(node->children) {
//      xml_dump_node(node->children, depth + 1);
//    }
  } while((node = node->next));
}

void
xml_add_attrs(xmlnode** ptr, HMAP_DB* hmap) {

  for(TUPLE** it = hmap_begin(hmap); hmap_iterator_dereference(it); hmap_iterator_increment(it)) {
    TUPLE* tuple = hmap_iterator_dereference(it);

    assert(tuple->key);
    assert(tuple->data);

    xmlnode* attr = xml_attrnode(tuple->key, tuple->key_len, tuple->data, tuple->data_len);

    *ptr = attr;
    ptr = &attr->next;

    //    buffer_puts(buffer_1, "attribute: ");
    //    buffer_put(buffer_1, hmap_iterator_dereference(it)->key, hmap_iterator_dereference(it)->key_len);
    //    buffer_puts(buffer_1, "=\"");
    //    if(hmap_iterator_dereference(it)->data)
    //      buffer_put(buffer_1, hmap_iterator_dereference(it)->data, hmap_iterator_dereference(it)->data_len);
    //    else
    //      buffer_puts(buffer_1, "NULL");
    //    buffer_puts(buffer_1, "\"");
    //    buffer_putnlflush(buffer_1);

    if(hmap_last(hmap, it))
      break;
  }
}

int
xml_read_node(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  switch(id) {
    case XML_NODE_ATTRIBUTE: {
      break;
    }
    case XML_NODE_ELEMENT:
    default: {
   //   hmap_print_list(reader->attrmap);
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
        //    name->s = NULL;
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
      //  xml_print_attrs(node);

      //  hmap_dump(node->attributes, buffer_2);
//        *attrs = NULL;
//        xml_add_attrs(&node->attributes, reader->attrmap);

        break;
      }
    }
  }
  return 1;
}

int
main() {
  buffer input;
  xmlreader reader;
  buffer_mmapprivate(&input, "../dirlist/test.xml");
  xml_read(&reader, &input, &xml_read_node);
  xml_dump_node(reader.doc, 0);
}
