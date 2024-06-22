#include "../cpp_internal.h"

int
cpp_parse_peek(tokenizer* t, token* tok) {
  int ret;

  for(;;) {
    ret = tokenizer_peek_token(t, tok);

    if(!token_is_whitespace(tok))
      break;

    x_tokenizer_next(t, tok);
  }

  return ret;
}
