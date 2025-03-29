#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_macro_arg*
cpp_read_macro_args(cpp_token** rest, cpp_token* tok, cpp_macro_param* params, char* va_args_name) {
  cpp_token* start = tok;
  tok = tok->next->next;

  cpp_macro_arg head = {};
  cpp_macro_arg* cur = &head;
  cpp_macro_param* pp = params;

  for(; pp; pp = pp->next) {
    if(cur != &head)
      tok = cpp_skip(tok, ",");

    cur = cur->next = cpp_read_macro_arg_one(&tok, tok, false);
    cur->name = pp->name;
  }

  if(va_args_name) {
    cpp_macro_arg* arg;

    if(cpp_equal(tok, ")")) {
      arg = alloc_zero(sizeof(cpp_macro_arg));
      arg->tok = cpp_new_eof(tok);
    } else {
      if(pp != params)
        tok = cpp_skip(tok, ",");
      arg = cpp_read_macro_arg_one(&tok, tok, true);
    }

    arg->name = va_args_name;
    ;
    arg->is_va_args = true;
    cur = cur->next = arg;
  } else if(pp) {
    cpp_error_tok(start, "too many arguments");
  }

  cpp_skip(tok, ")");
  *rest = tok;
  return head.next;
}
