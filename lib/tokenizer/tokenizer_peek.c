#include "../tokenizer_internal.h"

int
tokenizer_peek(tokenizer* t) {
  if(t->peeking)
    return t->peek_token.value;
  int ret = tokenizer_getc(t);
  if(ret != EOF)
    tokenizer_ungetc(t, ret);
  return ret;
}
