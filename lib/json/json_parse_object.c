#include "../json_internal.h"

int
json_parse_object(jsonval* val, charbuf* b) {
  if(charbuf_skip_ifeq(b, '{')) {
    int ret;
    unsigned char c;
    stralloc str;
    val->type = JSON_OBJECT;
    MAP_NEW(val->dictv);
    stralloc_init(&str);

    charbuf_skip_ws(b);
    for(; (ret = charbuf_peekc(b, &c)) > 0;) {
      jsonval *itemv, key = {.type = JSON_UNDEFINED}, member = {.type = JSON_UNDEFINED};
      if(c == '}') {
        charbuf_nextc(b, &c);
        return 1;
      }
      stralloc_zero(&str);
      if(json_parse_string(&key, b) > 0)
        stralloc_copy(&str, &key.stringv);
      else if(json_parse_number(&key, b) > 0)
        stralloc_catulong(&str, key.intv);

      stralloc_nul(&str);

/*      stralloc_zero(&key);
      if((ret = json_parse_stralloc(b, &key, charbuf_skip_ifeq(b, '"'))) <= 0)
        return ret;
*/
#if defined(DEBUG_OUTPUT) && defined(DEBUG_JSON)
      buffer_putm_internal(buffer_2, "json_parse_object ", " str=", str.s, 0);
      buffer_putnlflush(buffer_2);
#endif

      charbuf_skip_ws(b);
      if(charbuf_skip_ifeq(b, ':')) {
        charbuf_skip_ws(b);
        if(json_parse(&member, b) > 0) {
          MAP_PAIR_T pair;
          MAP_INSERT(val->dictv, str.s, str.len, &member, sizeof(jsonval));
          pair = 0;
          MAP_SEARCH(val->dictv, str.s, str.len, &pair);

          charbuf_skip_ws(b);
          if(charbuf_peekc(b, &c) > 0) {
            if(c == ',' || c == '}') {
              charbuf_skip(b);
              charbuf_skip_ws(b);
              if(c == ',')
                continue;
            }
          }
        }
      }
      break;
    }
    stralloc_free(&str);
    return 1;
  }
  return 0;
}