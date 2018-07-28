#include "../str.h"

#if defined(_WIN32) || defined(_WIN64)
#define PATH_LASTSEP(p) str_rchrs((p), "\\/", 2)
#else
#define PATH_LASTSEP(p) str_rchr((p), '/')
#endif

char*
str_basename(char* s) {
  size_t n = PATH_LASTSEP(s);
  if(s[n] != '\0')
    s = s + n + 1;
  return s;
}