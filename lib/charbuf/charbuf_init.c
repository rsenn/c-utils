#include "../charbuf.h"

void
charbuf_init(charbuf* b, read_fn* op, int64 fd) {
  b->p = 0;
  b->ch = '\0';
  b->op = op;
  b->fd = fd;
}
