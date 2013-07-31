#include "dir.h"

void dir_close(struct dir *d)
{
#ifdef __MINGW32__
  closedir(d->dir_handle);
#else
  CloseHandle(d->dir_handle);
#endif
}
