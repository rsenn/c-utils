#include "../cpp.h"
#include "../cpp_internal.h"

/* tokens before newline. This function skips such tokens. */
cpp_token*
cpp_skip_line(cpp_token* tok) {
  if(tok->at_bol)
    return tok;

  cpp_warn_tok(tok, "extra token");

  while(!tok->at_bol && tok->kind != TK_EOF)
    tok = tok->next;

  return tok;
}
