#include "../stralloc.h"
#include "../buffer.h"
#include "../byte.h"

int
buffer_getln(buffer* b, stralloc* sa, int* gotit, int termchar) {
  if(!stralloc_ready(sa, 0))
    return -1;
  sa->len = 0;

  for(;;) {
    int r;
    char* p;
    int off;
    r = buffer_feed(b);

    if(r < 0)
      return -1;

    if(r == 0) {
      *gotit = 0;
      return 0;
    }
    p = buffer_PEEK(b);
    off = byte_chr(p, r, termchar);

    if(off != r)
      r = off + 1; /* byte_chr returns index, not length */

    if(!stralloc_catb(sa, p, r))
      return -1;

    buffer_SKIP(b, r);

    if(off != r) {
      *gotit = 1;
      return 0;
    }
  }
}
