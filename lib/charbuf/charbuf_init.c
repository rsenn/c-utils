#include "../charbuf_internal.h"
#include "../alloc.h"

void
charbuf_init(charbuf* b, read_fn* op, fd_t fd, unsigned int lookahead) {
  b->a = (lookahead + 7) & (~7);
  b->p = 0;
  b->chrs = alloc(b->a);
  b->op = op;
  b->fd = fd;
  b->eof = 0;
  b->err = 0;
  b->offset = 0;
  byte_zero(&b->loc, sizeof(b->loc));
}
