#include "../path_internal.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/stat.h>
#endif

int
path_exists(const char* p) {
	int ret; 
#if WINDOWS_NATIVE
  ret = access(p, 0) == 0;
#else
  struct stat st;
  ret = lstat(p, &st) == 0;
#endif
  return ret;
}