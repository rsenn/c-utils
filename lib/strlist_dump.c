#include "strlist.h"
#include "buffer.h"
#include "fmt.h"
#include <ctype.h>

void
strlist_dump(buffer* out, const strlist* sl)
{
  const char* s;
  size_t i = 0, n = strlist_count(sl);
  buffer_puts(out, "strlist[");
  buffer_putulong(out, n);
  buffer_puts(out, "]{0:\"");
  const char* end = sl->sa.s + sl->sa.len;
  for(s = sl->sa.s; s < end; ++s) {
    while(*s) {
      char chrs[64] = { '\\', *s, '\0' };
      char* p = &chrs[1];
      if(!isprint(*p) || iscntrl(*p)) {
        p[fmt_ulong(p, (unsigned long)(unsigned char)*p)] = '\0';

        --p;
      }

      buffer_puts(out, p); //, &chrs[sizeof(chrs) - 1]  - p);

      ++s;
    }
    if(s + 1 < end) {
      buffer_puts(out, "\", ");
      buffer_putulong(out, ++i);
      buffer_puts(out, ":\"");
    }
    if(--n == 0)
      break;
  }
  buffer_putsflush(out, "\"}\n");
}
