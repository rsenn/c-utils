#include "../range.h"

void
range_init(range* r, void* start, size_t n_elem, size_t elem_size) {
  r->start = start;
  r->elem_size = elem_size;
  r->end = r->start + elem_size * n_elem;
}
