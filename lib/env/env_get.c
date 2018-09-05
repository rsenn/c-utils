/* ISC license. */

#include <stdlib.h>

const char*
env_get(const char* s) {
  return getenv(s);
}
