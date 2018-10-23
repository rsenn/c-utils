#include "../windoze.h"
#include "../path.h"
#include "../str.h"
#include <ctype.h>

int
path_absolute(const char* p) {
  size_t len = str_len(p);
  if(len > 0 && p[0] == '/') return 1;
#if WINDOWS
  if(len >= 2 && isalpha(p[0]) && p[1] == ':') return 1;
#endif
  return 0;
}
