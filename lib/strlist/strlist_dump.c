#include "../buffer.h"
#include "../fmt.h"
#include "../strlist.h"
#include <ctype.h>

void
strlist_dump(buffer* out, const strlist* sl) {
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
    buffer_putulong(out, i);
    buffer_puts(out, " = \"");
    buffer_put(out, x, p); //, &chrs[sizeof(chrs) - 1]  - p);
    //    buffer_puts(out, strlist_dumpx);
    buffer_putsflush(out, "\"\n");
    if(p + x < end && x[p] == sl->sep)
      p++;
    x += p;
    n -= p;
    i++;
  } while(x < end);
  buffer_putsflush(out, "}\n");
}
