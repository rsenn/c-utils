#include "../set.h"

int
set_intersection(set_t* out, const set_t* a, const set_t* b) {
  const char* x;
  size_t n;
  int r = 0;
  set_iterator_t it;

  set_foreach(a, it, x, n) {
    if(set_has(b, x, n)) {
      set_add(out, x, n);
      ++r;
    }
  }

  return r;
}
