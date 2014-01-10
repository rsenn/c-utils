#if !( defined(__MINGW32__) || defined(__MSYS__))
#include <dirent.h>
#endif
#include "dir_internal.h"

unsigned long dir_time(struct dir_s *d, int time_type)
{
  unsigned long r=0;
#if defined(__MINGW32__) || defined(__MSYS__)
  switch(time_type)
  {
    case DIR_TIME_CREATION: r = ((struct dir_internal_s *)(d->dir_int))->dir_finddata.ftCreationTime; break;
    case DIR_TIME_ACCESS: r = ((struct dir_internal_s *)(d->dir_int))->dir_finddata.ftLastAccessTime; break;
    case DIR_TIME_MODIFICATION: r = ((struct dir_internal_s *)(d->dir_int))->dir_finddata.ftLastWriteTime; break;
  }
#else
  const char *name = ((struct dir_internal_s *)(d->dir_int))->dir_entry->d_name;
  struct stat st;

  lstat(name, &st);

  switch(time_type)
  {
    case DIR_TIME_CREATION: r = st.st_ctime; break;
    case DIR_TIME_ACCESS: r = st.st_atime; break;
    case DIR_TIME_MODIFICATION: r = st.st_mtime; break;
  }
#endif
  return r;
}
