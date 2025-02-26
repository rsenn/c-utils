#include "../stralloc.h"
#include "../dns.h"
#include "../byte.h"

int
dns_domain_todot_cat(stralloc* out, const char* d) {
  char ch;
  char ch2;
  unsigned char ch3;
  char buf[4];

  if(!*d)
    return stralloc_append(out, ".");

  for(;;) {
    ch = *d++;

    while(ch--) {
      ch2 = *d++;

      if((ch2 >= 'A') && (ch2 <= 'Z'))
        ch2 = (char)(ch2 + 32);

      if(((ch2 >= 'a') && (ch2 <= 'z')) || ((ch2 >= '0') && (ch2 <= '9')) || (ch2 == '-') || (ch2 == '_')) {
        if(!stralloc_append(out, &ch2))
          return 0;
      } else {
        ch3 = (unsigned char)ch2;
        buf[3] = (char)('0' + (ch3 & 7));
        ch3 >>= 3;
        buf[2] = (char)('0' + (ch3 & 7));
        ch3 >>= 3;
        buf[1] = (char)('0' + (ch3 & 7));
        buf[0] = '\\';

        if(!stralloc_catb(out, buf, 4))
          return 0;
      }
    }

    if(!*d)
      return 1;

    if(!stralloc_append(out, "."))
      return 0;
  }
}

unsigned int
dns_domain_todot_inline(char* x) {
  char ch, *d;

  for(d = x; *d; d++) {
    ch = *d;
    *d = '.';

    while(ch--) {
      ++d;

      if((*d >= 'A') && (*d <= 'Z'))
        *d = (char)(*d + 32);
    }
  }
  return d - x;
}

unsigned int
dns_domain_todot(char* out, const char* in) {
  const char* x;
  char* y;
  unsigned char c;

  x = in;
  y = out;

  while((c = *x++)) {
    byte_copy(y, c, x);
    y += c;
    x += c;

    if(*x)
      *y++ = '.';
  }
  return y - out;
}
