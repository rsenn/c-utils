#include "../json_internal.h"

int
json_parse_object(jsonval* val, charbuf* b) {
  if(charbuf_skip_ifeq(b, '{')) {
    stralloc key;
    int ret;
    unsigned char c;
    val->type = JSON_OBJECT;
    MAP_NEW(val->dictv);
    stralloc_init(&key);

    charbuf_pred_skip(b, predicate_ctype, isspace);
    if((ret = charbuf_peekc(b, &c)) <= 0)
      return ret;

    if(c == '}')
      return 1;

    for(; (ret = charbuf_peekc(b, &c)) > 0;) {
      jsonval *itemv, member = {.type = JSON_UNDEFINED};
      MAP_PAIR_T pair;
      stralloc_zero(&key);
      if((ret = json_parse_stralloc(b, &key, charbuf_skip_ifeq(b, '"'))) <= 0)
        return ret;
      stralloc_nul(&key);
      charbuf_pred_skip(b, predicate_ctype, isspace);

      if((ret = charbuf_peekc(b, &c)) <= 0)
        break;

      if(c == ':') {
        charbuf_next(b);
        charbuf_pred_skip(b, predicate_ctype, isspace);

        json_parse(&member, b);
        MAP_INSERT(val->dictv, key.s, key.len, &member, sizeof(jsonval));

        charbuf_pred_skip(b, predicate_ctype, isspace);

        if((ret = charbuf_peekc(b, &c)) <= 0)
          break;

        if(c == ',') {
          charbuf_next(b);
          charbuf_pred_skip(b, predicate_ctype, isspace);
          continue;
        }
      }
      break;
    }
    stralloc_free(&key);
    return 1;
  }
  return 0;
}
