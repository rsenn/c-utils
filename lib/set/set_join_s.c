#include "../set.h"

/* The values joined with sep as a string that malloc made (free it), NULL if
 * the memory ran out. */
char*
set_join_s(const set_t* set, const char* sep) {
  stralloc sa;

  stralloc_init(&sa);

  if(!set_join(set, sep, &sa) || !stralloc_nul(&sa)) {
    stralloc_free(&sa);
    return NULL;
  }

  return sa.s;
}
