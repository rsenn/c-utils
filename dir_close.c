#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dir_internal.h"

#if USE_READDIR
#include <dirent.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>


void dir_close(struct dir_s* d)
{
#if USE_READDIR
  closedir(dir_INTERNAL(d)->dir_handle);
#else
  FindClose(dir_INTERNAL(d)->dir_handle);
#endif
  free(d->dir_int);
}
