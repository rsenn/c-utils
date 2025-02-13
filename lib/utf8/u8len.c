#include "../utf8.h"

int
u8len(const char* u8, size_t count) {
  if(0 == count)
    return 0;

  if(NULL == u8)
    return 0;
  else if(0 == *u8)
    return 0;
  else if(!(*u8 & ~0x7f))
    return 1;
  else if((*u8 & 0xe0) == 0xc0)
    return 2;
  else if((*u8 & 0xf0) == 0xe0)
    return 3;
  else if((*u8 & 0xf8) == 0xf0)
    return 4;
  else /* error */
    return -1;
}
