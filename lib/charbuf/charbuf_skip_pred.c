#include "../charbuf_internal.h"

ssize_t
charbuf_skip_pred(charbuf* b, int (*pred)(int, size_t, void*), void* ptr) {
  size_t n = 0;
  uint8 ch;
  for(;;) {
    if(charbuf_peekc(b, &ch) > 0 && pred((unsigned int)(unsigned char)ch, n, ptr)) {
      ++n;
      if(charbuf_skip(b) > 0)
        continue;
    }
    break;
  }
  return n;
}
