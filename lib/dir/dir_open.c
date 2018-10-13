#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* defined(HAVE_CONFIG_H) */

#include "../dir_internal.h"
#include "../str.h"
#include <assert.h>

#if USE_WIDECHAR
#include "../utf8.h"
#endif
static const char* last_error_str();

int
dir_open(struct dir_s* d, const char* p) {
  int ret;
  if(!(d->dir_int = malloc(sizeof(struct dir_internal_s)))) return 1;
#if USE_READDIR
  ret = ((dir_INTERNAL(d)->dir_handle = opendir(p)) == NULL) ? -1 : 0;
#else
  {
    HANDLE h;
    char path[MAXIMUM_PATH_LENGTH + 1];
    size_t len;
    str_copyn(path, p, sizeof(path) - 1);
    len = str_len(path);
    strncat(path,
            (len > 0 && (path[len - 1] == '\\' || path[len - 1] == DIRSEP_C)) ? "\\*" : "\\\\*",
            sizeof(path) - 1);

#if USE_WIDECHAR
    {
      size_t wlen = u8swcslen(path);
      wchar_t wpath[wlen + 1];
      u8stowcs(wpath, path, wlen);
      wpath[wlen] = '\0';
      h = (intptr_t)FindFirstFileW(wpath, &dir_INTERNAL(d)->dir_finddata);
    }
    dir_INTERNAL(d)->tmpname = NULL;
#else
    h = FindFirstFileA(path, &dir_INTERNAL(d)->dir_finddata);
#endif
    dir_INTERNAL(d)->first = 1;
    dir_INTERNAL(d)->dir_handle = h;
    ret = (h == INVALID_HANDLE_VALUE);

    /*	if(ret) {
            DWORD error = GetLastError();
            printf("ERROR: %s\n", last_error_str());
        }*/
  }
#endif

  /* assert(dir_INTERNAL(d)->dir_handle); */

  return ret;
}

#if !USE_READDIR
static const char*
last_error_str() {
  DWORD errCode = GetLastError();
  static char tmpbuf[1024];
  char* err;
  tmpbuf[0] = '\0';
  if(errCode == 0) return tmpbuf;
  SetLastError(0);
  if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* default language */
                    (LPTSTR)&err,
                    0,
                    NULL))
    return 0;
  _snprintf(tmpbuf, sizeof(tmpbuf), "ERROR: %s\n", err);
  /* or otherwise log it */
  // OutputDebugString(tmpbuf);
  LocalFree(err);
  return tmpbuf;
}
#endif
