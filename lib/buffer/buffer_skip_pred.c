#include "../buffer.h"

int
buffer_skip_pred(buffer* b, int (*pred)(int)) {
  char c;
  int n = 0;
  for(;;) {
    if(buffer_peekc(b, &c) < 0) return -1;
    if(!pred(c)) break;
    buffer_skipc(b);
    ++n;
  }
  return n;
}

