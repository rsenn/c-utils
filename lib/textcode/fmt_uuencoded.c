#include "../fmt.h"
#include "../textcode.h"

static inline unsigned int
enc(unsigned char x) {
  return ((x - 1) & 077) + '!';
}

/* uuencode: lines of up to 45 bytes, each one a length character, 4
 * characters per 3 bytes and a newline. dest may be NULL to measure, and the
 * measured size is exactly what is written. The last group of a line is
 * padded with zeros; the input is never read past len. */
size_t
fmt_uuencoded(char* dest, const char* src, size_t len) {
  register const unsigned char* s = (const unsigned char*)src;
  size_t n = 0;

  if(!dest) {
    size_t rem = len % 45;

    if(len > ((size_t)-1) / 2)
      return (size_t)-1;
    return len / 45 * 62 + (rem ? 2 + (rem + 2) / 3 * 4 : 0);
  }

  while(len) {
    size_t diff = len > 45 ? 45 : len;
    size_t groups = (diff + 2) / 3, g;

    dest[n++] = (char)enc((unsigned char)diff);

    for(g = 0; g < groups; ++g) {
      unsigned long b0 = 0, b1 = 0, b2 = 0, tmp;
      size_t have = diff - g * 3;

      b0 = s[0];
      if(have > 1)
        b1 = s[1];
      if(have > 2)
        b2 = s[2];
      tmp = (b0 << 16) + (b1 << 8) + b2;
      dest[n++] = (char)enc((tmp >> 18) & 077);
      dest[n++] = (char)enc((tmp >> 12) & 077);
      dest[n++] = (char)enc((tmp >> 6) & 077);
      dest[n++] = (char)enc(tmp & 077);
      s += 3;
    }
    s -= groups * 3 - diff; /* the last group may have been short */
    dest[n++] = '\n';
    len -= diff;
  }
  return n;
}
