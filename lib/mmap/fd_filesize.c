#include "../windoze.h"
#include "../mmap.h"
#include "../seek.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/stat.h>
#endif

ssize_t
fd_filesize(fd_type fd) {
  ssize_t size;
  seek_pos pos;

  if((pos = seek_cur(fd)) >= 0) {
    size = seek_end(fd);

    if(seek_set(fd, pos) == pos)
      if(size >= 0)
        return size;
  }

#if WINDOWS_NATIVE
  size = GetFileSize((HANDLE)fd, NULL);
#else
  struct stat st;
  if(fstat(fd, &st) < 0)
    return -1;

  size = st.st_size;
#endif

  return size;
}
