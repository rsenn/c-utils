#include "../json_internal.h"

int
json_parse_array(jsonval* val, charbuf* b) {
  if(charbuf_skip_ifeq(b, '[')) {
    int ret;
    jsonitem **ptr, *item;
    unsigned char c;
    int64_t i = 0;

    val->type = JSON_ARRAY;
    val->itemv = 0;

    ptr = &val->itemv;
    charbuf_pred_skip(b, charbuf_is_ctype, isspace);
    while((ret = charbuf_peekc(b, &c)) > 0) {
      if(c == ']') {
        ret = 1;
        break;
      }

      ptr = &((item = json_append(ptr, json_undefined()))->next);

      json_parse(&item->value, b);

#ifdef JSON_DEBUG
      buffer_puts(buffer_2, "json array element ");
      buffer_putlonglong(buffer_2, i++);
      buffer_puts(buffer_2, ": ");
      json_print(item->value, buffer_2, json_compact_printer);
      buffer_putnlflush(buffer_2);
#endif
      charbuf_pred_skip(b, charbuf_is_ctype, isspace);
      if((ret = charbuf_peekc(b, &c)) <= 0)
        break;
      if(c == ']')
        continue;
      if(charbuf_skip_ifeq(b, ',')) {

        charbuf_pred_skip(b, charbuf_is_ctype, isspace);
        continue;
      }
      break;
    }

    return 1;
  }
  return 0;
}