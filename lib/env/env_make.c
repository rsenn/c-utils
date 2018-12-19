/* ISC license. */

#include "../env.h"
#include "../str.h"
#include <errno.h>

int
env_make(const char** v, size_t argc, const char* s, size_t len) {
  while(argc--) {
    size_t n = str_len(s) + 1;
    if(n > len)
      return (errno = EINVAL, 0);
    *v++ = s;
    s += n;
    len -= n;
  }
  return 1;
}
