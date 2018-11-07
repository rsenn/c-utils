#include "../buffer.h"
#include "../byte.h"
#include "../scan.h"

ssize_t
buffer_get_token_pred(buffer* b, char* x, size_t len, string_predicate p, void* arg) {
  unsigned int blen;

  for(blen = 0; blen < len; ++blen) {
    ssize_t r;
    if((r = buffer_getc(b, x)) < 0) return r;
    if(r == 0) {
      break;
    };
    if(p(x - blen, blen + 1, arg)) {
      break;
    };
    ++x;
  }
  return blen;
}
