
#include "../json_internal.h"
#include "../bool.h"

int json_parse_getsa(charbuf*, stralloc*, bool);

int
json_object_parse(jsonval* j, charbuf* b) {
  if(charbuf_skip_ifeq(b, '{')) {
    stralloc key;
    int ret;
    unsigned char c;
    j->type = JSON_OBJECT;
    MAP_NEW(j->dictv);
    stralloc_init(&key);
    charbuf_skip_pred(b, isspace);

    for(; (ret = charbuf_peekc(b, &c)) > 0;) {
      jsonval *itemv, member = {.type = JSON_UNDEFINED};
      MAP_PAIR_T pair;
      if(c == '}') {
        charbuf_skip(b);
        return 1;
      }
      stralloc_zero(&key);
      if((ret = json_parse_getsa(b, &key, charbuf_skip_ifeq(b, '"'))) <= 0)
        return ret;
      stralloc_nul(&key);
      charbuf_skip_pred(b, &isspace);
      if(!charbuf_skip_ifeq(b, ':'))
        return 0;
      charbuf_skip_pred(b, &isspace);
      if((ret = json_parse(&member, b)) > 0) {
        MAP_INSERT(j->dictv, key.s, key.len, &member, sizeof(jsonval));
        charbuf_skip_pred(b, &isspace);
        if(charbuf_skip_ifeq(b, ',')) {
          charbuf_skip_pred(b, &isspace);
          continue;
        }
      }
      break;
    }
    stralloc_free(&key);
    return ret;
  }
  return 0;
}
