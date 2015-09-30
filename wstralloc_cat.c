#include "wstralloc.h"
#include "wstr.h"

extern int wstralloc_cat(wstralloc* sa, wstralloc* sa2) {
  return wstralloc_catb(sa, sa2->s, sa2->len);
}

