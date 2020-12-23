#include "../cpp_internal.h"

 int
cpp_tchain_parens_follows(struct cpp* cpp, int rec_level) {
  int i, c = 0;
  for(i = rec_level; i >= 0; --i) {
    c = tokenizer_peek(cpp->tchain[i]);
    if(c == EOF)
      continue;
    if(c == '(')
      return i;
    else
      break;
  }
  return -1;
}
