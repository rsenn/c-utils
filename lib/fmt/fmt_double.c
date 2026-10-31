#include "../fmt.h"
#include "../byte.h"
#include "../uint64.h"
#include <math.h>
#include <float.h>

#ifdef __BORLANDC__
/* its math.h has neither and the _isnan of float.h is not in the import library */
#undef isnan
#undef isinf
#define isnan(x) ((x) != (x))
#define isinf(x) ((x) > DBL_MAX)
#endif

/* formats d with up to prec digits after the point (default 6), trailing
 * zeros are dropped, large and small values use the form 1.5e20, nan and
 * infinity give NaN, Inf and -Inf. Returns the length written, or 0 if it
 * does not fit in max bytes (max <= 0 means no limit). */
size_t
fmt_double(char* dest, double d, int max, int prec) {
  char tmp[48];
  size_t n = 0, i;
  int e10 = 0;
  uint64 ip, fr, scale = 1;

  if(isnan(d)) {
    byte_copy(tmp, 3, "NaN");
    n = 3;
    goto done;
  }

  if(d < 0) {
    d = -d;
    tmp[n++] = '-';
  }

  if(isinf(d)) {
    byte_copy(tmp + n, 3, "Inf");
    n += 3;
    goto done;
  }

  if(prec <= 0)
    prec = 6;
  if(prec > 15)
    prec = 15;

  if(d != 0) {
    double m = d;
    int x = 0;

    while(m >= 10) {
      m /= 10;
      ++x;
    }
    while(m < 1) {
      m *= 10;
      --x;
    }
    /* only values outside 1e-4 .. 1e15 use the exponent */
    if(x < -4 || x > 14) {
      d = m;
      e10 = x;
    }
  }

  for(i = 0; i < (size_t)prec; ++i)
    scale *= 10;

  ip = (uint64)d;
  fr = (uint64)((d - (double)ip) * (double)scale + 0.5);
  if(fr >= scale) {
    ++ip;
    fr -= scale;
  }
  if(e10 && ip >= 10) {
    ip = 1;
    ++e10;
  }

  {
    char digits[24];
    size_t len = 0;

    do {
      digits[len++] = (char)('0' + ip % 10);
      ip /= 10;
    } while(ip);

    while(len)
      tmp[n++] = digits[--len];
  }

  if(fr) {
    size_t start = n + 1;

    tmp[n++] = '.';
    for(i = (size_t)prec; i > 0; --i) {
      tmp[n + i - 1] = (char)('0' + fr % 10);
      fr /= 10;
    }
    n += (size_t)prec;
    while(n > start && tmp[n - 1] == '0')
      --n;
  }

  if(e10) {
    tmp[n++] = 'e';
    if(e10 < 0) {
      tmp[n++] = '-';
      e10 = -e10;
    }
    if(e10 >= 100)
      tmp[n++] = (char)('0' + e10 / 100);
    if(e10 >= 10)
      tmp[n++] = (char)('0' + e10 / 10 % 10);
    tmp[n++] = (char)('0' + e10 % 10);
  }

done:
  if(max > 0 && n > (size_t)max)
    return 0;
  if(dest)
    byte_copy(dest, n, tmp);
  return n;
}
