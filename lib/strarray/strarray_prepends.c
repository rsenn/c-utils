#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"

int
strarray_prepends(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);

  for(i = 0; i < n; ++i) {
    char** p = (char**)array_get((array*)arr, sizeof(char*), i);
    size_t len = str_len(*p);
    stralloc sa;
    sa.s = *p;
    sa.len = sa.a = len;

    if(!stralloc_prepends(&sa, s))
      return 0;
    stralloc_nul(&sa);
    *p = sa.s;
  }
  return 1;
}
