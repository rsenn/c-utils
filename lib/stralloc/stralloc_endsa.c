#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_endsa(const stralloc* sa, const stralloc* suffix) {
  return stralloc_endb(sa, suffix->s, suffix->len);
}
