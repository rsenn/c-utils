#include "../ucs.h"

#define IDNA_LABEL 128

/* ucs_idna_to_ascii converts the UTF-8 domain name src[n] to its ASCII
 * form into dest, which has room for max bytes. Labels with only ASCII
 * stay as they are, others become xn-- plus Punycode, lowercased.
 * Returns the length, 0 on bad UTF-8, a label over 63 bytes or overflow. */
size_t
ucs_idna_to_ascii(char* dest, size_t max, const char* src, size_t n) {
  uint32 cp[IDNA_LABEL], u;
  size_t len = 0, count, r, l, ascii;
  const char* label;

  for(;;) {
    label = src;
    count = 0;
    ascii = 1;

    while(n) {
      if(!(r = scan_utf8_ucs4(src, n, &u)))
        return 0;
      if(u == '.' || u == 0x3002 || u == 0xff0e || u == 0xff61)
        break;
      if(count == IDNA_LABEL)
        return 0;
      ascii &= u < 0x80;
      cp[count++] = u;
      src += r;
      n -= r;
    }

    if(ascii) {
      l = (size_t)(src - label);
      if(l > 63 || len + l > max)
        return 0;
      for(r = 0; r < l; ++r)
        dest[len + r] = label[r];
    } else {
      for(r = 0; r < count; ++r)
        cp[r] = ucs_tolower(cp[r]);
      if(len + 4 > max)
        return 0;
      dest[len] = 'x';
      dest[len + 1] = 'n';
      dest[len + 2] = '-';
      dest[len + 3] = '-';
      if(!(l = fmt_punycode(0, cp, count)) || l > 59 || len + 4 + l > max)
        return 0;
      fmt_punycode(dest + len + 4, cp, count);
      l += 4;
    }
    len += l;

    if(!n)
      return len;

    /* skip the separator, which may be multibyte */
    r = scan_utf8_ucs4(src, n, &u);
    src += r;
    n -= r;
    if(len >= max)
      return 0;
    dest[len++] = '.';
  }
}
