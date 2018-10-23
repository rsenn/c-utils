#include "../str.h"
#include "../stralloc.h"

int
stralloc_inserts(stralloc* sa, const char* s, size_t pos) {
  return stralloc_insertb(sa, s, pos, str_len(s));
}
