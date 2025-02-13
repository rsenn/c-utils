#include "../path_internal.h"
#include "../utf8.h"

size_t
path_skip_separator(const char* s, size_t n) {
  size_t i, len;

  for(i = 0; i < n; i += len)
    if((len = u8_len(&s[i], 1)) > 1 || !path_issep(s[i]))
      break;

  return i;

  /*  const char *p = s, *e = s + n;

  for(p = s, e = s + n; p != e; p++)
    if(!path_issep(*p))
      break;

  return p - s;*/
}
