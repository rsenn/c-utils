#if !(defined(WIN32) || defined(__MINGW32__) || defined(__MSYS__))
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "dir.h"
#include "dir_internal.h"

unsigned long long dir_time(struct dir_s *d, int time_type)
{
  unsigned long long r=0;
#if defined(WIN32) || defined(__MINGW32__) || defined(__MSYS__)
  switch(time_type)
  {
    case D_TIME_CREATION: r = *(unsigned long long *)&dir_INTERNAL(d)->dir_finddata.ftCreationTime; break;
    case D_TIME_ACCESS: r = *(unsigned long long *)&dir_INTERNAL(d)->dir_finddata.ftLastAccessTime; break;
    case D_TIME_MODIFICATION: r = *(unsigned long long *)&dir_INTERNAL(d)->dir_finddata.ftLastWriteTime; break;
  }
#else
  const char *name = dir_INTERNAL(d)->dir_entry->d_name;
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
