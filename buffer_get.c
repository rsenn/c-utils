#include "byte.h"
#include "buffer.h"

ssize_t buffer_get(buffer* b,char* x,size_t len) {
  size_t done;
  ssize_t blen;
  done=0;
  while(len) {
    if((blen=buffer_feed(b))<=0) return blen;
    if((size_t) blen>=len)
      blen=len;
    byte_copy(x,blen,b->x+b->p);
    b->p+=blen;
    len-=blen;
    x+=blen;
    done+=blen;
  }
  return done;
}
