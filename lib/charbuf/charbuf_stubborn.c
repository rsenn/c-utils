#include "../charbuf.h"
#include <errno.h>

ssize_t
charbuf_stubborn_read(charbuf* b) {
  ssize_t ret;

  if(b->eof)
    return 0;
  if(b->err)
    return -1;

  for(;;) {
    ret = b->op((int)b->fd, &b->ch, 1, b);

    if(ret < 0 && errno == EINTR) {
      errno = 0;
      continue;
    }
    if(ret < 0)
      b->err = 1;
    else if(ret == 0)
      b->eof = 1;
    break;
  }
  return ret;
}
