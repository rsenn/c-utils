#include "../charbuf_internal.h"
#include "../alloc.h"

void
charbuf_free(charbuf* b) {
  if(b->chrs)
    alloc_free(b->chrs);
  b->chrs = 0;
  b->a = 0;
  b->p = 0;
}
