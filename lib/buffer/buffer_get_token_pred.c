#include "../buffer.h"

/* Like buffer_get_token, but the token ends when the predicate says so: it is
 * given the text read so far and returns 1 when the token is complete.
 * (The signature is libowfat's; with an argument for the predicate see
 * buffer_get_token_pred_arg.) */
ssize_t
buffer_get_token_pred(buffer* b, char* x, size_t len, string_predicate p) {
  size_t blen;

  for(blen = 0; blen < len; ++blen) {
    ssize_t r;

    if((r = buffer_getc(b, x)) < 0)
      return r;

    if(r == 0)
      break;

    if(p(x - blen, blen + 1))
      break;

    ++x;
  }

  return (ssize_t)blen;
}
