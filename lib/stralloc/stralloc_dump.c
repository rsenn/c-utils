#include "../uint64.h"
#include "../buffer.h"
#include "../stralloc.h"
#include "../fmt.h"

void
stralloc_dump(const stralloc* sa, buffer* b) {
  size_t n;
  char x[16] = {'\\'};

  buffer_puts(b, "((stralloc)({ .s = @0x");
  buffer_putptr(b, sa->s);
  buffer_puts(b, " \"");

  n = sa->len < 100 ? sa->len : 100;

  for(size_t i = 0; i < n; i++) {
    uint32 c;

    if((c = (unsigned int)(unsigned char)sa->s[i]) < 0x20 || c == '"') {
      if(c == '\r' || c == '\n' || c == '\t' || c == '\v')
        buffer_put(b, x, fmt_escapecharc(x, c));
      else
        buffer_put(b, x, 1 + fmt_8long(x + 1, c));
    } else {
      buffer_putc(b, c);
    }
  }

  buffer_puts(b, "\"");

  if(n < sa->len)
    buffer_puts(b, "... ");

  buffer_puts(b, ", .n = ");
  buffer_putulonglong(b, sa->len);
  buffer_puts(b, ", .a = ");
  buffer_putulonglong(b, sa->a);
  buffer_puts(b, " })");
  buffer_putnlflush(b);
}
