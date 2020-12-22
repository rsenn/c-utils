#include "../tokenizer_internal.h"

int
tokenizer_ungetc(tokenizer* t, int c) {
  ++t->getc_buf.buffered;
  assert(t->getc_buf.buffered < countof(t->getc_buf.buf));
  assert(t->getc_buf.cnt > 0);
  --t->getc_buf.cnt;
  assert(t->getc_buf.buf[t->getc_buf.cnt % countof(t->getc_buf.buf)] == c);
  return c;
}
