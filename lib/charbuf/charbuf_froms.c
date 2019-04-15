#include "../charbuf.h"

static int
charbuf_readstr(fd_t fd, char* buf, size_t len, charbuf* b) {
  char** sptr = (char**)b->ptr;
  *buf = *(*sptr)++;
  return !!*buf;
}

void
charbuf_froms(charbuf* b, char** s) {
  b->p = 0;
  b->ch = '\0';
  b->op = charbuf_readstr;
  b->ptr = (void*)s;
}
