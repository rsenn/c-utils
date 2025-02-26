#include "../str.h"
#include "../stralloc.h"
#include <assert.h>

void
stralloc_alloc(stralloc* sa) {
  assert(sa->a == 0);

  if(sa->len == 0 && sa->s)
    sa->len = str_len(sa->s);

  sa->s = str_ndup(sa->s, sa->len);
  sa->a = sa->len + 1;
}
