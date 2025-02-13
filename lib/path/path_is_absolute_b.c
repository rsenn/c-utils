#include "../path_internal.h"
#include "../utf8.h"
#include "../windoze.h"
#include <ctype.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if WINDOWS_NATIVE
#include <shlwapi.h>
#endif

int
path_is_absolute_b(const char* x, size_t n) {
#if WINDOWS_NATIVE
  size_t len = u8b_len(x, n);
#ifdef HAVE_ALLOCA
  wchar_t* w = alloca((len + 1) * sizeof(wchar_t));
#else
  wchar_t w[len + 1];
#endif
  u8b_to_wcs(w, x, n, len);
  w[len] = '\0';

  return !PathIsRelativeW(w);
#else

  if(n > 0 && x[0] == '/')
    return 1;

#if WINDOWS
  if(n >= 3 && isalnum(x[0]) && x[1] == ':' && path_issep(x[2]))
    return 1;
#endif
  return 0;
#endif
}
