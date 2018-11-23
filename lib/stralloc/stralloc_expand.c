#include "../windoze.h"
#if WINDOWS
#include <windows.h>
#else
#include <stdlib.h>
#endif
#include "../stralloc.h"

int
stralloc_expand(stralloc* sa) {
#if WINDOWS
  char* s;
  size_t n;
  stralloc_nul(sa);
  s = sa->s;
  n = sa->len * 2 + 4;

  sa->s = 0;
  sa->len = sa->a = 0;

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
