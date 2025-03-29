#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_macro_arg*
cpp_read_macro_arg_one(cpp_token** rest, cpp_token* tok, bool read_rest) {
  cpp_token head = {};
  cpp_token* cur = &head;
  int level = 0;

  for(;;) {
    if(level == 0 && cpp_equal(tok, ")"))
      break;
    if(level == 0 && !read_rest && cpp_equal(tok, ","))
      break;

    if(tok->kind == TK_EOF)
      cpp_error_tok(tok, "premature end of input");

    if(cpp_equal(tok, "("))
      level++;
    else if(cpp_equal(tok, ")"))
      level--;

    cur = cur->next = cpp_token_copy(tok);
    tok = tok->next;
  }

  cur->next = cpp_new_eof(tok);

  cpp_macro_arg* arg = alloc_zero(sizeof(cpp_macro_arg));
  arg->tok = head.next;
  *rest = tok;
  return arg;
}
