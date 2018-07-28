#include "../charbuf.h"

int
charbuf_getc(charbuf* b, char* ch) {
  int ret = charbuf_get(b);
  if(ret >= 0) {
    *ch = ret;
    ret = 1;
  }
  return ret;
}
