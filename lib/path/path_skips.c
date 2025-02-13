#include "../path_internal.h"
#include "../utf8.h"

size_t
path_skips(const char* s) {
  size_t i, len;

  for(i = 0; s[i]; i += len)
    if((len = u8len(&s[i], 1)) == 1 && path_issep(s[i]))
      break;

  for(i = 0; s[i]; i += len)
    if((len = u8len(&s[i], 1)) > 1 || !path_issep(s[i]))
      break;

  return i;
  /* register const char* p = s;

  while(*p && !path_issep(*p))
    ++p;

  while(*p && path_issep(*p))
    ++p;

  return p - s;*/
}
