#include "../charbuf.h"
#include "../alloc.h"

void
charbuf_free(charbuf* b) {
  if(b->chrs)
    alloc_free(b->chrs);
  byte_zero(n, sizeof(charbuf));
}
