#include "dir_internal.h"

char *dir_read(struct dir *d)
{
  char *ret = 0;
#if defined(__MINGW32__) || defined(__MSYS__)
  if(!d->first)
  {
    if(!FindNextFileA(d->dir_handle, &d->dir_finddata))
	  return 0;
  }
  d->first = 0;
  
  ret = d->dir_finddata.cFileName;
#else
  if(d->dir_entry = readdir(d->dir_handle))
  {
    ret = d->dir_entry->d_name;
	}
#endif
  return ret;
}
