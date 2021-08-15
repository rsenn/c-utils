/* ISC license. */

#include "../env.h"
#include <sys/types.h>

size_t
env_len(const char* const* e) {
  size_t i = 0;
  while(*e++)
    i++;
  return i;
}
