/* ISC license. */

#include "../env.h"
#include <errno.h>
#include <string.h>

int
env_make(char const** v, size_t argc, char const* s, size_t len) {
  while(argc--) {
    size_t n = strlen(s) + 1;
    if(n > len) return (errno = EINVAL, 0);
    *v++ = s;
    s += n;
    len -= n;
  }
  return 1;
}
