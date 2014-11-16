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
  CloseHandle(dir_INTERNAL(d)->dir_handle);
#else
  closedir(dir_INTERNAL(d)->dir_handle);
#endif
  free(d->dir_int);
}
