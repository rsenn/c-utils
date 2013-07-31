#ifndef _DIR_H__
#define _DIR_H__

#if defined(__MINGW32__) || defined(__MSYS__)
#include <windows.h>
#else
#include <dirent.h>
#endif

#if defined(__MINGW32__) || defined(__MSYS__)
#define PATHSEP_C '\\'
#define PATHSEP_S "\\"
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#endif
#define IS_PATHSEP(c) ((c)=='\\'||(c)=='/')

struct dir {
#if defined(__MINGW32__) || defined(__MSYS__)
  HANDLE dir_handle;
  WIN32_FIND_DATAA dir_finddata;
  int first;
#else
  DIR *dir_handle;
  struct dirent *dir_entry;
#endif

};

#if defined(__MINGW32__) || defined(__MSYS__)
#define dir_ATTRS(d) ((d)->dir_finddata.dwFileAttributes)
#else
#define dir_ATTRS(d) ((d)->dir_entry->st_mode)
#endif

#if defined(__MINGW32__) || defined(__MSYS__)
#define dir_ISDIR(d) (!!(dir_ATTRS(d)&0x10))
#else
#define dir_ISDIR(d) S_ISDIR(dir_ATTRS(d))
#endif



int dir_open(struct dir *d, const char *p);
char *dir_read(struct dir *d);
void dir_close(struct dir *d);

#endif // _DIR_H__
