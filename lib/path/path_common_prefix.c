#include "../path_internal.h"
#include "../byte.h"

SizePair
path_common_prefix(const char* s1, size_t n1, const char* s2, size_t n2) {
    SizePair r;

  for(r.sz1 = 0, r.sz2 = 0; r.sz1 != n1 && r.sz2 != n2;) {
    size_t i1, i2;

    i1 = path_skip_separator(&s1[r.sz1], n1 - r.sz1);
    i2 = path_skip_separator(&s2[r.sz2], n2 - r.sz2);

    if(!!i1 != !!i2)
      break;

    r.sz1 += i1;
    r.sz2 += i2;

    i1 = path_skip_component(&s1[r.sz1], n1 - r.sz1);
    i2 = path_skip_component(&s2[r.sz2], n2 - r.sz2);

    if(i1 != i2)
      break;

    if(byte_diff(&s1[r.sz1], i1, &s2[r.sz2]))
      break;

    r.sz1 += i1;
    r.sz2 += i2;
  }
  return r;
}
