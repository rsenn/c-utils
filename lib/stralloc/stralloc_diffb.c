#include "../byte.h"
#include "../stralloc.h"

int
stralloc_diffb(const stralloc* sa, const void* d, unsigned int dlen) {
  int r;
  size_t len;

  /* get shortest len */
  if((len = sa->len) > dlen)
    len = dlen;

  /* compare common lengths */
  if((r = byte_diff(sa->s, len, d)))
    return r;

  return (int)sa->len - (int)dlen;
}
