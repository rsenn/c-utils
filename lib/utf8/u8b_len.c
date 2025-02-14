#include "../utf8.h"

size_t
u8b_len(const char* u8, size_t n) {
  size_t len = 0;

  for(size_t i = 0; i < n;) {
    const char c = u8[i];

    if(!(c & 0x80)) {
      len++;
      i += 1;
    } else if((c & 0xe0) == 0xc0) {
      len++;
      i += 2;
    } else if((c & 0xf0) == 0xe0) {
      len++;
      i += 3;
    } else if((c & 0xf8) == 0xf0) {
      len++;
      i += 4;
    } else /* error: add width of single byte character entity &#xFF; */ {
      len += 6;
      i += 1;
    }
  }

  return len;
}
