#include "../ucs.h"

/* ucs_tolower is a simple case fold of Latin, Greek and Cyrillic, other
 * code points are returned unchanged. */
uint32
ucs_tolower(uint32 c) {
  if(c < 0x80)
    return c >= 'A' && c <= 'Z' ? c + 32 : c;
  if(c >= 0xc0 && c <= 0xde)
    return c == 0xd7 ? c : c + 32;
  if(c >= 0x100 && c <= 0x17f) {
    if(c == 0x130 || c == 0x131 || c == 0x138 || c == 0x149 || c == 0x17f)
      return c;
    if(c == 0x178)
      return 0xff;
    return ((c >= 0x139 && c <= 0x148) || (c >= 0x179 && c <= 0x17e)) ? ((c & 1) ? c + 1 : c) : ((c & 1) ? c : c + 1);
  }
  if(c == 0x386)
    return 0x3ac;
  if(c >= 0x388 && c <= 0x38a)
    return c + 37;
  if(c == 0x38c)
    return 0x3cc;
  if(c == 0x38e || c == 0x38f)
    return c + 63;
  if((c >= 0x391 && c <= 0x3a1) || (c >= 0x3a3 && c <= 0x3ab))
    return c + 32;
  if(c >= 0x410 && c <= 0x42f)
    return c + 32;
  if(c >= 0x400 && c <= 0x40f)
    return c + 80;
  if((c >= 0x460 && c <= 0x481) || (c >= 0x48a && c <= 0x4bf) || (c >= 0x4d0 && c <= 0x52f))
    return (c & 1) ? c : c + 1;
  if(c >= 0xff21 && c <= 0xff3a)
    return c + 32;
  return c;
}
