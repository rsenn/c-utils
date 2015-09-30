#include "byte.h"
#include "wstralloc.h"

int wstralloc_remove(wstralloc* sa, size_t pos, size_t n) {
  if(pos + 1 > sa->len) return -1;
  if(pos + n >= sa->len) {
    n = sa->len - pos;
    sa->len = pos;
    return n;
  }
  word_copy(&sa->s[pos], sa->len - (pos + n), &sa->s[pos + n]);
  sa->len -= n;
  return n;
}
