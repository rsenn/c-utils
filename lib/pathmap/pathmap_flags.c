#include "../pathmap.h"

/* pathmap_flags appends in[0..n) with the path in every "-I<path>" and
 * "-L<path>" word rewritten, white space is kept. Returns the number
 * of words changed, -1 on out of memory. */
int
pathmap_flags(const pathmap* m, const char* in, size_t n, stralloc* out) {
  size_t i = 0, j, skip;
  int r, changed = 0;

  while(i < n) {
    for(j = i; j < n && in[j] != ' ' && in[j] != '\t' && in[j] != '\n'; j++) {}

    skip = j - i > 2 && in[i] == '-' && (in[i + 1] == 'I' || in[i + 1] == 'L') ? 2 : 0;

    if(!skip) {
      if(!stralloc_catb(out, in + i, j - i))
        return -1;
    } else {
      if(!stralloc_catb(out, in + i, skip))
        return -1;

      if((r = pathmap_apply(m, in + i + skip, j - i - skip, out)) < 0)
        return -1;

      changed += r;
    }

    for(i = j; i < n && (in[i] == ' ' || in[i] == '\t' || in[i] == '\n'); i++) {}

    if(i > j && !stralloc_catb(out, in + j, i - j))
      return -1;
  }

  return changed;
}
