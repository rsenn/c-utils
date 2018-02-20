#include "strlist.h"
#include "buffer.h"
#include "fmt.h"
#include <ctype.h>

char
strlist_dumpx[5] = {',', ' ', '\0', '\0'};

void
strlist_dump(buffer* out, const strlist* sl)
{
  const char* s;
  size_t i = 0, n = strlist_count(sl);
  buffer_puts(out, "strlist[");
  buffer_putulong(out, n);
  if( n == 0) {
    buffer_puts(out, "]{}\n");
    buffer_flush(out);
    return;
  }
  buffer_puts(out, "]{0:\"");
  const char* end = sl->sa.s + sl->sa.len;
  for(s = sl->sa.s; s < end; ++s) {
    while(*s) {
      char chrs[64] = { '\\', *s, '\0' };
      char* p = &chrs[1];
      if(*p == '\n' || *p == '\r' || *p == '\0') {
        p[fmt_ulong(p, (unsigned long)(unsigned char)*p)] = '\0';

        --p;
      }

      buffer_puts(out, p); //, &chrs[sizeof(chrs) - 1]  - p);

      ++s;
    }
    if(s + 1 < end) {
      buffer_put(out, "\"", 1);
      buffer_puts(out, strlist_dumpx);
      buffer_putulong(out, ++i);
      buffer_puts(out, ":\"");
    }
    if(--n == 0)
      break;
  }
  buffer_putsflush(out, "\"}\n");
}
