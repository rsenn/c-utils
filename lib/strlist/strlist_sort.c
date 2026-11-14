#define _GNU_SOURCE
#include "../str.h"
#include "../strlist.h"

#include <string.h>
#include <stdlib.h>

static int
sort_cmp(const void* a, const void* b, void* arg) {
  strlist_cmpfn_t* fn = (strlist_cmpfn_t*)&str_diff; // arg;
  return fn(*(const char**)a, *(const char**)b);
}

size_t
strlist_sort(strlist* sl, strlist_cmpfn_t* cmp_fn) {
  size_t p, l = 0;
  char *tmp, **ptrs;

  stralloc_readyplus(&sl->sa, 1);

  if((p = strlist_count(sl)) <= 1)
    return p;

  ptrs = (char**)malloc(sizeof(char*) * p);
  tmp = sl->sa.s;

  for(p = 0; p < sl->sa.len; ++p) {
    char* str = &sl->sa.s[p];
    size_t len = byte_chr(str, sl->sa.len - p, sl->sep);

    str[len] = '\0';
    ptrs[l++] = str;
    p += len;
  }

  qsort(ptrs, l, sizeof(char*), (strlist_cmpfn_t*)(void*)&sort_cmp);

  sl->sa.s = 0;
  stralloc_init(&sl->sa);

  for(p = 0; p < l; ++p)
    strlist_push(sl, ptrs[p]);

  free(tmp);
  free(ptrs);
  return l;
}
