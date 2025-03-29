#include "../scan.h"

int
scan_fromhex(int c) {
  if(c >= 'a' && c <= 'z')
    c -= 'a' - 'A';

  c -= '0';

  if(c <= 9)
    return c;

  c -= 'A' - '0';

  if(c >= 0 && c < 6)
    return c + 10;

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
