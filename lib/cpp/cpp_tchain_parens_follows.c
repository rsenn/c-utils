#include "../cpp_internal.h"

int
cpp_tchain_parens_follows(cpp* pp, int rec_level) {
  for(int i = rec_level; i >= 0; --i) {
    int c;

    if((c = tokenizer_peek(pp->tchain[i])) == TOKENIZER_EOF)
      continue;

    if(c == '(')
      return i;

    break;
  }

  return -1;
}
