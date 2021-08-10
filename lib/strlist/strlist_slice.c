#include "../byte.h"
#include "../strlist.h"

void
strlist_slice(strlist* out, const strlist* in, int start, int end) {
  int64 i, j;
  const char* x = in->sa.s;
  size_t c = strlist_count(in);

  if(start < 0)
    start = c - ((-start) % c);

  if((i = strlist_pos(in, start)) == -1)
    return;

  if(end < 0)
    end = c - ((-end) % c);

  if((j = strlist_pos(in, end)) == -1)
    return;

  while(i < j) {
    size_t len = byte_chr(&x[i], j - i, in->sep);

    strlist_pushb(out, &x[i], len);

    i += len;
    if(i < j)
      i++;
  }
}
