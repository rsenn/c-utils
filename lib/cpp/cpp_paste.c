#include "../cpp.h"
#include "../cpp_internal.h"

// Concatenate two tokens to create a new token.
cpp_token*
cpp_paste(cpp_token* lhs, cpp_token* rhs) {
  // Paste the two tokens.
  char* buf = cpp_format("%.*s%.*s", lhs->len, lhs->loc, rhs->len, rhs->loc);

  // Tokenize the resulting string.
  cpp_token* tok = tokenize(cpp_new_file(lhs->file->name, lhs->file->file_no, buf));
  if(tok->next->kind != TK_EOF)
    cpp_error_tok(lhs, "pasting forms '%s', an invalid token", buf);
  return tok;
}
