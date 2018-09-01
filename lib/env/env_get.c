/* ISC license. */

#include <skalibs/posixplz.h>
#include <stdlib.h>

char const*
env_get(char const* s) {
  return getenv(s);
}
