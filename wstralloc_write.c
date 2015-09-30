#include "byte.h"
#include "wstralloc.h"

/* wstralloc_catb adds the wstring buf[0], buf[1], ... buf[len - 1] to the
 * end of the wstring stored in sa, allocating space if necessary, and
 * returns len. If sa is unallocated, wstralloc_catb is the same as
 * wstralloc_copyb. If it runs out of memory, wstralloc_catb leaves sa
 * alone and returns 0. */
int wstralloc_write(wstralloc* sa, const wchar_t* buf, size_t len) {
  if(wstralloc_readyplus(sa, len)) {
    word_copy(sa->s + sa->len, len, buf);
    sa->len += len;
    return len;
  }
  return 0;
}
