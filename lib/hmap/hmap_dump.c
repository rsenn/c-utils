#include "../hmap.h"
#include "../buffer.h"

#define put_tuple(t) \
  buffer_put(b, "#", 1); \
  buffer_putulong0(b, (t)->index, 5); \
  buffer_putm_internal(b, " ", (t)->key, "=", NULL);

void
hmap_dump(HMAP_DB* hmap, buffer* b) {
  TUPLE* tuple = hmap->list_tuple;

  while(tuple) {

    put_tuple(tuple);

    switch(tuple->data_type) {
      case HMAP_DATA_TYPE_INT:
        buffer_putlong(b, tuple->vals.val_int);
        break;
      case HMAP_DATA_TYPE_UINT:
        buffer_putulong(b, tuple->vals.val_uint);
        break;
      case HMAP_DATA_TYPE_INT64:
        buffer_putint64(b, tuple->vals.val_int64);
        break;
      case HMAP_DATA_TYPE_UINT64:
        buffer_putuint64(b, tuple->vals.val_uint64);
        break;
      case HMAP_DATA_TYPE_DOUBLE:
        buffer_putdouble(b, tuple->vals.val_double, 15);
        break;
      case HMAP_DATA_TYPE_CHARS:
        buffer_putm_internal(b, "\"", tuple->vals.val_chars, "\"", NULL);
        break;
      case HMAP_DATA_TYPE_CUSTOM:
        buffer_putptr(b, tuple->vals.val_custom);
        break;
    }

    if(tuple->next == hmap->list_tuple)
      break;

    tuple = tuple->next;
  }
}
