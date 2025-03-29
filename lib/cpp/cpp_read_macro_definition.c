#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

void
cpp_read_macro_definition(cpp_token** rest, cpp_token* tok) {
  if(tok->kind != TK_IDENT)
    cpp_error_tok(tok, "macro name must be an identifier");

  char* name = str_ndup(tok->loc, tok->len);
  tok = tok->next;

  if(!tok->has_space && cpp_equal(tok, "(")) {
    /* Function-like macro */
    char* va_args_name = NULL;
    cpp_macro_param* params = cpp_read_macro_params(&tok, tok->next, &va_args_name);

    cpp_macro* m = cpp_macro_add(name, false, cpp_copy_line(rest, tok));
    m->params = params;
    m->va_args_name = va_args_name;
  } else {
    /* Object-like macro */
    cpp_macro_add(name, true, cpp_copy_line(rest, tok));
  }
}
