#include "../strlist.h"
#include "../stralloc.h"
#include "../byte.h"

int
strlist_contains_sa(strlist* sl, const stralloc* sa) {
  size_t n, i;

  for(i = 0; i < sl->sa.len; i +=  n + 1) {
    n = str_len(&sl->sa.s[i]);
    if(n == sa->len && byte_equal(&sl->sa.s[i], n, sa->s))
     return 1;
  }
  return 0;
}
