#include "../windoze.h"
#include "../dir_internal.h"
#include <stdlib.h>

#if USE_READDIR
#include <dirent.h>
#else
#include <windows.h>
#endif

void
dir_close(struct dir_s* d) {
  if(dir_INTERNAL(d)->dir_handle) {
#if USE_READDIR
    closedir(dir_INTERNAL(d)->dir_handle);
#else
#if USE_WIDECHAR
    if(dir_INTERNAL(d)->tmpname)
      free(dir_INTERNAL(d)->tmpname);
#endif
    FindClose((HANDLE)dir_INTERNAL(d)->dir_handle);
    free(dir_INTERNAL(d)->dir_path);
#endif
  }
  free(d->dir_int);
  d->dir_int = NULL;
}
