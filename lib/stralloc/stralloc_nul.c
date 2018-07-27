#include "../stralloc.h"

int
stralloc_nul(stralloc* sa) {
  if(sa->len > 0 && sa->s[sa->len - 1] == '\0')
    return 0;

  if(!stralloc_ready(sa, sa->len + 1))
    return 0;

  sa->s[sa->len] = '\0';
  return 1;
}
