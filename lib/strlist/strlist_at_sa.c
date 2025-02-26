#include "../stralloc.h"
#include "../strlist.h"

/*stralloc
strlist_at_sa(const strlist* sl, size_t i) {
  stralloc sa;
  size_t p, l = 0;
  stralloc_init(&sa);

  for(p = 0; p < sl->sa.len; ++p) {
    if(l == i) {
      sa.s = &sl->sa.s[p];
      sa.len = byte_chr(&sl->sa.s[p], sl->sa.len - p, sl->sep);
      sa.a = 0;
      return sa;
    }

    if(sl->sa.s[p] == sl->sep)
      ++l;
  }
  return sa;
}*/

size_t
strlist_at_sa(const strlist* sl, stralloc* out, size_t i) {
  char* x;
  size_t len;

  if((x = strlist_at_n(sl, i, &len)))
    stralloc_copyb(out, x, len);
  return len;
}
