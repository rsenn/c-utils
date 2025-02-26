#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"

int
strarray_appends(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);
  char** p = strarray_BEGIN(arr);

  for(i = 0; i < n; ++i) {
    size_t len = str_len(*p);
    stralloc sa;
    sa.s = *p;
    sa.len = sa.a = len;

    if(!stralloc_cats(&sa, s))
      return 0;
    stralloc_nul(&sa);
    *p++ = sa.s;
  }

  return 1;
}
