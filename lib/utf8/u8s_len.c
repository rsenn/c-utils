#include "../utf8.h"

int
u8s_len(const char* u8) {
  int len = 0;
  char c;

  while((c = *u8)) {
    if(!(c & 0x80)) {
      len++;
      u8 += 1;
    } else if((c & 0xe0) == 0xc0) {
      len++;
      u8 += 2;
    } else if((c & 0xf0) == 0xe0) {
      len++;
      u8 += 3;
    } else if((c & 0xf8) == 0xf0) {
      len++;
      u8 += 4;
    } else /* error: add width of single byte character entity &#xFF; */ {
      len += 6;
      u8 += 1;
    }
  }

  return len;
}
