#include "../charbuf.h"

int
charbuf_skip_pred(charbuf* b, int (*pred)(int)) {
  int r, n = 0;
  
  for(;;) {
    if((r = charbuf_peek(b)) <= 0)
      break;
    if(!pred((char)r))
      return n;
    if((r = charbuf_skip(b)) <= 0)
      break;
    ++n;
  }
  return n;
}
