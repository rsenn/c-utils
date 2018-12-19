#include "../path_internal.h"
#include "../windoze.h"

#include <sys/stat.h>

#if WINDOWS_NATIVE
#include <io.h>
#define lstat stat
#endif

#if !defined(__LCC__) || defined(_WIN64)
#define _stat stat
#endif

int
path_exists(const char* p) {
#if WINDOWS_NATIVE
  if(access(p, 0) == 0) return 1;
#endif
  {
    struct _stat st;
    if(lstat(p, &st) == 0)
      return 1;
  }
  return 0;
}