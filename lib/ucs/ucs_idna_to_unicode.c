#include "../ucs.h"

#define IDNA_LABEL 128

/* ucs_idna_to_unicode converts the ASCII domain name src[n] into UTF-8 in
 * dest, which has room for max bytes. Labels starting with xn-- are
 * decoded, a label that does not decode stays as it is. Returns the
 * length, 0 on overflow. */
size_t
ucs_idna_to_unicode(char* dest, size_t max, const char* src, size_t n) {
  uint32 cp[IDNA_LABEL];
  size_t len = 0, l, i, r, count;
  const char* end;

  for(;;) {
    for(end = src; (size_t)(end - src) < n && *end != '.';)
      ++end;
    l = (size_t)(end - src);
    count = IDNA_LABEL;

    if(l > 4 && (src[0] | 32) == 'x' && (src[1] | 32) == 'n' && src[2] == '-' && src[3] == '-' &&
       scan_punycode(src + 4, l - 4, cp, &count)) {
      for(i = 0; i < count; ++i) {
        if(!(r = fmt_ucs4_utf8(0, cp[i])) || len + r > max)
          return 0;
        len += fmt_ucs4_utf8(dest + len, cp[i]);
      }
    } else {
      if(len + l > max)
        return 0;
      for(i = 0; i < l; ++i)
        dest[len + i] = src[i];
      len += l;
    }

    if(end == src + n)
      return len;
    if(len >= max)
      return 0;
    dest[len++] = '.';
    n -= l + 1;
    src = end + 1;
  }
}
