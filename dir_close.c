#include "dir.h"

void dir_close(struct dir *d)
{
#if defined(__MINGW32__) || defined(__MSYS__)
  closedir(d->dir_handle);
#else
  CloseHandle(d->dir_handle);
#endif
}
