#include "../buffer.h"
#include "../fmt.h"
#include "../strlist.h"
#include <ctype.h>
/*
char strlist_dumpx[5] = {',', '\n', ' ', ' ', '\0'};
*/
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
  while(x < end) {
    size_t p = byte_chr(x, end - x, sl->sep);
    if(p) {
      buffer_puts(out, "  ");
      buffer_putulong(out, i);
      buffer_puts(out, " = \"");
      buffer_put(out, x, p); //, &chrs[sizeof(chrs) - 1]  - p);
      if(p + x < end && x[p] == sl->sep)
        p++;
      //    buffer_puts(out, strlist_dumpx);
      buffer_putsflush(out, "\"\n");
    }
    x += p;
    n -= p;
    i++;
  }
  buffer_putsflush(out, "}\n");
}