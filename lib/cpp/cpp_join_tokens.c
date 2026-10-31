#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"

/* Concatenates all tokens in `tok` and returns a new string. */
char*
cpp_join_tokens(cpp_token* tok, cpp_token* end) {
  char* buf;
  int pos;
  cpp_token* t;
  /* Compute the length of the resulting token. */
  int len = 1;

  for(t = tok; t != end && t->kind != TK_EOF; t = t->next) {
    if(t != tok && t->has_space)
      len++;

    len += t->len;
  }

  buf = alloc_zero(len);

  /* Copy token texts. */
  pos = 0;

  for(t = tok; t != end && t->kind != TK_EOF; t = t->next) {
    if(t != tok && t->has_space)
      buf[pos++] = ' ';

    str_copyn(buf + pos, t->loc, t->len);
    pos += t->len;
  }

  buf[pos] = '\0';
  return buf;
}
