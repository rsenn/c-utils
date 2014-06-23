#include "dir_internal.h"

char *dir_read(struct dir_s *d)
{
  char *ret = 0;
#if defined(__MINGW32__) || defined(__MSYS__)
	  if(!((struct dir_internal_s *)(d->dir_int))->first)
	  {
		if(!FindNextFileA(((struct dir_internal_s *)(d->dir_int))->dir_handle, &((struct dir_internal_s *)(d->dir_int))->dir_finddata))
		  return 0;
	  }
	  ((struct dir_internal_s *)(d->dir_int))->first = 0;
	  
	  ret = ((struct dir_internal_s *)(d->dir_int))->dir_finddata.cFileName;
	#else


	  if((((struct dir_internal_s *)(d->dir_int))->dir_entry = readdir(((struct dir_internal_s *)(d->dir_int))->dir_handle)) != 0)
	  {
		ret = ((struct dir_internal_s *)(d->dir_int))->dir_entry->d_name;
		}
	#endif
	  return ret;
}
