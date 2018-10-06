#include "../stralloc.h"

int
stralloc_prepend(stralloc* sa, const stralloc* other) {
  return stralloc_prependb(sa, other->s, other->len);
}
