#include "../scan.h"

#if 0 // def __GNUC__
static inline int
isdigit(int c) {
  return (c >= '0' && c <= '9');
}
//#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#else
#include <ctype.h>
#endif

size_t
scan_double(const char* in, double* dest) {
  double d = 0;
  const char* c = in;
  char neg = 0;

  switch(*c) {
    case '-': neg = 1;
    case '+': c++; break;
    default: break;
  }

  while(isdigit(*c)) {
    d = d * 10 + (*c - '0');
    ++c;
  }

  if(*c == '.') {
    double factor = .1;

    while(isdigit(*++c)) {
      d = d + (factor * (*c - '0'));
      factor /= 10;
    }
  }

  if((*c | 32) == 'e') {
    int exp = 0;
    char eneg = 0;
    const char* e = c + 1;

    /* the sign of the exponent follows the e */
    if(*e == '-') {
      eneg = 1;
      e++;
    } else if(*e == '+') {
      e++;
    }

    if(!isdigit(*e)) {
      d = 0;
      c = in;
      goto done;
    }

    for(c = e; isdigit(*c); ++c)
      exp = exp * 10 + (*c - '0');

    if(eneg)
      while(exp) { /* XXX: this introduces rounding errors */
        d /= 10;
        --exp;
      }
    else
      while(exp) { /* XXX: this introduces rounding errors */
        d *= 10;
        --exp;
      }
  }
done:
  *dest = (neg ? -d : d);
  return (size_t)(c - in);
}
