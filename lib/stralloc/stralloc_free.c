#include <stdlib.h>
#include "../stralloc.h"

void stralloc_free(stralloc* sa) {
  if(sa->s) {
    if(sa->a)
      free(sa->s);
  }
  sa->s = 0;
}
