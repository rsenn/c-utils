#include "../path_internal.h"
#include "../windoze.h"

#include <sys/stat.h>

#if WINDOWS_NATIVE
#include <windows.h>
#include <shlwapi.h>
#define lstat stat
#else
#include <unistd.h>
#endif

#if !defined(__LCC__) || defined(_WIN64)
#ifndef _stat
#define _stat stat
#endif
#endif

int
path_exists(const char* p) {
#if WINDOWS_NATIVE
  return !!PathFileExistsA(p);
#else
  struct _stat st;
  int r;
#if !(NO_ACCESS || defined(__ANDROID__))
  if(access(p, 0) == 0)
#endif
    return 1;
  r = lstat(p, &st);
  if(r == 0)
    return 1;
  return 0;
#endif
}
