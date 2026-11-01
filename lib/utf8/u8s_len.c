#include "../utf8.h"

/* Number of characters in the string u8. A byte that does not start a
 * sequence, or a sequence that ends at the terminator, counts for the 6
 * characters of the entity that stands for it ("&#xFF;"). */
size_t
u8s_len(const char* u8) {
  size_t len = 0;
  char c;

  while((c = *u8)) {
    size_t step = !(c & 0x80) ? 1 : (c & 0xe0) == 0xc0 ? 2 : (c & 0xf0) == 0xe0 ? 3 : (c & 0xf8) == 0xf0 ? 4 : 0, k;

    for(k = 1; k < step; k++)
      if(u8[k] == '\0')
        break;

    if(step == 0 || k < step) {
      len += 6;
      u8 += 1;
    } else {
      len++;
      u8 += step;
    }
  }

  return len;
}
