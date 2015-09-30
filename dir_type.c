#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_READDIR
#include <dirent.h>
#endif
#include "dir_internal.h"

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
  if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
    r |= D_SYMLINK;

  if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x10)
    r |= D_DIRECTORY;
  else if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x20)
    r |= D_FILE;
#endif
  return r;
}
