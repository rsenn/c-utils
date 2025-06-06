#include "../path_internal.h"
#include "../stralloc.h"

char*
path_join(const char* a, const char* b) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copys(&sa, a);
  path_appends(b, &sa);
  stralloc_nul(&sa);
  return sa.s;
}
