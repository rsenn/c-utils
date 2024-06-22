#include "../cpp_internal.h"

/* fetches the next token until it is non-whitespace */
int
cpp_parse_skip(tokenizer* t, token* tok) {
  int ws_count, ret;

  if(!(ret = tokenizer_next(t, tok)))
    return ret;

  ret = cpp_parse_whitespace(t, tok, &ws_count);
  return ret;
}
