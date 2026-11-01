#include "../scan.h"

int
scan_fromhex(int c) {
  unsigned char u = (unsigned char)c;

  u = (unsigned char)(u - '0');
  if(u <= 9)
    return u;
  u = (unsigned char)(u & ~0x20);
  u = (unsigned char)(u - ('A' - '0'));
  if(u < 6)
    return u + 10;

  return -1;

  /* more readable but creates worse code:

    if(c>='0' && c<='9')
      return c-'0';
    else if(c>='A' && c<='F')
      return c-'A'+10;
    else if(c>='a' && c<='f')
      return c-'a'+10;

    return -1;
  */
}
