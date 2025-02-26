#include "../stralloc.h"
#include "../charbuf_internal.h"

ssize_t
charbuf_pred_get(charbuf* b, stralloc* sa, int (*pred)(int, size_t, void*), void* ptr) {
  ssize_t ret;
  stralloc_zero(sa);
  stralloc_ready(sa, 2);

  for(;;) {
    unsigned char c;

    if((ret = charbuf_peekc(b, &c)) > 0 && pred((unsigned int)(unsigned char)c, sa->len, ptr)) {
      stralloc_CATC(sa, c);

      if(charbuf_skip(b) > 0)
        continue;
    }
    break;
  }
  stralloc_nul(sa);
  return sa->len;
}
