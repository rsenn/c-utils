#include "../path_internal.h"
#include "../utf8.h"

size_t
path_len(const char* s, size_t n) {
  size_t i, len;

  for(i = 0; i < n; i += len) {
    /* u8_len() returns 0 on an invalid UTF-8 lead byte -- treat it
     * as one raw byte instead of looping forever without advancing. */
    if((len = u8_len(&s[i], 1)) == 0)
      len = 1;

    if(len == 1 && path_issep(s[i]))
      break;
  }

  return i;

  /*const char *p = s, *e = s + n;

  while(p < e && !path_issep(*p))
    ++p;
  return p - s;*/
}
