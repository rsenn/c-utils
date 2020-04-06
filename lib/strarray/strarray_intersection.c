#include "../strarray.h"

void
strarray_intersection(const strarray* s1, const strarray* s2, strarray* out) {
  const char **a = array_start(s1), **b = array_start(s2);
  size_t alen = strarray_size(s1), blen = strarray_size(s2);
  size_t i = 0, j = 0;
  while(i < alen && j < blen) {
    int delta = str_diff(a[i], b[j]);
    if(delta < 0) {
      i += 1;
    } else if(delta > 0) {
      j += 1;
    } else {
      // A[i] == B[j]
      strarray_push(out, a[i]);
      i += 1;
      j += 1;
    }
  }
}