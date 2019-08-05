#include "../buffer.h"
#include "../fmt.h"
#include "../strlist.h"
#include <ctype.h>

char strlist_dumpx[5] = {',', '\n', ' ', ' ', '\0'};

void
strlist_dump(buffer* out, const strlist* sl) {
  const char *s, *end;
  size_t i = 0, n, count = strlist_count(sl);
  buffer_puts(out, "strlist[");
  buffer_putulong(out, count);
  if(count == 0) {
    buffer_puts(out, "] {}\n");
    buffer_flush(out);
    return;
  }
  buffer_puts(out, "] {\n  0: \"");

  end = sl->sa.s + sl->sa.len;
  strlist_foreach_s(sl, s) {

    n = byte_chr(s, end - s, sl->sep);
    buffer_put(out, s, n); //, &chrs[sizeof(chrs) - 1]  - p);

    if(s + 1 >= end)
      break;
    if(i + 1 >= count)
      break;

    buffer_put(out, "\"", 1);
    buffer_puts(out, strlist_dumpx);
    buffer_putulong(out, ++i);
    buffer_puts(out, ": \"");
  }
  buffer_putsflush(out, "\"\n}\n");
}
