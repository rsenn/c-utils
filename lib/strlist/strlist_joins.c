#include "../byte.h"
#include "../strlist.h"

void
strlist_joins(const strlist* sl, stralloc* sa, const char* delim) {
  size_t n, i;

  for(i = 0; i < sl->sa.len; i += n + 1) {
    n = byte_chr(&sl->sa.s[i], sl->sa.len - i, sl->sep);

    if(i > 0)
      stralloc_cats(sa, delim);
    stralloc_catb(sa, &sl->sa.s[i], n);
  }
}
