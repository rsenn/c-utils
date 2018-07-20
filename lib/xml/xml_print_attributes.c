#include "../hmap.h"
#include "../xml.h"

void
xml_print_attributes(xmlnode* node, buffer* b, const char* sep, const char* eq, const char* quot) {
  TUPLE* tpl = node->attributes->list_tuple;
  while(tpl) {
    buffer_putm(b, sep, tpl->key, eq, quot);
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
    buffer_puts(b, quot);
    if(tpl->next == node->attributes->list_tuple) break;
    tpl = tpl->next;
  }
}
