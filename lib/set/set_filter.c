#include "../set.h"

static int
set_filter_magic(const set_t* in,
                 set_t* out,
                 int (*pred)(const char*, size_t),
                 int magic) {
  const char* x;
  size_t n;
  int r = 0;
  set_iterator_t it;

  set_foreach(in, it, x, n) {
    if(magic == pred(x, n)) {
      set_add(out, x, n);
      ++r;
    }
  }
  return r;
}

int
set_filter(const set_t* in, set_t* out, int (*pred)(const char*, size_t)) {
  return set_filter_magic(in, out, pred, 1);
}
int
set_filter_out(const set_t* in,
               set_t* out,
               int (*pred)(const char*, size_t)) {
  return set_filter_magic(in, out, pred, 0);
}