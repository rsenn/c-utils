#include "../tokenizer_internal.h"

void
tokenizer_register_custom_token(tokenizer* t,
                                int tokentype,
                                const char* str) {
  int pos = tokentype - TT_CUSTOM;
  assert(tokentype >= TT_CUSTOM &&
         tokentype < TT_CUSTOM + MAX_CUSTOM_TOKENS);

  t->custom_tokens[pos] = str;

  if(pos + 1 > t->custom_count)
    t->custom_count = pos + 1;
}
