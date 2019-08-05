#include "../path_internal.h"
size_t
path_num_sa(const char* p, size_t len, stralloc* sa, int n) {
  size_t r = path_num(p, len, n);
  stralloc_copyb(sa, p, r);
  return r;
}

