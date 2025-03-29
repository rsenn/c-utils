#include "../cpp.h"
#include "../cpp_internal.h"

// Replace func-like macro parameters with given arguments.
cpp_token*
cpp_subst(cpp_token* tok, cpp_macro_arg* args) {
  cpp_token head = {};
  cpp_token* cur = &head;

  while(tok->kind != TK_EOF) {
    // "#" followed by a parameter is replaced with stringized actuals.
    if(cpp_equal(tok, "#")) {
      cpp_macro_arg* arg = cpp_find_arg(args, tok->next);
      if(!arg)
        cpp_error_tok(tok->next, "'#' is not followed by a macro parameter");
      cur = cur->next = cpp_stringize(tok, arg->tok);
      tok = tok->next->next;
      continue;
    }

    // [GNU] If __VA_ARG__ is empty, `,##__VA_ARGS__` is expanded
    // to the empty token list. Otherwise, its expaned to `,` and
    // __VA_ARGS__.
    if(cpp_equal(tok, ",") && cpp_equal(tok->next, "##")) {
      cpp_macro_arg* arg = cpp_find_arg(args, tok->next->next);
      if(arg && arg->is_va_args) {
        if(arg->tok->kind == TK_EOF) {
          tok = tok->next->next->next;
        } else {
          cur = cur->next = cpp_copy_token(tok);
          tok = tok->next->next;
        }
        continue;
      }
    }

    if(cpp_equal(tok, "##")) {
      if(cur == &head)
        cpp_error_tok(tok, "'##' cannot appear at start of macro expansion");

      if(tok->next->kind == TK_EOF)
        cpp_error_tok(tok, "'##' cannot appear at end of macro expansion");

      cpp_macro_arg* arg = cpp_find_arg(args, tok->next);
      if(arg) {
        if(arg->tok->kind != TK_EOF) {
          *cur = *cpp_paste(cur, arg->tok);
          for(cpp_token* t = arg->tok->next; t->kind != TK_EOF; t = t->next)
            cur = cur->next = cpp_copy_token(t);
        }
        tok = tok->next->next;
        continue;
      }

      *cur = *cpp_paste(cur, tok->next);
      tok = tok->next->next;
      continue;
    }

    cpp_macro_arg* arg = cpp_find_arg(args, tok);

    if(arg && cpp_equal(tok->next, "##")) {
      cpp_token* rhs = tok->next->next;

      if(arg->tok->kind == TK_EOF) {
        cpp_macro_arg* arg2 = cpp_find_arg(args, rhs);
        if(arg2) {
          for(cpp_token* t = arg2->tok; t->kind != TK_EOF; t = t->next)
            cur = cur->next = cpp_copy_token(t);
        } else {
          cur = cur->next = cpp_copy_token(rhs);
        }
        tok = rhs->next;
        continue;
      }

      for(cpp_token* t = arg->tok; t->kind != TK_EOF; t = t->next)
        cur = cur->next = cpp_copy_token(t);
      tok = tok->next;
      continue;
    }

    // If __VA_ARG__ is empty, __VA_OPT__(x) is expanded to the
    // empty token list. Otherwise, __VA_OPT__(x) is expanded to x.
    if(cpp_equal(tok, "__VA_OPT__") && cpp_equal(tok->next, "(")) {
      cpp_macro_arg* arg = cpp_read_macro_arg_one(&tok, tok->next->next, true);
      if(cpp_has_varargs(args))
        for(cpp_token* t = arg->tok; t->kind != TK_EOF; t = t->next)
          cur = cur->next = t;
      tok = cpp_skip(tok, ")");
      continue;
    }

    // Handle a macro token. cpp_macro arguments are completely macro-expanded
    // before they are substituted into a macro body.
    if(arg) {
      cpp_token* t = cpp_preprocess2(arg->tok);
      t->cpp_at_bol = tok->cpp_at_bol;
      t->cpp_has_space = tok->cpp_has_space;
      for(; t->kind != TK_EOF; t = t->next)
        cur = cur->next = cpp_copy_token(t);
      tok = tok->next;
      continue;
    }

    // Handle a non-macro token.
    cur = cur->next = cpp_copy_token(tok);
    tok = tok->next;
    continue;
  }

  cur->next = tok;
  return head.next;
}
