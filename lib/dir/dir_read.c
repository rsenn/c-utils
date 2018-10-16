#include "../dir_internal.h"
#include <assert.h>

#if USE_READDIR
#include <dirent.h>
#else
#include <windows.h>
#endif

char*
dir_read(struct dir_s* d)
{
  char* ret = 0;
#if USE_READDIR
  struct dirent* de;
  assert(dir_INTERNAL(d)->dir_handle);

  if((de = readdir(dir_INTERNAL(d)->dir_handle)) != 0) {
    dir_INTERNAL(d)->dir_entry = de;
    ret = dir_NAME(d);
  }
#else
  if(!dir_INTERNAL(d)->first) {
#if USE_WIDECHAR
    if(!FindNextFileW((HANDLE)dir_INTERNAL(d)->dir_handle, &dir_INTERNAL(d)->dir_finddata))
#else
    if(!FindNextFileA(dir_INTERNAL(d)->dir_handle, &dir_INTERNAL(d)->dir_finddata))
#endif
      return 0;
  }
  dir_INTERNAL(d)->first = 0;

#if USE_WIDECHAR
  ret = dir_name(d);
#else
  ret = dir_INTERNAL(d)->dir_finddata.cFileName;
#endif
#endif
  return ret;
}
