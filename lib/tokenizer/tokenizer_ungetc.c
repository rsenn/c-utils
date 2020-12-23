#include "../tokenizer_internal.h"

int
tokenizer_ungetc(tokenizer* t, int c) {
  ++t->chb.buffered;
  assert(t->chb.buffered < countof(t->chb.buf));
  assert(t->chb.cnt > 0);
  --t->chb.cnt;
  assert(t->chb.buf[t->chb.cnt % countof(t->chb.buf)] == c);
  return c;
}
