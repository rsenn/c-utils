#include "../utf8.h"

/* Number of characters in the n bytes at u8. A byte that does not start a
 * sequence, or a sequence that the bytes do not hold all of, counts for the
 * 6 characters of the entity that stands for it ("&#xFF;"). */
size_t
u8b_len(const char* u8, size_t n) {
  size_t i;
  size_t len = 0;

  for(i = 0; i < n;) {
    const char c = u8[i];
    size_t step = !(c & 0x80) ? 1 : (c & 0xe0) == 0xc0 ? 2 : (c & 0xf0) == 0xe0 ? 3 : (c & 0xf8) == 0xf0 ? 4 : 0;

    if(step == 0 || i + step > n) {
      len += 6;
      i += 1;
    } else {
      len++;
      i += step;
    }
  }

  return len;
}
