#include "../buffer.h"
#include "../byte.h"
#include "../scan.h"

ssize_t
buffer_get_token_pred(buffer* b, char* x, size_t n, string_predicate p, void* arg) {
  size_t bytes;

  for(bytes = 0; bytes < n; ++bytes) {
    ssize_t r;

    if((r = buffer_getc(b, x)) < 0)
      return r;

    if(r == 0)
      break;

    if(p(x - bytes, bytes + 1, arg))
      break;

    ++x;
  }
  
  return bytes;
}
