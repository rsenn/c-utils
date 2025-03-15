#include "../buffer.h"
#include "../byte.h"
#include "../scan.h"

#include <string.h> /* for memccpy */

ssize_t
buffer_get_token(buffer* b, char* x, size_t n, const char* charset, size_t setlen) {
  size_t bytes;
  ssize_t r;

  if((ssize_t)n < 0)
    n = (ssize_t)(((size_t)-1) >> 1);

  if(setlen == 1) {
    for(bytes = 0; bytes < n;) {
      ssize_t i;
      char* d;

      if((r = buffer_feed(b)) <= 0)
        return r < 0 ? -1 : bytes;

      if(r > (ssize_t)(n - bytes))
        r = n - bytes;

      if((i = byte_ccopy(x + bytes, r, buffer_PEEK(b), (unsigned char)charset[0])) < r) {
        d = x + bytes + i;

        /* memccpy returns a pointer to the next char after matching
         * char or NULL if it copied all r it was asked for */
        buffer_SKIP(b, i);

        return d - x;
      }

      bytes += r;
      buffer_SKIP(b, r);
    }
  } else {
    for(bytes = 0; bytes < n; ++bytes) {

      if((r = buffer_getc(b, x)) < 0)
        return r;

      if(r == 0) {
        *x = 0;
        break;
      }

      if(byte_chr(charset, setlen, *x) < setlen)
        break;

      ++x;
    }
  }

  return (ssize_t)bytes;
}
