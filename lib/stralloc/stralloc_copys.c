#include "../str.h"
#include "../stralloc.h"

extern int
stralloc_copys(stralloc* sa, const char* s) {
  return stralloc_copyb(sa, s, str_len(s));
}
