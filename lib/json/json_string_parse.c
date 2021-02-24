#include "../json_internal.h"

int
json_string_parse(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifeq(b, '"')) {
    j->type = JSON_STRING;
    stralloc_init(&j->stringv);
    json_parse_getsa(b, &j->stringv, true);
    return 1;
  }
  return 0;
}
