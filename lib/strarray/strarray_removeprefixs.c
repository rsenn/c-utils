#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"

int
strarray_removeprefixs(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);
  size_t prefixlen = str_len(s);
  for(i = 0; i < n; ++i) {
    char** p = array_get(arr, sizeof(char*), i);
    size_t len = str_len(*p);
    stralloc sa;
    sa.s = *p; sa.len = sa.a = len;
    if(stralloc_starts(&sa, s)) {
      byte_copy(sa.s, sa.len - prefixlen, &sa.s[prefixlen]);
      sa.len -= prefixlen;
      stralloc_shrink(&sa);
    }
    stralloc_nul(&sa);
    *p = sa.s;
  }
  return 1;
}
