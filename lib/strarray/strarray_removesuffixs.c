#include "../strarray.h"
#include "../stralloc.h"
#include "../str.h"

int
strarray_removesuffixs(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);
  size_t slen = str_len(s);
char** p = array_start(arr);
  for(i = 0; i <n; ++i) {
size_t len =  str_len(p[i]);
    stralloc sa = { p[i], len, len };
    if(stralloc_ends(&sa, s)) {
      sa.len -= slen;
    }
    stralloc_nul(&sa);
    *p = sa.s;
  }
  return 1;
}
