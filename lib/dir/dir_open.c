#include <assert.h>
#include "../dir_internal.h"

#if USE_WIDECHAR
#include "../utf8.h"
#endif

int dir_open(struct dir_s* d, const char* p) {
  int ret;
  if(!(d->dir_int = malloc(sizeof(struct dir_internal_s))))
    return 1;
#if USE_READDIR
  ret = ((dir_INTERNAL(d)->dir_handle = opendir(p)) == NULL) ? -1 : 0;
#else
  {
    char path[MAXIMUM_PATH_LENGTH + 1];
    size_t len;
    strncpy(path, p, sizeof(path) - 1);
    path[MAXIMUM_PATH_LENGTH] = '\0';
    len = str_len(path);
    strncat(path, (len > 0 && (path[len - 1] == '\\' || path[len - 1] == '/')) ? "*" : "\\*", sizeof(path) - 1);

#if USE_WIDECHAR
    {
      size_t wlen = u8swcslen(path);
      wchar_t wpath[wlen + 1];
      u8stowcs(wpath, path, wlen);
      wpath[wlen] = '\0';
      dir_INTERNAL(d)->dir_handle = FindFirstFileW(wpath, &dir_INTERNAL(d)->dir_finddata);
    }
#else
    dir_INTERNAL(d)->dir_handle = FindFirstFileA(path, &dir_INTERNAL(d)->dir_finddata);
#endif
    dir_INTERNAL(d)->first = 1;

    ret = (dir_INTERNAL(d)->dir_handle ==  INVALID_HANDLE_VALUE);
  }
#endif

  //assert(dir_INTERNAL(d)->dir_handle);

  return ret;
}
