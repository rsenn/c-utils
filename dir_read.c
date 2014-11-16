#include "dir_internal.h"

char *dir_read(struct dir_s *d)
{
  char *ret = 0;
#if defined(WIN32) || defined(__MINGW32__) || defined(__MSYS__)
	  if(!dir_INTERNAL(d)->first)
	  {
		if(!FindNextFileA(dir_INTERNAL(d)->dir_handle, &dir_INTERNAL(d)->dir_finddata))
		  return 0;
	  }
	  dir_INTERNAL(d)->first = 0;
	  
	  ret = dir_INTERNAL(d)->dir_finddata.cFileName;
	#else


	  if((dir_INTERNAL(d)->dir_entry = readdir(dir_INTERNAL(d)->dir_handle)) != 0)
	  {
		  ret = dir_NAME(dir_INTERNAL(d));
		}
	#endif
	  return ret;
}
