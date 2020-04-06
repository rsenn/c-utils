
#include "../strarray.h"

void
strarray_union(const strarray* s1, const strarray* s2, strarray* out) {
  size_t i = 0, j = 0, k = 0, common = 0;
  size_t size1 = strarray_size(s1), size2 = strarray_size(s2);
  const char **a = array_start(s1), **b = array_start(s2), **c;

  array_allocate(out, sizeof(char*), (size1 + size2) - 1);
  c = array_start(out);

  for(k = 0; k < (size1 + size2); k++) {
    int delta = str_diff(a[i], b[j]);

    if(i >= size1) {
      c[k] = b[j++];
    } else if(j >= size2) {
      c[k] = a[i++];
    } else if(delta < 0) {
      c[k] = a[i++];
    } else if(delta > 0) {
      c[k] = b[j++];
    } else {
      c[k] = a[i++];
      j++;
      common++;
    }
  }
  array_truncate(out, sizeof(char*), k - common);
}
