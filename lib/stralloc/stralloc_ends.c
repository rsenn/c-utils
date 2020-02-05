#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_ends(const stralloc* sa, const stralloc* suffix) {
  return stralloc_ends(sa, suffix->s, suffix->len);
}
