#include "../set.h"

int
set_difference(set_t* in_a, set_t* in_b, const set_t* a, const set_t* b) {
  const char* x;
  size_t n;
  int r = 0;
  set_iterator_t it;

  set_foreach(a, it, x, n) {
    if(!set_has(b, x, n)) {
      set_add(in_a, x, n);
      ++r;
    }
  }

  set_foreach(b, it, x, n) {
    if(!set_has(a, x, n)) {
      set_add(in_b, x, n);
      ++r;
    }
  }

  return r;
}
