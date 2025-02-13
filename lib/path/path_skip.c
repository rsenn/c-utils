#include "../path_internal.h"
#include "../utf8.h"

size_t
path_skip(const char* s, size_t n) {
  size_t i, len;

  for(i = 0; i < n; i += len)
    if((len = u8len(&s[i], 1)) == 1 && path_issep(s[i]))
      break;

  for(i = 0; i < n; i += len)
    if((len = u8len(&s[i], 1)) > 1 || !path_issep(s[i]))
      break;

  return i;
  /*const char *p = s, *e = s + n;

  while(p < e && !path_issep(*p))
    ++p;

  while(p + 1 < e && path_issep(*p))
    ++p;

  return p - s;*/
}
