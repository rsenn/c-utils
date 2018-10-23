#include "../windoze.h"
#include "../path_internal.h"
#include "../str.h"
#include <ctype.h>

int
path_absolute(const char* p) {
  size_t len = str_len(p);
  if(len > 0 && p[0] == '/') return 1;
#if WINDOWS
  if(len >= 3 && isalnum(p[0]) && p[1] == ':' && path_issep(p[2])) return 1;
#endif
  return 0;
}