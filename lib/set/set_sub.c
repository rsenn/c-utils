#include "../set.h"

int
set_sub(const set_t* a, const set_t* b, set_t* c) {
  const char* x;
  size_t n;
  int r = 0;
  set_foreach(a, x, n) {
    if(!set_has(b, x, n)) {
      set_add(c, x, n);
      ++r;
    }
  }
  return r;
}
