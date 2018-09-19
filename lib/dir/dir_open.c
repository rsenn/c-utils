#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* defined(HAVE_CONFIG_H) */

#include "../dir_internal.h"
#include "../str.h"
#include <assert.h>

#if USE_WIDECHAR
#include "../utf8.h"
#endif

int
dir_open(struct dir_s* d, const char* p) {
  int ret;
  if(!(d->dir_int = malloc(sizeof(struct dir_internal_s)))) return 1;
#if USE_READDIR
  ret = ((dir_INTERNAL(d)->dir_handle = opendir(p)) == NULL) ? -1 : 0;
#else
  {
    char path[MAXIMUM_PATH_LENGTH + 1];
    size_t len;
    str_copyn(path, p, sizeof(path) - 1);
    len = str_len(path);
    strncat(path, (len > 0 && (path[len - 1] == '\\' || path[len - 1] == '/')) ? "*" : "\\*", sizeof(path) - 1);

#if USE_WIDECHAR
    {
      size_t wlen = u8swcslen(path);
      wchar_t wpath[wlen + 1];
      u8stowcs(wpath, path, wlen);
      wpath[wlen] = '\0';
      dir_INTERNAL(d)->dir_handle = (intptr_t)FindFirstFileW(wpath, &dir_INTERNAL(d)->dir_finddata);
    }
    dir_INTERNAL(d)->tmpname = NULL;
#else
    dir_INTERNAL(d)->dir_handle = (intptr_t)FindFirstFileA(path, &dir_INTERNAL(d)->dir_finddata);
#endif
    dir_INTERNAL(d)->first = 1;
    
    ret = (dir_INTERNAL(d)->dir_handle == (intptr_t)INVALID_HANDLE_VALUE);
  }
#endif

  /* assert(dir_INTERNAL(d)->dir_handle); */

  return ret;
}
