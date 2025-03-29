#include "../cpp.h"
#include "../cpp_internal.h"

/* Concatenate two tokens to create a new token. */
cpp_token*
cpp_paste(cpp_token* lhs, cpp_token* rhs) {
  /* Paste the two tokens. */
  char* buf = cpp_format("%.*s%.*s", lhs->len, lhs->loc, rhs->len, rhs->loc);

  cpp_file* file = cpp_file_new(lhs->file->name, lhs->file->file_no, buf);

  /* Tokenize the resulting string. */
  cpp_token* tok = cpp_tokenize(file);

  //cpp_file_free(file);

  if(tok->next->kind != TK_EOF)
    cpp_error_tok(lhs, "pasting forms '%s', an invalid token", buf);

  return tok;
}
