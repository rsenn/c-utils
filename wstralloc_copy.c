#include "wstralloc.h"
#include "wstr.h"

extern int wstralloc_copy(wstralloc* sa, const wstralloc* sa2) {
  return wstralloc_copyb(sa, sa2->s, sa2->len);
}

