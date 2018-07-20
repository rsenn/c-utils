#include "../hmap.h"
#include "../xml.h"

static void
xml_dump_attributes(xmlnode* node, buffer* b) {
  TUPLE* tpl = node->attributes->list_tuple;
  while(tpl) {
    buffer_putm(b, " ", tpl->key, "=\"");
    switch(tpl->data_type) {
    case HMAP_DATA_TYPE_INT:
      buffer_putlong(b, tpl->vals.val_int);
      break;
    case HMAP_DATA_TYPE_UINT:
      buffer_putulong(b, tpl->vals.val_uint);
      break;
    case HMAP_DATA_TYPE_INT64:
      buffer_putlonglong(b, tpl->vals.val_int64);
      break;
    case HMAP_DATA_TYPE_UINT64:
      buffer_putulonglong(b, tpl->vals.val_uint64);
      break;
    case HMAP_DATA_TYPE_DOUBLE:
      buffer_putdouble(b, tpl->vals.val_double);
      break;
    case HMAP_DATA_TYPE_CHARS:
      buffer_put(b, tpl->vals.val_chars, tpl->data_len);
      break;
    case HMAP_DATA_TYPE_CUSTOM:
      buffer_putptr(b, tpl->vals.val_custom);
      break;
    }
    buffer_putc(b, '"');
    if(tpl->next == node->attributes->list_tuple)
      break;
    tpl = tpl->next;
  }
}

static void
xml_dump_node(xmlnode* node, buffer* b, int depth) {
  do {
    buffer_putnspace(b, depth * 2);
    buffer_putm(b, "<", node->name);
    if(node->attributes)
      xml_dump_attributes(node, b);
    if(node->children) {
      buffer_puts(b, ">\n");
      xml_dump_node(node->children, b, depth + 1);
      buffer_putnspace(b, depth * 2);
      buffer_putm(b, "</", node->name, ">");
    } else {
      buffer_puts(b, node->name[0] == '?' ? "?>" : "/>");
    }
    buffer_putnlflush(b);
  } while((node = node->next));
}

void
xml_dump(xmlnode* node, buffer* b) {
  xml_dump_node(node, b, 0);
}
