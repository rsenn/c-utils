#include "../path_internal.h"
#include "../utf8.h"

size_t
path_skip(const char* s, size_t n) {
  size_t i = 0, len;

  if((len = path_skip_component(&s[i], n - i))) {
    i += len;

    if((len = path_skip_separator(&s[i], n - i)))
      i += len;
  }

  /*for(i = 0; i < n; i += len)

    if((len = u8_len(&s[i], 1)) == 1 && path_issep(s[i]))
      break;

  for(i = 0; i < n; i += len)

    if((len = u8_len(&s[i], 1)) > 1 || !path_issep(s[i]))
      break;*/

  return i;
}
