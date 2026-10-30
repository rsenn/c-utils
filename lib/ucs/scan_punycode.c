#include "../ucs_internal.h"

/* scan_punycode decodes the Punycode (RFC 3492) in src[n], without the
 * xn-- prefix, into dest. *max is the room of dest in code points and
 * becomes the number decoded. Returns n, 0 on bad input or overflow. */
size_t
scan_punycode(const char* src, size_t n, uint32* dest, size_t* max) {
  uint32 c = PUNYCODE_INITIAL_N, bias = PUNYCODE_INITIAL_BIAS, i = 0, oldi, w, k, t, d;
  size_t b = 0, in, out, j;

  for(j = 0; j < n; ++j)
    if(src[j] == '-')
      b = j;

  if(b > *max)
    return 0;

  for(j = 0; j < b; ++j) {
    if((unsigned char)src[j] >= 0x80)
      return 0;
    dest[j] = (unsigned char)src[j];
  }
  out = b;

  for(in = b ? b + 1 : 0; in < n; ++out) {
    for(oldi = i, w = 1, k = PUNYCODE_BASE;; k += PUNYCODE_BASE) {
      if(in >= n)
        return 0;
      d = (unsigned char)src[in++];
      d = d >= '0' && d <= '9' ? d - '0' + 26 : d >= 'a' && d <= 'z' ? d - 'a' : d >= 'A' && d <= 'Z' ? d - 'A' : 36;
      if(d >= PUNYCODE_BASE || d > (0xffffffffu - i) / w)
        return 0;
      i += d * w;
      t = PUNYCODE_T(k, bias);
      if(d < t)
        break;
      if(w > 0xffffffffu / (PUNYCODE_BASE - t))
        return 0;
      w *= PUNYCODE_BASE - t;
    }

    bias = ucs_punycode_adapt(i - oldi, out + 1, oldi == 0);

    if(i / (out + 1) > 0xffffffffu - c || out >= *max)
      return 0;
    c += i / (out + 1);
    i %= out + 1;

    if(c < 0x80 || c > 0x10ffff)
      return 0;
    for(j = out; j > i; --j)
      dest[j] = dest[j - 1];
    dest[i++] = c;
  }
  *max = out;
  return n;
}
