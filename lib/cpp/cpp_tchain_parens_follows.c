#include "../cpp_internal.h"

int
cpp_tchain_parens_follows(cpp* pp, int rec_level) {
  int i, c = 0;

  for(i = rec_level; i >= 0; --i) {
    c = tokenizer_peek(pp->tchain[i]);
    if(c == TOKENIZER_EOF)
      continue;
    if(c == '(')
      return i;
    else
      break;
  }

  return -1;
}
