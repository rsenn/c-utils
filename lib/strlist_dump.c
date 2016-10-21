#include "strlist.h"
#include "buffer.h"

void
strlist_dump(buffer* out, const strlist* sl) {
  size_t n = strlist_count(sl);
  buffer_puts(out, "strlist[");
  buffer_putulong(out, n);
  buffer_puts(out, "]{");
  for(size_t i = 0; i < n; ++i) {
  	buffer_puts(out, strlist_at(sl, i));
  	if(i + 1 < n)
  		buffer_put(out, "|", 1);
  }
  buffer_putsflush(out, "}\n");
}
