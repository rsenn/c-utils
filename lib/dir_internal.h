#ifndef _DIR_INTERNAL_H__
#define _DIR_INTERNAL_H__

#include "dir.h"

#define _POSIX_ 1

#ifdef _MSC_VER
#include <crtdefs.h>
#endif

#if USE_READDIR
# include <dirent.h>
#else
//# ifdef _WIN32
#  include <windows.h>
//# endif
# include <limits.h>
#endif

#ifdef __MSYS__
#define intptr_t __PTRDIFF_TYPE__
#endif

//#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

#ifndef MAXIMUM_PATH_LENGTH
//#define MAXIMUM_PATH_LENGTH _MAX_PATH
#define MAXIMUM_PATH_LENGTH 4095
#endif

struct dir_internal_s {
#if USE_READDIR
  DIR *dir_handle;
  struct dirent* dir_entry;
#else
  intptr_t dir_handle;
#if USE_WIDECHAR
  WIN32_FIND_DATAW dir_finddata;
  char* tmpname;
#else
  WIN32_FIND_DATAA dir_finddata;
#endif 
  int first;
#endif

};

#ifndef _S_IFDIR
#define _S_IFDIR  0040000 /* directory */
#endif // !defined(_S_IFDIR)

#ifndef _S_IFCHR
#define _S_IFCHR  0020000 /* character special */
#endif // !defined(_S_IFCHR)

#ifndef S_IFBLK
#define S_IFBLK  0060000 /* block special */
#endif // !defined(S_IFBLK)

#ifndef S_IFREG
#define S_IFREG  0100000 /* regular */
#endif // !defined(S_IFREG)

#ifndef S_IFLNK
#define S_IFLNK  0120000 /* symbolic link */
#endif // !defined(S_IFLNK)

#ifndef S_IFSOCK
#define S_IFSOCK 0140000 /* socket */
#endif // !defined(S_IFSOCK)

#ifndef S_IFIFO
#define S_IFIFO  0010000 /* fifo */
#endif // !defined(S_IFIFO)

#if USE_READDIR
#ifndef _WIN32
#define USE_LSTAT 1
#endif
#endif


#define dir_INTERNAL(d) ((struct dir_internal_s *)((d)->dir_int))

#if USE_READDIR
#define dir_NAME(d) (dir_INTERNAL(d)->dir_entry->d_name)
#else
#define dir_NAME(d) (dir_INTERNAL(d)->dir_finddata.cFileName)
#endif

#if USE_READDIR
#define dir_TYPE(d) (dir_INTERNAL(d)->dir_entry->d_type)
#else
#define dir_TYPE(d) (dir_INTERNAL(d)->dir_finddata.cFileType)
#endif

#if USE_READDIR
#define dir_ATTRS(d) (dir_INTERNAL(d)->dir_entry->st_mode)
#else
#define dir_ATTRS(d) (dir_INTERNAL(d)->dir_finddata.dwFileAttributes)
#endif

#if USE_READDIR
#define dir_ISDIR(d) S_ISDIR(dir_ATTRS(d))
#else
#define dir_ISDIR(d) (!!(dir_ATTRS(d) & 0x10))
#endif

#if USE_READDIR
#define dir_ISLINK(d) S_ISLINK(dir_ATTRS(d))
#else
#define dir_ISLINK(d) (!!(dir_ATTRS(d) & 0x08))
#endif

#if USE_READDIR
#define dir_ISLINK(d) S_ISLINK(dir_ATTRS(d))
#else
#define dir_ISLINK(d) (!!(dir_ATTRS(d) & 0x08))
#endif

#endif // _DIR_INTERNAL_H__
