#include "../path_internal.h"
#include "../windoze.h"

#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
  size_t len = u8s_len(p);
#ifdef HAVE_ALLOCA
  wchar_t* w = alloca((len + 1) * sizeof(wchar_t));
#else
  wchar_t w[len + 1];
#endif
  u8s_to_wcs(w, p, len);
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
