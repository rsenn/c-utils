#include "dir_internal.h"
#include <stdlib.h>

int dir_open(struct dir_s *d, const char *p)
{
  int ret;
  if(!(d->dir_int = malloc(sizeof(struct dir_internal_s))))
  return 1;
#if defined(__MINGW32__) || defined(__MSYS__)
  char path[MAX_PATH+1];
  size_t len;
  strncpy(path, p, sizeof(path)-1);
  path[MAX_PATH]='\0';
  len = strlen(path);
  strncat(path, (len > 0 && (path[len -1] == '\\' || path[len - 1] == '/')) ? "*" : "\\*", sizeof(path)-1);
  
  dir_INTERNAL(d)->dir_handle = FindFirstFileA(path, &dir_INTERNAL(d)->dir_finddata);
  dir_INTERNAL(d)->first = 1;
  ret = (dir_INTERNAL(d)->dir_handle ==  INVALID_HANDLE_VALUE);
#else
  ret = !(dir_INTERNAL(d)->dir_handle = opendir(p));
#endif
return ret;
}
