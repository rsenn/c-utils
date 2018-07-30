#include "../buffer.h"
#include "../json.h"
#include "../slist.h"
#include "../stralloc.h"

static void json_print_list(jsonval*, buffer*, int);
static void
json_print_val(jsonval* val, buffer* b, int depth) {
  switch(val->type) {
    case JSON_STRING:
      buffer_puts(b, "\"");
      buffer_putsa(b, &val->stringv);
      buffer_puts(b, "\"");
      break;
    case JSON_DOUBLE:
      buffer_putdouble(b, val->doublev);
      break;
    case JSON_BOOL:
      buffer_puts(b, val->boolv ? "true" : "false");
      break;
    case JSON_INT:
      buffer_putlonglong(b, val->intv);
      break;
    case JSON_OBJECT: {
      TUPLE* t = hmap_begin(val->dictv);
      buffer_puts(b, "{\n");
      for(; t; t = t->next) {
        buffer_putnspace(b, 2 * (depth + 1));
        buffer_putc(b, '"');
        buffer_put(b, t->key, t->key_len);
        buffer_puts(b, "\": ");
        json_print_val(t->vals.val_custom, b, depth + 1);
        int last = t->next == hmap_begin(val->dictv);
        buffer_puts(b, last ? "\n" : ",\n");
        if(last) break;
      }
      buffer_putnspace(b, 2 * depth);
      buffer_puts(b, "}\n");
      break;
    }
    case JSON_ARRAY: {
      slink* ptr;
      buffer_puts(b, "[\n");
      buffer_putnspace(b, 2 * (depth + 1));

      for(ptr = val->listv.root; ptr; ptr = ptr->next) {
        json_print_val((jsonval*)&ptr[1], b, depth + 1);
        if(ptr->next) {
          buffer_puts(b, ",\n");
          buffer_putnspace(b, 2 * (depth + 1));
        }
      }

      buffer_puts(b, "\n");
      buffer_putnspace(b, 2 * depth);
      buffer_puts(b, "]");
      break;
    }
  }
}

void
json_print(jsonval* val, buffer* b) {
  json_print_val(val, b, 0);
  buffer_puts(b, "\n");
  buffer_flush(b);
}
