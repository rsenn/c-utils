#include "../utf8.h"
#include <assert.h>

int
u8towc(wchar_t* out, const char* u8) {
  int len;

  assert(out);

  if((len = u8len(u8, 1)) < 1)
    return len;

  if(1 == len) {
    *out = u8[0] & 0x7f;
    return len;
  }

  if(2 == len) {
    if((u8[1] & 0xc0) != 0x80) /* error */
      return -1;

    *out = ((u8[0] & 0x1f) << 6) | (u8[1] & 0x3f);
    return 2;
  }

  if(3 == len) {
    if((u8[1] & 0xc0) != 0x80) /* error */
      return -1;

    if((u8[2] & 0xc0) != 0x80) /* error */
      return -1;

    *out = ((u8[0] & 0x0f) << 12) | ((u8[1] & 0x3f) << 6) | (u8[2] & 0x3f);
    return 3;
  }

  if(4 == len) {
    if((u8[1] & 0xc0) != 0x80) /* error */
      return -1;

    if((u8[2] & 0xc0) != 0x80) /* error */
      return -1;

    if((u8[3] & 0xc0) != 0x80) /* error */
      return -1;

    *out = ((u8[0] & 0x07) << 18) | ((u8[1] & 0x3f) << 12) | ((u8[2] & 0x3f) << 6) | (u8[3] & 0x3f);
    return 4;
  }

  /* error */
  return -1;
}
