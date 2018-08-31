#include "../byte.h"
#include "../strlist.h"

void
strlist_join(const strlist* sl, stralloc* sa, char delim) {
  size_t i;
  stralloc_ready(sa, sl->sa.len);
  byte_copy(sa->s, sl->sa.len, sl->sa.s);
  sa->len = sl->sa.len;

  if(sa->s[sa->len - 1] == sl->sep) sa->len--;

  for(i = 0; i < sa->len; ++i) {
    if(sa->s[i] == sl->sep) sa->s[i] = delim;
  }
}
