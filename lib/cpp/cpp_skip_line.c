#include "../cpp.h"
#include "../cpp_internal.h"

// tokens before newline. This function skips such tokens.
cpp_token*
cpp_skip_line(cpp_token* tok) {
  if(tok->cpp_at_bol)
    return tok;
  warn_tok(tok, "extra token");
  while(tok->cpp_at_bol)
    tok = tok->next;

  return tok;
}
