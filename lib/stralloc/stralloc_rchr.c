#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_rchr(const stralloc* sa, char c) {
  return byte_rchr(sa->s, sa->len, c);
}

