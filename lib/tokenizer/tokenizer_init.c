#include "../tokenizer_internal.h"
#include "../byte.h"

void
tokenizer_init(tokenizer* t, buffer* in, int flags) {
  byte_zero(t, sizeof(tokenizer));

  t->input = in;
  t->line = 1;
  t->flags = flags;
  t->bufsize = MAX_TOK_LEN;
}
