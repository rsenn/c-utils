#include "../dir_internal.h"

#if USE_READDIR
#include <dirent.h>
#else
#include <windows.h>
#endif

#if USE_WIDECHAR
#include "../utf8.h"
#endif

char*
dir_name(struct dir_s* d) {
#if !USE_READDIR && USE_WIDECHAR
  size_t n = wcstombs(NULL, dir_INTERNAL(d)->dir_finddata.cFileName, 1);

  if(dir_INTERNAL(d)->tmpname)
    free(dir_INTERNAL(d)->tmpname);

  if((dir_INTERNAL(d)->tmpname = malloc(n + 1)) == NULL)
    return NULL;
  wcstombs(dir_INTERNAL(d)->tmpname, dir_NAME(d), wcslen(dir_NAME(d)));
  dir_INTERNAL(d)->tmpname[n] = '\0';
  return dir_INTERNAL(d)->tmpname;
#else
  return dir_NAME(d);
#endif
}
