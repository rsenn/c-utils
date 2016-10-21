#include "strlist.h"
#include "buffer.h"

void
strlist_dump(buffer* out, const strlist* sl) {
  size_t n = strlist_count(sl);
  buffer_puts(out, "strlist[");
  buffer_putulong(out, n);
  buffer_puts(out, "]{\"");
  for(size_t i = 0; i < n; ++i) {
    const char* s = strlist_at(sl, i);
    while(*s) {
      char chrs[5] = { '\\', *s, '\0' };
      char *p = &chrs[1];
       if(!isprint(*p) || iscntrl(*p)) {
        p[fmt_ulong(p, *p)] = '\0';
        
         --p;
       }


       buffer_puts(out, p); //, &chrs[sizeof(chrs) - 1]  - p);
     
       ++s;
    }
  	if(i + 1 < n)
  		buffer_puts(out, "\", \"");
  }
  buffer_putsflush(out, "\"}\n");
}
