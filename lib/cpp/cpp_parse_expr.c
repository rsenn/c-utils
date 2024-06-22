#include "../cpp_internal.h"

int
cpp_parse_expr(tokenizer* t, int rbp, int* err) {
  token tok;
  int left, ret = cpp_parse_skip(t, &tok);

  if(tok.type == TT_EOF)
    return 0;

  left = cpp_parse_unary(t, &tok, err);

  for(;;) {
    ret = cpp_parse_peek(t, &tok);

    if(cpp_bp(tok.type) <= rbp)
      break;

    ret = tokenizer_next(t, &tok);

    if(tok.type == TT_EOF)
      break;

    left = cpp_parse_binary(t, left, &tok, err);
  }

  (void)ret;

  return left;
}
