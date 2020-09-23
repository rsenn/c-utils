#include "../range.h"

void
range_rotate(range* r, int n) {
  size_t len, count;
  char* x;
  count = r->elem_size * n;

  if(count && (x = alloc(count))) {
    len = r->end - r->start;
    byte_copy(x, count, r->start);
    byte_copy(r->start, len - count, &r->start[count]);
    byte_copy(r->end - count, count, x);
    alloc_free(x);
  }
}
