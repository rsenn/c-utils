#include "../cpp_internal.h"

/* fetches the next token until it is non-whitespace */
int
cpp_parse_skip(tokenizer* t, token* tok) {
  int ws;

  if(tokenizer_next(t, tok))
    return cpp_parse_whitespace(t, tok, &ws);

  return 0;
}
