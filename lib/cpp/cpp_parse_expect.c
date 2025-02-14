#include "../cpp_internal.h"

/* return index of matching item in values array, or -1 on error */
int
cpp_parse_expect(tokenizer* t,
                 enum tokentype tt,
                 const char* const values[],
                 token* token) {
  int ret;

  do {
    if((ret = tokenizer_next(t, token)) == 0 || token->type == TT_EOF)
      goto err;

  } while(token_is_whitespace(token));

  if(token->type != tt) {
  err:
    error("unexpected token", t, token);
    return -1;
  }

  for(int i = 0; values[i]; i++)
    if(!str_diff(values[i], t->buf))
      return i;

  return -1;
}
