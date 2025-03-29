#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_new_eof(cpp_token* tok) {
  cpp_token* t;

  if((t = cpp_token_copy(tok))) {
    t->kind = TK_EOF;
    t->len = 0;
  }

  return t;
}
