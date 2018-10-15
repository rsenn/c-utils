#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"

int
strarray_removesuffixs(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);
  size_t slen = str_len(s);
  char** p = array_start(arr);
  for(i = 0; i < n; ++i) {
    size_t len = str_len(*p);
    stralloc sa;
    sa.s = *p; sa.len = len; sa.a = len + 1;
    if(stralloc_endb(&sa, s, slen)) {
      sa.len -= slen;
    }
    stralloc_0(&sa);
    *p++ = sa.s;
  }
  return 1;
}
