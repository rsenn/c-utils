#include "../uint64.h"
#include "../buffer.h"
#include "../hmap.h"
#include "../xml.h"

void
xml_print_attributes(HMAP_DB* db, buffer* b, const char* sep, const char* eq, const char* quot) {
  TUPLE* tpl = db->list_tuple;

  while(tpl) {
    buffer_putm_internal(b, tpl == db->list_tuple ? "" : sep, tpl->key, eq, quot, 0);

    switch(tpl->data_type) {
      case HMAP_DATA_TYPE_INT:
        buffer_putlong(b, tpl->vals.val_int);
        break;
      case HMAP_DATA_TYPE_UINT:
        buffer_putulong(b, tpl->vals.val_uint);
        break;
      case HMAP_DATA_TYPE_INT64:
        buffer_putlonglong(b, tpl->vals.val_longlong);
        break;
      case HMAP_DATA_TYPE_UINT64:
        buffer_putulonglong(b, tpl->vals.val_ulonglong);
        break;
      case HMAP_DATA_TYPE_DOUBLE:
        buffer_putdouble(b, tpl->vals.val_double, 15);
        break;
      case HMAP_DATA_TYPE_CHARS:
        buffer_put(b, tpl->vals.val_chars, tpl->data_len - 1);
        break;
      case HMAP_DATA_TYPE_CUSTOM:
        buffer_putptr(b, tpl->vals.val_custom);
        break;
    }
    buffer_puts(b, quot);
    if(tpl->next == db->list_tuple) break;
    tpl = tpl->next;
  }
}
