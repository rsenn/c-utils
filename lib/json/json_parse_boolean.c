#include "../json_internal.h"

int
json_parse_boolean(jsonval* val, charbuf* b) {
  int ret;
  if((ret = charbuf_skip_ifset(b, "tf", 2)) > 0) {
    const char* n;
    int v = charbuf_peek(b) == 'r';
    for(n = v ? "rue" : "alse"; *n; ++n) {
      if(!charbuf_skip_ifeq(b, *n))
        return 0;
    }
    val->type = JSON_BOOL;
    val->boolv = v;
    return 1;
  }
  return ret;
}
