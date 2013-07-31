#include "dir_internal.h"

#if defined(__MINGW32__) ||defined(__MSYS__)
#include <windows.h>
#else
#include <dirent.h>
#endif


void dir_close(struct dir *d)
{
#if defined(__MINGW32__) || defined(__MSYS__)
  CloseHandle(d->dir_handle);
#else
  closedir(d->dir_handle);
#endif
}
