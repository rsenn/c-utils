#include "../cpp_internal.h"

void
cpp_convert_tokens(cpp_token* tok) {
  for(cpp_token* t = tok; t->kind != TK_EOF; t = t->next) {
    if(cpp_is_keyword(t))
      t->kind = TK_KEYWORD;
    else if(t->kind == TK_PP_NUM)
      cpp_convert_number(t);
  }
}
