#include "../str.h"
#include "../strlist.h"
#include <string.h>

typedef int (cmp_fn_t)(const void *, const void *);

size_t
strlist_sort(strlist* sl) {
  size_t p, l = 0;
  
  char *end, *tmp, **ptrs = calloc(sizeof(char*), (strlist_count(sl) +  1));
  tmp = sl->sa.s;
  end = &sl->sa.s[sl->sa.len];

  for(p = 0; p < sl->sa.len; ++p) {
    char* str = &sl->sa.s[p];
    size_t len = byte_chr(str, sl->sa.len - p, sl->sep);

    ptrs[l++] = str;

    p += len;
  }

  qsort(ptrs, l, sizeof(char*), (cmp_fn_t*)&strcmp);
  stralloc_init(&sl->sa);

 for(p = 0; p < l; ++p) {
   size_t len =  byte_chr(ptrs[p], end - ptrs[p], sl->sep);
   strlist_pushb(sl, ptrs[p], len); 
 }

  free(tmp);
  free(ptrs);
  return l;
}
