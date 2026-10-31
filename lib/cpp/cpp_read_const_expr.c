#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_read_const_expr(cpp_ctx* pp, cpp_token** rest, cpp_token* tok) {
  cpp_token head = {0};
  cpp_token* cur;
  tok = cpp_copy_line(rest, tok);

  cur = &head;

  while(tok->kind != TK_EOF) {
    /* "defined(foo)" or "defined foo" becomes "1" if macro "foo"
       is defined. Otherwise "0". */
    if(cpp_equal(tok, "defined")) {
      cpp_macro* m;
      cpp_token* start = tok;
      bool has_paren = cpp_consume(&tok, tok->next, "(");

      if(tok->kind != TK_IDENT)
        cpp_error_tok(start, "macro name must be an identifier");
      m = cpp_macro_find(pp, tok);

      cpp_query_undefined(pp, tok, 0);
      tok = tok->next;

      if(has_paren)
        tok = cpp_skip(tok, ")");

      cur = cur->next = cpp_new_num_token(pp, m ? 1 : 0, start);
      continue;
    }

    cur = cur->next = tok;
    tok = tok->next;
  }

  cur->next = tok;
  return head.next;
}
