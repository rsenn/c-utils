#include "../path_internal.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/stat.h>
#endif

int
path_exists(const char* p) {
#if WINDOWS_NATIVE
  return access(p, 0) == 0;
#else
  struct stat st;
  return lstat(p, &st) == 0;
#endif
}