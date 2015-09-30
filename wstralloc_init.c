#include "wstralloc.h"

void wstralloc_init(wstralloc* sa) {
  sa->s = 0;
  sa->len = sa->a = 0;
}
