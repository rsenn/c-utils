#include "../path_internal.h"
#include "../utf8.h"
#include "../windoze.h"

#if WINDOWS_NATIVE
#include <windows.h>
#include <shlwapi.h>
#define lstat stat
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#if !defined(__LCC__) || defined(_WIN64)
#ifndef _stat
#define _stat stat
#endif
#endif

int
path_exists(const char* p) {
#if WINDOWS_NATIVE
  size_t len = u8s_len(p);
  wchar_t w[len + 1];

  u8s_to_wcs(w, p, len);
  w[len] = '\0';

  return !!PathFileExistsW(w);
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
