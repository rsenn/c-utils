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
path_is_directory(const char* p) {
  struct _stat st;
  int r;
  if((r = lstat(p, &st) == 0)) {
    if(S_ISDIR(st.st_mode))
      return 1;
  }
  return 0;
}
