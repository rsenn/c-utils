#include "fmt.h"

/* write int in least amount of bytes, return number of bytes */
/* as used in varints from Google protocol buffers */
size_t fmt_varint(char* dest,unsigned long long l) {
  /* high bit says if more bytes are coming, lower 7 bits are payload; little endian */
  size_t i;
  for (i=0; l; ++i, l>>=7) {
    if (dest) dest[i]=(l&0x7f) | ((!!(l&~0x7f))<<7);
  }
  return i;
}

size_t fmt_pb_type0_int(char* dest,unsigned long long l) __attribute__((alias("fmt_varint")));
