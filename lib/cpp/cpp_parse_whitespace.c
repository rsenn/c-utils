#include "../cpp_internal.h"

/* skips until the next non-whitespace token (if the current one is one too)*/
int
cpp_parse_whitespace(tokenizer* t, token* token, int* count) {
  int ret = 1;

  *count = 0;

  while(token_is_whitespace(token)) {
    ++(*count);

    if(!(ret = x_tokenizer_next(t, token)))
      break;
  }

  return ret;
}
