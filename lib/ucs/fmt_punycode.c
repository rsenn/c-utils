#include "../ucs_internal.h"

static inline char
punycode_digit(uint32 d) {
  return (char)(d < 26 ? 'a' + d : '0' + d - 26);
}

/* fmt_punycode encodes src[n] as Punycode (RFC 3492) without the xn--
 * prefix and returns the length, 0 on empty input or overflow. dest may
 * be NULL to only measure. */
size_t
fmt_punycode(char* dest, const uint32* src, size_t n) {
  uint32 m, q, t, k, bias = PUNYCODE_INITIAL_BIAS, c = PUNYCODE_INITIAL_N, delta = 0;
  size_t j, b, h, len = 0;

  for(j = 0; j < n; ++j)
    if(src[j] < 0x80) {
      if(dest)
        dest[len] = (char)src[j];
      ++len;
    }

  h = b = len;

  if(b) {
    if(dest)
      dest[len] = '-';
    ++len;
  }

  while(h < n) {
    for(m = 0xffffffffu, j = 0; j < n; ++j)
      if(src[j] >= c && src[j] < m)
        m = src[j];

    if(m - c > (0xffffffffu - delta) / (h + 1))
      return 0;
    delta += (m - c) * (h + 1);
    c = m;

    for(j = 0; j < n; ++j) {
      if(src[j] < c && ++delta == 0)
        return 0;

      if(src[j] != c)
        continue;

      for(q = delta, k = PUNYCODE_BASE;; k += PUNYCODE_BASE) {
        t = PUNYCODE_T(k, bias);
        if(q < t)
          break;
        if(dest)
          dest[len] = punycode_digit(t + (q - t) % (PUNYCODE_BASE - t));
        ++len;
        q = (q - t) / (PUNYCODE_BASE - t);
      }
      if(dest)
        dest[len] = punycode_digit(q);
      ++len;
      bias = ucs_punycode_adapt(delta, h + 1, h == b);
      delta = 0;
      ++h;
    }
    ++delta;
    ++c;
  }
  return len;
}
