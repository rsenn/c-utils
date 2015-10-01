#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dir_internal.h"

#ifdef USE_READDIR
#include <dirent.h>
#ifdef __MINGW32__
#include <sys/stat.h>
#include <io.h>
#include <windows.h>
#endif
#endif

static unsigned 
dir_fileattr(const char *p) {
 unsigned int r = 0;
#ifdef USE_LSTAT
 struct stat st;
 
  if(lstat(dir_INTERNAL(d)->dir_entry->d_name, &st) == 0) {
	if(S_IFDIR(st.st_mode)) r |= D_DIRECTORY;
  else if(S_IFLNK(st.st_mode)) r |= D_SYMLINK;
  else if(S_IFREG(st.st_mode)) r |= D_FILE;

#else
DWORD attr = GetFileAttributes(p);
  if(attr & FILE_ATTRIBUTE_DIRECTORY)
   r |= D_DIRECTORY;
  else
   r |= D_FILE;
  if(attr & FILE_ATTRIBUTE_REPARSE_POINT)
    r |= D_SYMLINK;
  
#endif
  return r;
  }

int dir_type(struct dir_s* d)
{
  int r = 0;
#ifdef USE_READDIR
#ifndef DT_DIR
#define DT_DIR 4
#endif
#ifndef DT_REG
#define DT_REG 8
#endif
#ifndef DT_LNK
#define DT_LNK 10
#endif

#ifndef __MINGW32__
  switch((dir_INTERNAL(d)->dir_entry->d_type)) {
    case DT_DIR: {
      r |= D_DIRECTORY;
      break;
    }

    case DT_REG: {
      r |= D_FILE;
      break;
    }
    case DT_LNK: {
    r |= D_SYMLINK;
    break; 
   }
    case 0:
    default: {
               break;
    }
  }   
#else
  r = dir_fileattr(dir_INTERNAL(d)->dir_entry->d_name);
#endif
 
#else
  if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
    r |= D_SYMLINK;

  if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x10)
    r |= D_DIRECTORY;
  else if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x20)
    r |= D_FILE;
#endif
  return r;
}
