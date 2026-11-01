#include "../fmt.h"
#include "../uint32.h"
#include <sys/types.h>

void
fmt_hex2(char* dest, unsigned char w) {
  dest[2] = fmt_hexchars[w & 0x0f];
  w >>= 4;
  dest[1] = fmt_hexchars[w & 0x0f];
  dest[0] = 'x';
}
