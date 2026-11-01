#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* appends to out the elements start..end-1 of in, Python style: negative
 * indexes count from the end, indexes beyond the list are clipped, an empty
 * range appends nothing */
void
strlist_slice(strlist* out, const strlist* in, int start, int end) {
  long c = (long)strlist_count(in), i;

  if(start < 0)
    start += c;
  if(end < 0)
    end += c;
  if(start < 0)
    start = 0;
  if(end > c)
    end = (int)c;
  for(i = start; i < end; ++i) {
    size_t n;
    const char* x = strlist_at_n(in, (size_t)i, &n);

    if(x)
      strlist_pushb(out, x, n);
  }
}
