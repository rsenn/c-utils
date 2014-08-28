#include "buffer.h"
#include "fmt.h"

int buffer_putulong(buffer *b,size_t l) {
  char buf[FMT_ULONG];
  return buffer_put(b,buf,fmt_ulong(buf,l));
}

