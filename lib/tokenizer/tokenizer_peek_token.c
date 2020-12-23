#include "../tokenizer_internal.h"

int
tokenizer_peek_token(tokenizer* t, struct token_s* tok) {
  int ret = tokenizer_next(t, tok);
  t->peek_token = *tok;
  t->peeking = 1;
  return ret;
}
