#include "../path_internal.h"
#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#include <sys/stat.h>
#define lstat stat
#else
#include <sys/stat.h>
#endif

int
path_exists(const char* p) {
#if WINDOWS_NATIVE
  if(access(p, 0) == 0) return 1;
#endif
  {
    struct stat st;
    if(lstat(p, &st) == 0)
      return 1;
  }
  return 0;
}