#include "../windoze.h"

#include "../str.h"

#if WINDOWS
#define PATH_LASTSEP(p) str_rchrs((p), "\\/", 2)
#else
#define PATH_LASTSEP(p) str_rchr((p), '/')
#endif

char*
str_basename(const char* s) {
  size_t n = PATH_LASTSEP(s);
  if(n && s[n] != '\0') s = s + n + 1;
  return (char*)s;
}
