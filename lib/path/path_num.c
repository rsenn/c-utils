#include "../path_internal.h"
size_t
path_num(const char* p, size_t len, int n) {
  const char *s = p, *e = p + len;
  while(s < e) {
    s += path_skip(s, e - s);
    if(--n <= 0)
      break;
  }
  return s - p;
}
