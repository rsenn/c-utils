/* ISC license. */
#include "../env.h"
#include "../str.h"
#include "../byte.h"

#include <stdlib.h>

const char*
env_get_b(const char* s, size_t namelen) {
  char** e;

  for(e = environ; *e; e++) {
    size_t len = str_chr(*e, '=');

    if(namelen == len && !byte_diff((*e), len, s))
      return (*e) + len + 1;
  }
  return 0;
  //  return getenv(s);
}
