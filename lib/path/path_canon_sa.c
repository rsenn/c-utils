#include "../path_internal.h"

/* path_canon_sa canonicalizes sa in place, see path_canon().
 * returns 1 on success, 0 on out of memory. */
int
path_canon_sa(stralloc* sa) {
  if(!stralloc_ready(sa, sa->len + 2))
    return 0;

  sa->len = path_canon(sa->s, sa->a, sa->s, sa->len);
  return stralloc_nul(sa);
}
