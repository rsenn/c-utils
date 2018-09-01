/* ISC license. */

#include <stdlib.h>

char const*
env_get(char const* s) {
  return getenv(s);
}
