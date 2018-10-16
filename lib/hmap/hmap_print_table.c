#include "../hmap_internal.h"

static void
hmap_print(HMAP_DB* hmap) {
  int i = 0;
  TUPLE* t = NULL;
  t = hmap->tuple;
  for(i = 0; i < hmap->bucket_size; i++) {
    buffer_puts(buffer_1, "index[");
    buffer_putlong(buffer_1, t->index);
    buffer_puts(buffer_1, "][");
    buffer_putptr(buffer_1, t);
    buffer_puts(buffer_1, "] key[");
    buffer_put(buffer_1, t->key, t->key_len);
    buffer_puts(buffer_1, "], data[");

    switch(t->data_type) {
      case HMAP_DATA_TYPE_INT: buffer_putlong(buffer_1, t->vals.val_int); break;
      case HMAP_DATA_TYPE_UINT: buffer_putulong(buffer_1, t->vals.val_uint); break;
      case HMAP_DATA_TYPE_INT64: buffer_putint64(buffer_1, t->vals.val_int64); break;
      case HMAP_DATA_TYPE_UINT64: buffer_putuint64(buffer_1, t->vals.val_uint64); break;
      case HMAP_DATA_TYPE_DOUBLE: buffer_putdouble(buffer_1, t->vals.val_double, 15); break;
      case HMAP_DATA_TYPE_CHARS: buffer_put(buffer_1, t->vals.val_chars, t->data_len); break;
      case HMAP_DATA_TYPE_CUSTOM: buffer_putptr(buffer_1, t->vals.val_custom); break;
    }
    t++;
  }
}

int
hmap_print_table(HMAP_DB* hmap) {
  if(hmap == NULL) return HMAP_DB_EMPTY;
  hmap_print(hmap);
  return HMAP_SUCCESS;
}
