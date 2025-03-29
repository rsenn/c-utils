#include "../cpp.h"
#include "../cpp_internal.h"

// Read and evaluate a constant expression.
long
cpp_eval_const_expr(cpp_token** rest, cpp_token* tok) {
  cpp_token* start = tok;
  cpp_token* expr = cpp_read_const_expr(rest, tok->next);
  expr = cpp_preprocess2(expr);

  if(expr->kind == TK_EOF)
    cpp_error_tok(start, "no expression");

  // [https://www.sigbus.info/n1570#6.10.1p4] The standard requires
  // we replace remaining non-macro identifiers with "0" before
  // evaluating a constant expression. For example, `#if foo` is
  // equivalent to `#if 0` if foo is not defined.
  for(cpp_token* t = expr; t->kind != TK_EOF; t = t->next) {
    if(t->kind == TK_IDENT) {
      cpp_token* next = t->next;
      *t = *cpp_new_num_token(0, t);
      t->next = next;
    }
  }

  // Convert pp-numbers to regular numbers
  cpp_convert_tokens(expr);

  cpp_token* rest2;
  long val = const_expr(&rest2, expr);
  
  if(rest2->kind != TK_EOF)
    cpp_error_tok(rest2, "extra token");

  return val;
}
