#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_read_const_expr(cpp_token** rest, cpp_token* tok) {
  tok = cpp_copy_line(rest, tok);

  cpp_token head = {};
  cpp_token* cur = &head;

  while(tok->kind != TK_EOF) {
    // "defined(foo)" or "defined foo" becomes "1" if macro "foo"
    // is defined. Otherwise "0".
    if(cpp_equal(tok, "defined")) {
      cpp_token* start = tok;
      bool has_paren = consume(&tok, tok->next, "(");

      if(tok->kind != TK_IDENT)
        cpp_error_tok(start, "macro name must be an identifier");
      cpp_macro* m = cpp_find_macro(tok);
      tok = tok->next;

      if(has_paren)
        tok = skip(tok, ")");

      cur = cur->next = cpp_new_num_token(m ? 1 : 0, start);
      continue;
    }

    cur = cur->next = tok;
    tok = tok->next;
  }

  cur->next = tok;
  return head.next;
}
