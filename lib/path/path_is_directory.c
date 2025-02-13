#include "../path_internal.h"
#include "../windoze.h"

#include <sys/stat.h>

#if WINDOWS_NATIVE
#include <windows.h>
#include <shlwapi.h>
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
  size_t len = u8swcslen(p);
  wchar_t* w = alloc((len + 1) * sizeof(wchar_t));
  u8stowcs(w, p, len);
  w[len] = '\0';

  return !!PathIsDirectoryW(w);
#else
  struct _stat st;
  int r;

  if((r = lstat(p, &st) == 0))
    if(S_ISDIR(st.st_mode))
      return 1;

  return 0;
#endif
}
