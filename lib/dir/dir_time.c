#include "../dir.h"
#include "../dir_internal.h"

#if USE_READDIR
#include <dirent.h>
#include <sys/stat.h>
#endif

#if _FILE_OFFSET_BITS == 64
typedef struct stat stat_t;
#else
typedef struct stat stat_t;
#endif

#if !USE_READDIR && WINDOWS
static uint64
filetime_to_unix(const FILETIME* ft) {
  uint64 windowsTicks = ((uint64)ft->dwHighDateTime << 32) + ft->dwLowDateTime;
  return (uint64)(windowsTicks / 10000000 - SEC_TO_UNIX_EPOCH);
}
#endif

unsigned long
dir_time(struct dir_s* d, int type) {
  unsigned long r = 0;
#if USE_READDIR
  const char* name = dir_INTERNAL(d)->dir_entry->d_name;
  stat_t st;

  lstat(name, &st);

  switch(type) {
    case D_TIME_CREATION: r = st.st_ctime; break;
    case D_TIME_ACCESS: r = st.st_atime; break;
    case D_TIME_MODIFICATION: r = st.st_mtime; break;
  }
#elif WINDOWS
  switch(type) {
    case D_TIME_CREATION: r = filetime_to_unix(&dir_INTERNAL(&d)->dir_finddata.ftCreationTime); break;
    case D_TIME_ACCESS: r = filetime_to_unix(&dir_INTERNAL(&d)->dir_finddata.ftLastAccessTime); break;
    case D_TIME_MODIFICATION: r = filetime_to_unix(&dir_INTERNAL(&d)->dir_finddata.ftLastWriteTime); break;
  }
#endif

  return r;
}
