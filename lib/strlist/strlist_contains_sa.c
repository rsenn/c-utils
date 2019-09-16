#include "../byte.h"
#include "../stralloc.h"
#include "../strlist.h"

int
strlist_contains_sa(strlist* sl, const stralloc* sa) {
  size_t n, i;

  for(i = 0; i < sl->sa.len; i += n + 1) {
    n = byte_chr(&sl->sa.s[i], sl->sa.len - i, sl->sep);
    if(n == sa->len && byte_equal(&sl->sa.s[i], n, sa->s))
      return 1;
  }
  return 0;
}
