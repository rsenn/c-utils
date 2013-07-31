#include "dir_internal.h"

#if defined(__MINGW32__) ||defined(__MSYS__)
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <stdlib.h>


void dir_close(struct dir_s *d)
{
#if defined(__MINGW32__) || defined(__MSYS__)
  CloseHandle(((struct dir_internal_s *)(d->dir_int))->dir_handle);
#else
  closedir(((struct dir_internal_s *)(d->dir_int))->dir_handle);
#endif
  free(d->dir_int);
}
