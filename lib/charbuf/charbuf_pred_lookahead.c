#include "../stralloc.h"
#include "../charbuf_internal.h"

ssize_t
charbuf_pred_lookahead(charbuf* b, int (*pred)(int, size_t, void*), void* ptr) {
  size_t n = 0;

  for(;;) {
    uint8* v;
    if((v = charbuf_peekn(b, n + 1)) > 0) {
      if(pred((unsigned int)(unsigned char)v[n], n, ptr)) {
        n++;
        continue;
      }
    }
    break;
  }
  return n;
}
