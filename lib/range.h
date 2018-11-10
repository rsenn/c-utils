#ifndef RANGE_H
#define RANGE_H 1

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef  struct range {
  char* start;
  char* end;
  size_t elem_size;
} range;

static inline size_t
range_size(const range* r) {
  return (r->end - r->start) / (r->elem_size ? r->elem_size : 1);
}

static inline int
range_empty(const range* r) {
  return r->end == r->start;
}

void range_init(range* r, void* start, size_t n_elem, size_t elem_size);

static inline void* range_start(const range* r) { return r->start; }
static inline void* range_end(const range* r) { return r->end; }

static inline void*
range_index(range* r, size_t i) {
  return r->start + r->elem_size * i;
}
static inline void* range_next(range* r, void* ptr) { return (char*)ptr + r->elem_size; }

#define range_foreach(r, var) for(var = range_start(r); var != range_end(r); var = range_next(r, var))

#ifdef __cplusplus
}
#endif
#endif /* defined RANGE_H */

