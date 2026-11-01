#include "../uint64.h"
#include "../scan.h"
#include "../json_internal.h"

int
json_parse_number(jsonval* val, charbuf* b) {
  size_t n;
  stralloc num;
  int ret;
  unsigned char c;

  if((ret = charbuf_peek(b)) <= 0)
    return ret;
  c = ret;

  if(isdigit(c) || c == '-' || c == '.' || c == '+') {
    val->type = JSON_INT;
    stralloc_init(&num);

    for(;;) {
      stralloc_APPEND(&num, &c);

      if((ret = charbuf_nextc(b, &c)) <= 0)
        return ret;

      if(!isdigit(c) && c != '.' && c != 'E' && c != 'e') {
        /* the sign of an exponent, 1e-5 or 1E+5 */
        if(!((c == '-' || c == '+') && val->type == JSON_DOUBLE && (num.s[num.len - 1] == 'e' || num.s[num.len - 1] == 'E')))
          break;
      }

      if(c == '.' || c == 'E' || c == 'e')
        val->type = JSON_DOUBLE;
    }
    stralloc_nul(&num);
    n = val->type == JSON_INT ? scan_i64(num.s, &val->intv) : scan_double(num.s, &val->doublev);
    stralloc_free(&num);

    if(n > 0) {
      return 1;
    }
  }
  return 0;
}
