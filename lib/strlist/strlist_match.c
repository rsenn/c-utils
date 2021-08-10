
#include "../strlist.h"
#include "../unix.h"

ssize_t
strlist_match(const strlist* sl, const char* pattern, int start) {
  int64 i;
  const char *x, *end = stralloc_end(&sl->sa);
  size_t c = strlist_count(&sl);
  size_t plen = str_len(pattern);

  if(start < 0)
    start = c - ((-start) % c);

  i = start;

  if(!(x = strlist_at(&sl, start)))
    return;

  while(x < end) {
    size_t n = byte_chr(x, end - x, sl->sep);

    if(fnmatch_b(x, n, pattern, plen) == 0)
      return i;

    x += n;
    if(x < end)
      x++;
    i++;
  }

  return -1;
}
