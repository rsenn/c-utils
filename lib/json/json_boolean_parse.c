#include "../json_internal.h"

int
json_boolean_parse(jsonval* j, charbuf* b) {
  int ret;
  if((ret = charbuf_skip_ifset(b, "tf", 2)) > 0) {
    const char* n;
    int v = charbuf_peek(b) == 'r';
    for(n = v ? "rue" : "alse"; *n; ++n) {
      if(!charbuf_skip_ifeq(b, *n))
        return 0;
    }
    j->type = JSON_BOOL;
    j->boolv = v;
    return 1;
  }
  return ret;
}
