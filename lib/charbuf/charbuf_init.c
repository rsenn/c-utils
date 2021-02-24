#include "../charbuf.h"
#include "../alloc.h"

void
charbuf_init(charbuf* b, read_fn* op, fd_t fd, unsigned int lookahead) {

  lookahead += 0x7;
  lookahead &= ~0x7;

  b->p = 0;
  b->chrs = alloc_zero((b->a = lookahead));
  b->op = op;
  b->fd = fd;
  b->eof = 0;
  b->err = 0;
  byte_zero(&b->loc, sizeof(b->loc));
}
