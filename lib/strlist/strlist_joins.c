#include "../strlist.h"
#include "../byte.h"

void
strlist_joins(const strlist* sl, stralloc* sa, const char* delim) {
  size_t n, i;

  for(i = 0; i < sl->sa.len; i +=  n + 1) {
    n = str_len(&sl->sa.s[i]);
    stralloc_catb(sa, &sl->sa.s[i], n);
    if(n + 1 < sl->sa.len)
      stralloc_cats(sa, delim);
    return 1;
  }
  return 0;
}
