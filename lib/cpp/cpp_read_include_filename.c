#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

/* Read an #include argument. */
char*
cpp_read_include_filename(cpp_token** rest, cpp_token* tok, bool* is_dquote) {
  /* Pattern 1: #include "foo.h" */
  if(tok->kind == TK_STR) {
    /* A double-quoted filename for #include is a special kind of
       token, and we don't want to interpret any escape sequences in it.
       For example, "\f" in "C:\foo" is not a formfeed character but
       just two non-control characters, backslash and f.
       So we don't want to use token->str. */
    *is_dquote = true;
    *rest = cpp_skip_line(tok->next);
    return str_ndup(tok->loc + 1, tok->len - 2);
  }

  /* Pattern 2: #include <foo.h> */
  if(cpp_equal(tok, "<")) {
    /* Reconstruct a filename from a sequence of tokens between
       "<" and ">". */
    cpp_token* start = tok;

    /* Find closing ">". */
    for(; !cpp_equal(tok, ">"); tok = tok->next)
      if(tok->at_bol || tok->kind == TK_EOF)
        cpp_error_tok(tok, "expected '>'");

    *is_dquote = false;
    *rest = cpp_skip_line(tok->next);
    return cpp_join_tokens(start->next, tok);
  }

  /* Pattern 3: #include FOO
     In this case FOO must be macro-expanded to either
     a single string token or a sequence of "<" ... ">". */
  if(tok->kind == TK_IDENT) {
    cpp_token* tok2 = cpp_preprocess2(cpp_copy_line(rest, tok));
    return cpp_read_include_filename(&tok2, tok2, is_dquote);
  }

  cpp_error_tok(tok, "expected a filename");
  return 0;
}
