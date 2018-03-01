#include "stdint.h"
#include "char.h"
#include "stralloc.h"

/*
  1 --> OK
  0 --> memory exhasted
*/

#define MASK	0x3f
#define SHIFT	6

static unsigned int encode(stralloc *out, register uint32_t v, unsigned int octets)
{
  char data[4];
  byte_t ch1;
  byte_t ch2;
  byte_t ch3;
  byte_t ch4;

  ch4 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch3 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch2 = (byte_t)(v & MASK);
  v >>= SHIFT;
  ch1 = (byte_t)(v & MASK);

  data[2] = data[3] = '=';

  data[0] = char_base64url_chars[ch1];  /* 1 */
  data[1] = char_base64url_chars[ch2];  /* 2 */

  if (octets == 1) {  /* 8 bits remaining */
    return stralloc_catb(out, data, 4);
  }

  data[2] = char_base64url_chars[ch3];  /* 3 */

  if (octets == 2) {  /* 16 bits remaining */
    return stralloc_catb(out, data, 4);
  }

  data[3] = char_base64url_chars[ch4];  /* 4 */

  return stralloc_catb(out, data, 4);
}

unsigned int stralloc_base64url_encode(stralloc *out, const stralloc *in)
{
  static stralloc tmp = STRALLOC;
  register uint32_t v;
  register unsigned int i;
  register unsigned int len;
  unsigned int octets;
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
    if (octets == 3) {
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
  if (!encode(&tmp, v, octets)) return 0;
  return stralloc_copy(out, &tmp);
}
