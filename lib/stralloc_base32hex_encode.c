#include "stdint.h"
#include "char.h"
#include "base32hex.h"
#include "stralloc.h"

/*
  1 --> OK
  0 --> memory exhasted
*/

#define MASK	0x1f
#define SHIFT	5

static unsigned int encode(stralloc *out, register uint64_t v, unsigned int octets)
{
  char data[8];
  byte_t ch1;
  byte_t ch2;
  byte_t ch3;
  byte_t ch4;
  byte_t ch5;
  byte_t ch6;
  byte_t ch7;
  byte_t ch8;

  ch8 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch7 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch6 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch5 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch4 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch3 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch2 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch1 = (byte_t)(v & MASK);


  data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = '=';

  data[0] = char_base32hex_chars[ch1];  /* 1 */
  data[1] = char_base32hex_chars[ch2];  /* 2 */

  if (octets == 1) {  /* 8 bits remaining */
    return stralloc_catb(out, data, 8);
  }

  data[2] = char_base32hex_chars[ch3];  /* 3 */
  data[3] = char_base32hex_chars[ch4];  /* 4 */

  if (octets == 2) {  /* 16 bits remaining */
    return stralloc_catb(out, data, 8);
  }

  data[4] = char_base32hex_chars[ch5];  /* 5 */

  if (octets == 3) {   /* 24 bits remaining */
    return stralloc_catb(out, data, 8);
  }

  data[5] = char_base32hex_chars[ch6];  /* 6 */
  data[6] = char_base32hex_chars[ch7];  /* 7 */

  if (octets == 4) {   /* 32 bits remaining */
    return stralloc_catb(out, data, 8);
  }

  data[7] = char_base32hex_chars[ch8];  /* 8 */

  return stralloc_catb(out, data, 8);
}

unsigned int stralloc_base32hex_encode(stralloc *out, const stralloc *in)
{
  static stralloc tmp = STRALLOC;
  register uint64_t v;
  register unsigned int i;
  unsigned int octets;
  unsigned int len;
  byte_t ch;

  len = in->len;
  if (!len) return stralloc_erase(out);
  if (!stralloc_erase(&tmp)) return 0;
  v = octets = 0;
  for (i = 0; i < len; i++) {
    ch = (byte_t)in->s[i];
    v <<= 8;
    v |= ch;
    octets++;
    if (octets == 5) {
      if (!encode(&tmp, v, octets)) return 0;
      v = octets = 0;
    }
  }
  if (!octets) return stralloc_copy(out, &tmp);
  if (octets < 2) {
    v <<= 8;  /* octets == 1 */
  }
  if (octets < 3) {
    v <<= 8;  /* octets == 1 or 2 */
  }
  if (octets < 4) {
    v <<= 8;  /* octets == 1 or 2 or 3 */
  }
  if (octets < 5) {
    v <<= 8;  /* octets == 1 or 2 or 3 or 4 */
  }
  if (!encode(&tmp, v, octets)) return 0;
  return stralloc_copy(out, &tmp);
}
