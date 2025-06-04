#include "../path_internal.h"
#include "../stralloc.h"
#include "../strlist.h"
#include "../windoze.h"
#define MAX_NUM(a, b) ((a) > (b) ? (a) : (b))
#if WINDOWS_NATIVE
#include <windows.h>
#endif

int
path_relative_to_b(const char* p, size_t plen, const char* to, size_t tolen, stralloc* out) {
  const char *s1 = p, *s2 = to;
  size_t i, n1 = plen, n2 = tolen;
  SizePair sp = path_common_prefix(s1, n1, s2, n2);

  stralloc_zero(out);

  s1 += sp.sz1;
  n1 -= sp.sz1;
  s2 += sp.sz2;
  n2 -= sp.sz2;

  while(n2 > 0 && s2[0] == PATHSEP_C) {
    s2++;
    n2--;
  }

  while((i = path_skip(s2, n2))) {
    stralloc_cats(out, ".." PATHSEP_S);

    s2 += i;
    n2 -= i;
  }

  i = path_skip_separator(s1, n1);

  stralloc_catb(out, s1 + i, n1 - i);

  if(out->len == 0)
    stralloc_catc(out, '.');

  stralloc_nul(out);
  return 1;
}
