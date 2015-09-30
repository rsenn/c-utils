#include <stdlib.h>
#include "wstralloc.h"

void wstralloc_free(wstralloc* sa) {
  if(sa->s) free(sa->s);
  sa->s = 0;
}
