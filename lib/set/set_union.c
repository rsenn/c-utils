#include "../set.h"

int
set_union(set_t* out, const set_t* a, const set_t* b) {
  const char* x;
  size_t n;
  int r = 0;
  set_iterator_t it;

  set_foreach(a, it, x, n) {
    set_add(out, x, n);
    ++r;
  }

  set_foreach(b, it, x, n) {
    if(!set_has(a, x, n)) {
      set_add(out, x, n);
      ++r;
    }
  }

  return r;
}
