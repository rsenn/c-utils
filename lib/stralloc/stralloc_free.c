#include "../alloc.h"
#include "../stralloc.h"

void
stralloc_free(stralloc* sa) {
  if(sa->s) {
    if(sa->a)
      alloc_free(sa->s);
  }
  sa->s = 0;
}
