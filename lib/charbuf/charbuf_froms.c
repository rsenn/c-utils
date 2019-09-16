#include "../charbuf.h"

static ssize_t
charbuf_readstr(fd_t fd, char* buf, size_t len, charbuf* b) {
  char** sptr = (char**)b->ptr;
  *buf = *(*sptr)++;
  return !!*buf;
}

void
charbuf_froms(charbuf* b, char** s) {
  b->p = 0;
  b->ch = '\0';
  b->op = (read_fn*)&charbuf_readstr;
  b->ptr = (void*)s;
}

