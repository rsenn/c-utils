#include "../windoze.h"
#include "../stralloc.H"

int
stralloc_expand(stralloc* sa, const char* s) {
#if WINDOWS
  size_t n = str_len(s) * 2 + 4;

  do {
    /* reserve some space */
    stralloc_ready(sa, n);
    /* repeat until we have reserved enough space */
  } while((n = ExpandEnvironmentStrings(s, sa->s, sa->len)) > sa->len);

  /* now truncate to effective length */
  if(n > 0)
    stralloc_trunc(sa, n - 1);

  return n;
#else
  return stralloc_copys(sa, s);
#endif
}
