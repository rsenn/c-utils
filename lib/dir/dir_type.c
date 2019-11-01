#if USE_READDIR || !(defined(_WIN32) || defined(_WIN32) || defined(__MSYS__))
#include <dirent.h>
#endif

#include "../dir_internal.h"

#if !defined(_DIRENT_HAVE_D_TYPE) && (defined(__MSYS__) || defined(__CYGWIN__))
#include "../buffer.h"
#include "../stralloc.h"

#include <sys/stat.h>
#endif
#ifndef FILE_ATTRIBUTE_REPARSE_POINT
#define FILE_ATTRIBUTE_REPARSE_POINT 0x400
#endif

int
dir_type(struct dir_s* d) {
  int r = 0;
#if !USE_READDIR && (defined(_WIN32) || defined(_WIN32) || defined(__MSYS__))
  if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
    r |= D_SYMLINK;
  else if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x10)
    r |= D_DIRECTORY;
  else if(dir_INTERNAL(d)->dir_finddata.dwFileAttributes & 0x20)
    r |= D_FILE;
#else
#ifndef DT_DIR
#define DT_DIR 4
#endif

#ifndef DT_REG
#define DT_REG 8
#endif

#ifndef DT_LNK
#define DT_LNK 10
#endif

#if defined(_DIRENT_HAVE_D_TYPE) || (!defined(__MSYS__) && !defined(__CYGWIN__))
  switch((dir_TYPE(d))) {
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
    default: { break; }
  }
#else
  {
    stralloc sa;
    struct stat st;
    DIR* dh = dir_INTERNAL(d)->dir_handle;

    stralloc_init(&sa);
    dir_path(d, &sa);
    stralloc_nul(&sa);

    if(lstat(sa.s, &st) != -1) {
      if(S_ISLNK(st.st_mode))
        r |= D_SYMLINK;
      else if(S_ISDIR(st.st_mode))
        r |= D_DIRECTORY;
      else if(S_ISREG(st.st_mode))
        r |= D_FILE;
    }

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "dir_type path: ");
    buffer_putsa(buffer_2, &sa);
    buffer_putnlflush(buffer_2);
#endif

    //      printf("dh: %p __d_dirname: %s\n", dh, dh->__d_dirname);

    stralloc_free(&sa);
  }
//#error No dirent type method
#endif

#endif
  return r;
}
