#include "../json_internal.h"

int
json_array_parse(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifeq(b, '[')) {
    int ret;
    jsonitem **ptr, *item;
    unsigned char c;
    int64_t i = 0;

    j->type = JSON_ARRAY;
    j->itemv = 0;

    ptr = &j->itemv;
    charbuf_skip_pred(b, isspace);
    for(; (ret = charbuf_peekc(b, &c)) > 0; ptr = &item->next) {
      if(c == ']')
        break;

      if((item = json_append(ptr, json_undefined())) == 0)
        return -1;
      if((ret = json_parse(&item->value, b)) <= 0)
        break;

#ifdef JSON_DEBUG
      buffer_puts(buffer_2, "json array element ");
      buffer_putlonglong(buffer_2, i++);
      buffer_puts(buffer_2, ": ");
      json_print(item->value, buffer_2, json_compact_printer);
      buffer_putnlflush(buffer_2);
#endif

      charbuf_skip_pred(b, &isspace);
      if((ret = charbuf_getc(b, &c)) <= 0)
        return ret;
      if(c == ',') {
        charbuf_skip_pred(b, isspace);
        continue;
      }
      break;
    }

    if(c == ']')
      ret = 1;
    return ret;
  }
  return 0;
}