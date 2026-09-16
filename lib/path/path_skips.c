#include "../path_internal.h"
#include "../utf8.h"

size_t
path_skips(const char* s) {
  size_t i, len;

  for(i = 0; s[i]; i += len) {
    /* u8_len() returns 0 on an invalid UTF-8 lead byte -- treat it
     * as one raw byte instead of looping forever without advancing. */
    if((len = u8_len(&s[i], 1)) == 0)
      len = 1;

    if(len == 1 && path_issep(s[i]))
      break;
  }

  for(i = 0; s[i]; i += len)

    if((len = u8_len(&s[i], 1)) > 1 || !path_issep(s[i]))
      break;

  return i;
  /* register const char* p = s;

  while(*p && !path_issep(*p))
    ++p;

  while(*p && path_issep(*p))
    ++p;

  return p - s;*/
}
