#include "../path_internal.h"
#include "../windoze.h"

#include <sys/stat.h>

#if WINDOWS_NATIVE
#include <windows.h>
#define lstat stat
#endif

#if !defined(__LCC__) || defined(_WIN64)
#ifndef _stat
#define _stat stat
#endif
#endif

int
path_is_directory(const char* p) {
#if WINDOWS_NATIVE
  return !!PathIsDirectoryA(p);
#else
  struct _stat st;
  int r;
  if((r = lstat(p, &st) == 0)) {
    if(S_ISDIR(st.st_mode))
      return 1;
  }
  return 0;
#endif
}
