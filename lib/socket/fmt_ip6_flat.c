#include "../fmt.h"
#include "../ip6.h"

size_t
fmt_ip6_flat(char* s, const char ip[16]) {
  int i;
  if(!s) return 32;
  for(i = 0; i < 16; i++) {
    *s++ = fmt_tohex((char)((unsigned char)ip[i] >> 4));
    *s++ = fmt_tohex((unsigned char)ip[i] & 15);
  }
  return 32;
}
