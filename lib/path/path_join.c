#include "internal.h"
#include "../alloc.h"
#include "../str.h"

/* path_join returns a malloc'd path_cat() of a and b, with room for path_canon() in place.
 * (n + 2 bytes; NULL on out of memory) */
char*
path_join(const char* a, const char* b) {
  size_t an = str_len(a), bn = str_len(b), n = path_cat(0, 0, a, an, b, bn);
  char* s = alloc(n + 2);

  if(s)
    path_cat(s, n + 1, a, an, b, bn);

  return s;
}
