#include "../charbuf.h"
#include <errno.h>

ssize_t
charbuf_stubborn_read(charbuf* b) {
  ssize_t w;

if(b->eof || b->err)
    return b->eof ? 0 : -1;
  
  for(;;) {
    if((w = b->op((int)b->fd, &b->ch,1, b)) < 0)
      if(errno == EINTR)
        continue;

      if(w == 0) b->eof =1;
     else if(w == -1) b->err =1;

    break;
  }
  return w;
}
