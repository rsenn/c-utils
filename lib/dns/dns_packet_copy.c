/*
DNS should have used LZ77 instead of its own sophomoric compression
algorithm.
*/
#include "../dns.h"
#include <errno.h>

unsigned int
dns_packet_copy(const char* buf, unsigned int len, unsigned int pos, char* out, unsigned int outlen) {
  while(outlen) {
    if(pos >= len) {
      errno = EINVAL;
      return 0;
    }
    *out = buf[pos++];
    ++out;
    --outlen;
  }
  return pos;
}
