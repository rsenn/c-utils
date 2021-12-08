#include "../buffer.h"
#include "../fmt.h"
#include "../strlist.h"
#include <ctype.h>

void
strlist_dump_named(buffer* out, const strlist* sl, const char* names[]) {
  const char *x, *end;
  size_t i = 0, n, count = strlist_count(sl);
  buffer_puts(out, "strlist[");
  buffer_putulong(out, count);
  if(count == 0) {
    buffer_puts(out, "] {}\n");
    buffer_flush(out);
    return;
  }
  buffer_puts(out, "] {\n");
  x = sl->sa.s;
  n = sl->sa.len;
  end = sl->sa.s + n;
  do {
    size_t p = byte_chr(x, end - x, sl->sep);

    buffer_puts(out, "  ");
    if(names[i])
      buffer_puts(out, names[i]);
    buffer_puts(out, " = \"");
    buffer_put(out, x, p);
    buffer_putsflush(out, "\"\n");
    if(p + x < end && x[p] == sl->sep)
      p++;
    x += p;
    n -= p;
    i++;
  } while(x < end);
  buffer_putsflush(out, "}\n");
}
