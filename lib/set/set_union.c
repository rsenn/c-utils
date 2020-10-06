#include "../set.h"

int
set_cat(set_t* set, const set_t* other) {
  const char* x;
  size_t n;
  int r = 0;

  set_foreach(other, x, n) {
    if(!set_has(set, x, n)) {
      set_add(set, x, n);
      ++r;
    }
  }
  return r;
}
