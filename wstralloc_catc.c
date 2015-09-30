#include "wstralloc.h"

int wstralloc_catc(wstralloc* sa, const unsigned short c) {
  if(wstralloc_ready(sa, sa->len + 1)) {
    sa->s[sa->len] = c;
    sa->len++;
    return 1;
  }
  return 0;
}
