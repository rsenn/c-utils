#include "../array.h"
#include "../safemult.h"

#ifndef __unlikely
#define __unlikely(x) (x)
#endif

/* I'm not sure I understand what this function is good for */
void
array_truncate(array* x, uint64 membersize, int64 len) {
  uint64 wanted;

  if(__unlikely(len < 0))
    return;

  if(__unlikely(!umult64(membersize, len, &wanted)))
    return;

  if(__unlikely(wanted > x->initialized))
    return;
  x->initialized = wanted;
}
