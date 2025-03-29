#include "../cpp_internal.h"
#include <stdlib.h>

/* The definition of the numeric literal at the preprocessing stage
   is more relaxed than the definition of that at the later stages.
   In order to handle that, a numeric literal is tokenized as a
   "pp-number" token first and then converted to a regular number
   token after preprocessing.

   This function converts a pp-number token to a regular number token. */
void
cpp_convert_number(cpp_token* tok) {
  /* Try to parse as an integer constant. */
  if(cpp_convert_int(tok))
    return;

  /* If it's not an integer, it must be a floating point constant. */
  char* end;
  long double val = strtold(tok->loc, &end);

  cpp_type* ty;
  if(*end == 'f' || *end == 'F') {
    ty = cpp_ty_float;
    end++;
  } else if(*end == 'l' || *end == 'L') {
    ty = cpp_ty_ldouble;
    end++;
  } else {
    ty = cpp_ty_double;
  }

  if(tok->loc + tok->len != end)
    cpp_error_tok(tok, "invalid numeric constant");

  tok->kind = TK_NUM;
  tok->fval = val;
  tok->ty = ty;
}
