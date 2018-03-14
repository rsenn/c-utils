#include "stdint.h"
#include "base64url.h"
#include "stralloc.h"
#include "error.h"

static unsigned int decode(stralloc *out, register uint32_t v, unsigned int octets)
{
  char ch1;
  char ch2;
  char ch3;

  ch3 = (char)(v & 0xff);
  v >>= 8;
  ch2 = (char)(v & 0xff);
  v >>= 8;
  ch1 = (char)(v & 0xff);
  if (!stralloc_append(out, &ch1)) return 0;
  octets--;
  if (!octets) return 1;
  if (!stralloc_append(out, &ch2)) return 0;
  octets--;
  if (!octets) return 1;
  if (!stralloc_append(out, &ch3)) return 0;
  return 1;
}

unsigned int stralloc_base64url_decode(stralloc *out, const stralloc *in)
{
  static stralloc tmp = STRALLOC;
  register uint32_t v;
  register unsigned int i;
  register unsigned int len;
  unsigned int bits;
  unsigned int padding;
  int r;
  char ch;

  len = in->len;
  if (!len) return stralloc_erase(out);
  if (len & 0x03) {
    errno = error_proto;
    return 0;
  }
  if (!stralloc_erase(&tmp)) return 0;
  v = padding = bits = 0;
  for (i = 0; i < len; i++) {
    if (i && !(i & 0x03)) {  /* modulo 4 */
      if (!decode(&tmp, v, 3)) return 0;
      v = 0;
      bits = 0;
    }
    ch = in->s[i];
    if (ch == '=') {
      break;
    }
    r = base64url_decode((unsigned int)ch);  /* returns: -1 or 0..63 */
    if (r < 0) {
      errno = error_proto;
      return 0;
    }
    v <<= 6;
    v |= (unsigned int)r;
    bits += 6;
  }
  while (i < len) {
    if (in->s[i] != '=') break;
    padding++;
    v <<= 6;
    i++;
  }
  if (i != len) {
    errno = error_proto;
    return 0;
  }
  if (padding > 2) {
    errno = error_proto;
    return 0;
  }
  if (!decode(&tmp, v, bits >> 3)) return 0;  /* bytes = bits div 8 */
  return stralloc_copy(out, &tmp);
}
