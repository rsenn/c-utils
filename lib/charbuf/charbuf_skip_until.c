#include "../charbuf.h"

int
charbuf_skip_until(charbuf* b, char c) {
  int r, n = 0;
  for(;;) {
    if((r = charbuf_peek(b)) <= 0) break;
    if(r == c) return n;
    if((r = charbuf_skip(b)) <= 0) break;
    ++n;
  }
  return r;
}

