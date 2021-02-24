#include "../json_internal.h"

int
json_string_parse(jsonval* val, charbuf* b) {
  if(charbuf_skip_ifeq(b, '"')) {
    val->type = JSON_STRING;
    stralloc_init(&val->stringv);
    json_parse_getsa(b, &val->stringv, true);
    return 1;
  }
  return 0;
}
