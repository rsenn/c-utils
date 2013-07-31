#include "dir.h"

int dir_open(struct dir *d, const char *p)
{
  int ret;
#if defined(__MINGW32__) || defined(__MSYS__)
  char path[MAX_PATH+1];
  size_t len;
  strncpy(path, p, sizeof(path)-1);
  path[MAX_PATH]='\0';
  len = strlen(path);
  strncat(path, (len > 0 && (path[len -1] == '\\' || path[len - 1] == '/')) ? "*" : "\\*", sizeof(path)-1);
  
  d->dir_handle = FindFirstFileA(path, &d->dir_finddata);
  d->first = 1;
  ret = (d->dir_handle ==  INVALID_HANDLE_VALUE);
#else
  ret = !(d->dir_handle = opendir(p));
#endif
return ret;
}
