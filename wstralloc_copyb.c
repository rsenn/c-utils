#include "byte.h"
#include "wstralloc.h"

/* wstralloc_copyb copies the wstring buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, wstralloc_copyb leaves sa alone and returns 0. */
int wstralloc_copyb(wstralloc* sa, const wchar_t* buf, size_t len) {
  if(wstralloc_ready(sa, len)) {
    sa->len = len;
    word_copy(sa->s, len, buf);
    return 1;
  }
  return 0;
}
