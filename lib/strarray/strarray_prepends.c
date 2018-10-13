#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"

int
strarray_prepends(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);
  for(i = 0; i < n; ++i) {
    char** p = array_get(arr, sizeof(char*), i);
    size_t len = str_len(*p);
    stralloc sa = {*p, len, len};
    if(!stralloc_prepend(&sa, s)) return 0;
    stralloc_nul(&sa);
    *p = sa.s;
  }
  return 1;
}
