#include "../stralloc.h"
#include "../textcode.h"

/* like fmt_to_sa, for the fmt_* functions that take a set of characters to
 * escape; appends to sa, returns the number of bytes added, 0 on failure */
int
fmt_to_sa2(size_t (*func)(char*, const char*, size_t, const char*),
           stralloc* sa,
           const char* src,
           size_t len,
           const char* escapeme) {
  size_t needed = func(0, src, len, escapeme);

  if(!stralloc_readyplus(sa, needed))
    return 0;
  func(sa->s + sa->len, src, len, escapeme);
  sa->len += needed;
  return needed;
}
