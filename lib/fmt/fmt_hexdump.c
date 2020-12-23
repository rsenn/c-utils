#include "../fmt.h"
#include "../uint8.h"

unsigned int
fmt_hexdump(void* out, const void* d, unsigned int len) {
  char *x, *e, *w;
  size_t i, o = 0;
  x = d;
  e = x + len;
  w = out;
  for(i = 0; i < len; i++) {
    uint8 ch = x[i];
    o += fmt_hexb(w ? &w[o] : 0, &ch, 1);
    if(w)
      w[o] = ' ';
    ++o;
  }
  if(w)
    w[o] = '|';
  ++o;
  for(i = 0; i < len; i++) {
    if(w)
      w[o] = isprint(*x) ? *x : '.';
    ++o;
  }
  if(w)
    w[o] = '|';
  ++o;
  return o;
}
