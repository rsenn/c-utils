#include "../buffer.h"
#include "../byte.h"

ssize_t
buffer_getn(buffer* b, char* x, size_t n) {
  size_t bytes;

  if((ssize_t)n < 0)
    n = ((size_t)-1) >> 1;

  for(bytes = 0; bytes < n; ++bytes) {
    ssize_t r;

    if((r = buffer_getc(b, x)) < 0)
      return r;

    if(r == 0) 
      break;
   
    ++x;
  }
  return (ssize_t)bytes;
}
