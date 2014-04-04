#if !( defined(__MINGW32__) || defined(__MSYS__))
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "dir.h"
#include "dir_internal.h"

unsigned long long dir_time(struct dir_s *d, int time_type)
{
  unsigned long long r=0;
#if defined(__MINGW32__) || defined(__MSYS__)
  switch(time_type)
  {
    case D_TIME_CREATION: r = *(unsigned long long *)&((struct dir_internal_s *)(d->dir_int))->dir_finddata.ftCreationTime; break;
    case D_TIME_ACCESS: r = *(unsigned long long *)&((struct dir_internal_s *)(d->dir_int))->dir_finddata.ftLastAccessTime; break;
    case D_TIME_MODIFICATION: r = *(unsigned long long *)&((struct dir_internal_s *)(d->dir_int))->dir_finddata.ftLastWriteTime; break;
  }
#else
  const char *name = ((struct dir_internal_s *)(d->dir_int))->dir_entry->d_name;
  struct stat st;

  lstat(name, &st);

  switch(time_type)
  {
    case D_TIME_CREATION: r = st.st_ctime; break;
    case D_TIME_ACCESS: r = st.st_atime; break;
    case D_TIME_MODIFICATION: r = st.st_mtime; break;
  }
#endif
  return r;
}
