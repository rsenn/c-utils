#include "../utf8.h"

/* the bytes of the sequence that starts at p, 1 for a byte that is no start */
static size_t
step(const char* p) {
  size_t n = u8_len(p, 1);

  return n ? n : 1;
}

/* Position of the last character in u8 that is one of the needles, or the
 * length of u8 in bytes if there is none. */
size_t
u8s_rchrs(const char* u8, const char needles[], size_t nn) {
  int found = 0;
  size_t i, j, k, pos = 0;

  for(i = 0; u8[i];) {
    for(j = 0; j < nn;) {
      const size_t nlen = j + step(&needles[j]) <= nn ? step(&needles[j]) : nn - j;

      for(k = 0; k < nlen && u8[i + k] && u8[i + k] == needles[j + k]; k++) {
      }

      if(k == nlen) {
        pos = i;
        found = 1;
      }

      j += nlen;
    }

    i += step(&u8[i]);
  }

  return found ? pos : i;
}
