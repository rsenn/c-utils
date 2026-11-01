#include "../fmt.h"
#include "../uint32.h"
#include <sys/types.h>

void
fmt_oct3(char* dest, unsigned char w) {
  dest[2] = (char)((w & 7) + '0');
  w >>= 3;
  dest[1] = (char)((w & 7) + '0');
  w >>= 3;
  dest[0] = (char)((w & 7) + '0');
}
