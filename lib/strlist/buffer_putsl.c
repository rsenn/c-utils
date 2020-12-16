#include "../buffer.h"
#include "../strlist.h"
#include <ctype.h>

void
buffer_putsl(buffer* out, const strlist* sl, const char* separator) {
  const char *x, *end;
  size_t i = 0, n, count = strlist_count(sl);

  x = sl->sa.s;
  n = sl->sa.len;
  end = sl->sa.s + n;
  do {
    size_t p = byte_chr(x, end - x, sl->sep);

    if(i)
      buffer_puts(out, separator);
    buffer_put(out, x, p);
    if(p + x < end && x[p] == sl->sep)
      p++;
    x += p;
    n -= p;
    i++;
  } while(x < end);
}
