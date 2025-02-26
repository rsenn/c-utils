#include "../charbuf_internal.h"
#include "../str.h"

static ssize_t
charbuf_readstr(int fd, void* buf, size_t len, void* b) {
  char* src = ((charbuf*)b)->ptr;
  size_t n;

  if((n = (*src == '\0' ? 0 : str_len(src))) > len)
    n = len;

  if(n) {
    byte_copy(buf, n, src);
    src += n;
    ((charbuf*)b)->ptr = src;
  }
  return n;
}

void
charbuf_froms(charbuf* b, char* s, unsigned lookahead) {
  charbuf_init(b, charbuf_readstr, -1, lookahead);
  b->ptr = (void*)s;
}
