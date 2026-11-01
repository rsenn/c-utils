/*
DNS should have used LZ77 instead of its own sophomoric compression
algorithm.
*/
#include "../dns.h"
#include <errno.h>

unsigned int
dns_packet_skipname(const char* buf, unsigned int len, unsigned int pos) {
  unsigned char ch;

  for(; pos < len; pos += ch) {
    ch = buf[pos++];

    if(ch >= 192)
      return pos + 1;

    if(ch >= 64)
      break;

    if(!ch)
      return pos;
  }

  errno = EINVAL;
  return 0;
}
