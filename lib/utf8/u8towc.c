#include "../utf8.h"

int
u8towc(wchar_t* out, const char* u8) {
  int len;
  /* assert */

  if(NULL == out)
    return -1;

  len = u8len(u8, 1);

  if(len < 1)
    return len;
  else if(1 == len) {
    out[0] = u8[0] & 0x7f;
    return len;
  } else if(2 == len) {
    if((u8[1] & 0xc0) != 0x80) /* error */
      return -1;
    out[0] = ((u8[0] & 0x1f) << 6) | (u8[1] & 0x3f);
    return 2;
  } else if(3 == len) {
    if((u8[1] & 0xc0) != 0x80) /* error */
      return -1;
    if((u8[2] & 0xc0) != 0x80) /* error */
      return -1;
    out[0] = ((u8[0] & 0x0f) << 12) | ((u8[1] & 0x3f) << 6) | (u8[2] & 0x3f);
    return 3;
  } else if(4 == len) {
    if((u8[1] & 0xc0) != 0x80) /* error */
      return -1;
    if((u8[2] & 0xc0) != 0x80) /* error */
      return -1;
    if((u8[3] & 0xc0) != 0x80) /* error */
      return -1;
    out[0] = ((u8[0] & 0x07) << 18) | ((u8[1] & 0x3f) << 12) | ((u8[2] & 0x3f) << 6) | (u8[3] & 0x3f);
    return 4;
  } else /* error */
    return -1;
}
