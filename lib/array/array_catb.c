#include "../array.h"
#include "../byte.h"

void
array_catb(array* to, const void* from, uint64 len) {
  int64 l;

  if(!len)
    return;

  if(to->allocated < 0)
    return;

  if(to->initialized + len < to->initialized) {
  fail:
    array_fail(to);
    return;
  }
  l = to->initialized;

  if(!array_allocate(to, 1, to->initialized + len - 1))
    goto fail;
  byte_copy(to->p + l, to->initialized - l, from);
}
