#include "../path_internal.h"
#include "../utf8.h"

size_t
path_right(const char* s, size_t n) {
  size_t i = path_dirlen_b(s, n);

  while(i < n) {
    size_t len = u8_len(&s[i], 1);

    if(len > 1 || !path_issep(s[i]))
      break;
  }

  return i;

  /* const char* p = s + n - 1;

   while(p >= s && path_issep(*p))
     --p;

   while(p >= s && !path_issep(*p))
     --p;

   return p - s;*/
}
