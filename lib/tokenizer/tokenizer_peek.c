#include "../tokenizer_internal.h"

int
tokenizer_peek(tokenizer* t) {
  int ret;

  if(t->peeking)
    return t->peek_token.value;

  if((ret = tokenizer_getc(t)) != TOKENIZER_EOF)
    tokenizer_ungetc(t, ret);

  return ret;
}
