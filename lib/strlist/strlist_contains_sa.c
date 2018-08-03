#include "../strlist.h"
#include "../stralloc.h"
#include "../byte.h"

int
strlist_contains_sa(strlist* sl, const stralloc* sa) {
  size_t n, i;

  for(i = 0; i < sl->sa.len; i +=  n + 1) {
    n = str_len(&sl->sa.s[i]);

    buffer_put(buffer_2, &sl->sa.s[i], n);
    buffer_puts(buffer_2, " == ");
    buffer_putsa(buffer_2, sa);
    buffer_putnlflush(buffer_2);

    if(n == sa->len && byte_equal(&sl->sa.s[i], n, sa->s))
     return 1;
  }
  return 0;
}
