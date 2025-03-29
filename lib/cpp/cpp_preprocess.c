#include "../cpp.h"
#include "../cpp_internal.h"

// Entry point function of the preprocessor.
cpp_token*
cpp_preprocess(cpp_token* tok) {
  tok = cpp_preprocess2(tok);
  if(cond_incl)
    cpp_error_tok(cond_incl->tok, "unterminated conditional directive");
  cpp_convert_pp_tokens(tok);
  cpp_join_adjacent_string_literals(tok);

  for(cpp_token* t = tok; t; t = t->next)
    t->line_no += t->line_delta;

  return tok;
}
