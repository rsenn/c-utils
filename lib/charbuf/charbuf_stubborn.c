#include "../charbuf.h"
#include <errno.h>

ssize_t
charbuf_stubborn_read(charbuf* b, size_t max) {
  ssize_t ret;
  size_t n;

  if(b->eof)
    return 0;
  if(b->err)
    return -1;

  for(;;) {
    n = b->a - b->p;
    if(n >= max)
      n = max;

    ret = b->op((int)b->fd, &b->chrs[b->p], n, b);

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
