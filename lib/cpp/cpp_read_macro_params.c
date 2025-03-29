#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"

cpp_macro_param*
cpp_read_macro_params(cpp_token** rest, cpp_token* tok, char** va_args_name) {
  cpp_macro_param head = {};
  cpp_macro_param* cur = &head;

  while(!cpp_equal(tok, ")")) {
    if(cur != &head)
      tok = skip(tok, ",");

    if(cpp_equal(tok, "...")) {
      *va_args_name = "__VA_ARGS__";
      *rest = skip(tok->next, ")");
      return head.next;
    }

    if(tok->kind != TK_IDENT)
      cpp_error_tok(tok, "expected an identifier");

    if(cpp_equal(tok->next, "...")) {
      *va_args_name = str_ndup(tok->loc, tok->len);
      *rest = skip(tok->next->next, ")");
      return head.next;
    }

    cpp_macro_param* m = alloc_zero(sizeof(cpp_macro_param));
    m->name = str_ndup(tok->loc, tok->len);
    cur = cur->next = m;
    tok = tok->next;
  }

  *rest = tok->next;
  return head.next;
}
