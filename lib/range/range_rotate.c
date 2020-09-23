#include "../alloc.h"
#include "../range.h"
#include "../byte.h"

void
range_rotate(range* r, int n) {
  char *x, *y;
  size_t num_elements, size, bytes;

  num_elements = range_size(r);

  if(n < 0)
    n = num_elements + n;

  if(n > 0 && n < num_elements) {
    bytes = n * r->elem_size;
    size = r->end - r->start;

    if(bytes && (x = alloc_zero(bytes))) {
      byte_copy(x, bytes, r->start);
      y = &r->start[bytes];

      if(y < r->end)
        byte_copy(r->start, r->end - y, y);
      byte_copy(r->end - bytes, bytes, x);
      alloc_free(x);
    }
  }
}
