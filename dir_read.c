#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include "dir_internal.h"

char* dir_read(struct dir_s* d)
{
  char* ret = 0;
#ifdef USE_READDIR
    assert(dir_INTERNAL(d)->dir_handle);

    if((dir_INTERNAL(d)->dir_entry = readdir(dir_INTERNAL(d)->dir_handle)) != 0)
    {
      ret = dir_NAME(dir_INTERNAL(d));
    }
#else
    if(!dir_INTERNAL(d)->first)
    {
    if(!FindNextFileA(dir_INTERNAL(d)->dir_handle, &dir_INTERNAL(d)->dir_finddata))
      return 0;
    }
    dir_INTERNAL(d)->first = 0;
    
    ret = dir_INTERNAL(d)->dir_finddata.cFileName;
#endif
    return ret;
}
