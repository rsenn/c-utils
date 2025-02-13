#include "../utf8.h"
#include <assert.h>

int
u8len(const char* u8, size_t count) {
  if(0 == count)
    return 0;

  assert(u8);

  if(0 == *u8)
    return 0;

  if(!(*u8 & ~0x7f))
    return 1;

  if((*u8 & 0xe0) == 0xc0)
    return 2;

  if((*u8 & 0xf0) == 0xe0)
    return 3;

  if((*u8 & 0xf8) == 0xf0)
    return 4;

  /* error */
  return -1;
}
