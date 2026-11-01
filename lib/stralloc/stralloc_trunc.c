#include "../stralloc.h"

/* Sets the length to n and puts a NUL byte after it (not counted in len). The
 * allocation is never reduced: it used to be realloc'ed to n + 1 without
 * updating sa->a, so the next append wrote past the block. stralloc_shrink
 * gives memory back. Growing to n > len leaves the new bytes undefined.
 * Returns 1, or 0 when out of memory (sa is then unchanged). */
int
stralloc_trunc(stralloc* sa, size_t n) {
  if(n + 1 > sa->a && !stralloc_ready(sa, n + 1))
    return 0;
  sa->len = n;
  sa->s[n] = '\0';
  return 1;
}
