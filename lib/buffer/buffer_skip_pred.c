#include "../buffer.h"

int
buffer_skip_pred(buffer* b, int (*pred)(int)) {
  int n = 0;
  for(;;) {
    ssize_t r;
    char c;
    if((r = buffer_peekc(b, &c)) <= 0) {
      n = r;
      break;
    }
    if(!pred(c))
      break;
    buffer_skipc(b);
    ++n;
  }
  return n;
}
