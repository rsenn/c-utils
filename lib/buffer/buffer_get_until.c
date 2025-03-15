#include "../buffer.h"
#include "../byte.h"
#include "../scan.h"

int
buffer_get_until(buffer* b, char* x, size_t n, const char* charset, size_t setlen) {
  size_t bytes;

  for(bytes = 0; bytes < n;) {
    ssize_t r;

    if((r = buffer_getc(b, x)) < 0)
      return r;

    if(r == 0) {
      *x = 0;
      break;
    }

    ++bytes;

    if(byte_chr(charset, setlen, *x++) < setlen)
      break;
  }

  return bytes;
}
