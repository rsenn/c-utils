#include "../uint64.h"
#include "../buffer.h"
#include "../hmap.h"

static void
put_tuple(buffer* b, TUPLE* t) {
  const char* x = t->key;
  size_t n = t->key_len;
  buffer_put(b, "#", 1);
  buffer_putulong0(b, (t)->index, 5);
  buffer_puts(b, " ");

  while(n > 0 && x[n - 1] == '\0')
    --n;
  buffer_put(b, x, n);
  buffer_putm_internal(b, " = ", NULL);
  buffer_flush(b);
}

void
hmap_dump(HMAP_DB* hmap, buffer* b) {
  TUPLE* tuple = hmap->list_tuple;

  while(tuple) {

    put_tuple(b, tuple);

    switch(tuple->data_type) {
      case HMAP_DATA_TYPE_INT: buffer_putlong(b, tuple->vals.val_int); break;
      case HMAP_DATA_TYPE_UINT: buffer_putulong(b, tuple->vals.val_uint); break;
      case HMAP_DATA_TYPE_INT64: buffer_putlonglong(b, tuple->vals.val_longlong); break;
      case HMAP_DATA_TYPE_UINT64: buffer_putulonglong(b, tuple->vals.val_ulonglong); break;
      case HMAP_DATA_TYPE_DOUBLE: buffer_putdouble(b, tuple->vals.val_double, 15); break;
      case HMAP_DATA_TYPE_CHARS: {
        const char* x = tuple->vals.val_chars;
        size_t n = tuple->data_len;

        while(n > 0 && x[n - 1] < 0x20)
          --n;
        buffer_puts(b, "\"");
        buffer_put(b, x, n);
        buffer_puts(b, "\"");
        buffer_flush(b);
        break;
      }
      case HMAP_DATA_TYPE_CUSTOM: buffer_putptr(b, tuple->vals.val_custom); break;
    }

    if(tuple->next == hmap->list_tuple)
      break;

    tuple = tuple->next;
    buffer_putnlflush(b);
  }
}
