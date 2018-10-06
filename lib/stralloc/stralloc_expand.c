#include "../windoze.h"
#include "../stralloc.H"

int
stralloc_expand(stralloc* sa) {
#if WINDOWS
  const char* s;
  size_t n;
  stralloc_nul(sa);
  s = sa->s;
  n = sa->len * 2 + 4;

  sa->s = sa->len = sa->a = 0;

  do {
    /* reserve some space */
    stralloc_ready(sa, n);
    /* repeat until we have reserved enough space */
  } while((n = ExpandEnvironmentStrings(s, sa->s, sa->a)) > sa->a);

  /* now truncate to effective length */
  if(n > 0)
    stralloc_trunc(sa, n - 1);

  free(s);

  return n;
#endif
}
