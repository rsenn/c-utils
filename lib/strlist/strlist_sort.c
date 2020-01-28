#include "../str.h"
#include "../strlist.h"

#include <string.h>
#include <stdlib.h>

typedef int(cmp_fn_t)(const void*, const void*);

static int
sort_cmp(const char** a, const char** b) {
  size_t len, alen = str_len(*a), blen = str_len(*b);
  if(alen != blen)
    return alen - blen;

  len = alen < blen ? alen : blen;

  return byte_diff(*a, len, *b);
}

size_t
strlist_sort(strlist* sl, cmp_fn_t* cmp_fn) {
  size_t p, l = 0;
  char *end, *tmp, **ptrs;

  if(cmp_fn == NULL)
    cmp_fn = &sort_cmp;

  ptrs = calloc(sizeof(char*), (strlist_count(sl) + 1));
  tmp = sl->sa.s;
  end = &sl->sa.s[sl->sa.len];

  for(p = 0; p < sl->sa.len; ++p) {
    char* str = &sl->sa.s[p];
    size_t len = byte_chr(str, sl->sa.len - p, sl->sep);
    str[len] = '\0';
    ptrs[l++] = str;

    p += len;
  }

  qsort(ptrs, l, sizeof(char*), cmp_fn);
  stralloc_init(&sl->sa);

  for(p = 0; p < l; ++p) {
    size_t len = byte_chr(ptrs[p], end - ptrs[p], '\0');
    strlist_pushb(sl, ptrs[p], len);
  }

  free(tmp);
  free(ptrs);
  return l;
}
