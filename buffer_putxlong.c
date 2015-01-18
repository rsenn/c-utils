#include "buffer.h"
#include "fmt.h"

int buffer_putxlong(buffer *b,size_t l) {
  char buf[FMT_XLONG];
  return buffer_put(b,buf,fmt_xlong(buf,l));
}

