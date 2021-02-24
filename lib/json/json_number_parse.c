#include "../json_internal.h"

int
json_number_parse(jsonval* j, charbuf* b) {
  size_t n;
  stralloc num;
  int ret;
  unsigned char c;
  if((ret = charbuf_peek(b)) <= 0)
    return ret;
  c = ret;
  if(isdigit(c) || c == '-' || c == '.' || c == '+') {
    j->type = JSON_INT;
    stralloc_init(&num);
    for(;;) {
      stralloc_APPEND(&num, &c);
      if((ret = charbuf_nextc(b, &c)) <= 0)
        return ret;
      if(!isdigit(c) && c != '.' && c != 'E' && c != 'e') {
        if(!(j->type == JSON_DOUBLE && c == '-'))
          break;
      }
      if(c == '.' || c == 'E' || c == 'e')
        j->type = JSON_DOUBLE;
    }
    stralloc_nul(&num);
    n = j->type == JSON_INT ? scan_longlong(num.s, &j->intv) : scan_double(num.s, &j->doublev);
    stralloc_free(&num);
    if(n > 0) {
      return 1;
    }
  }
  return 0;
}
