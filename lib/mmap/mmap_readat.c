#define _POSIX_C_SOURCE 200809
#define _ATFILE_SOURCE

#include <sys/types.h>
#ifndef _WIN32
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../open.h"
#endif
#include "../mmap.h"
#include "../seek.h"

extern const char*
mmap_readat(const char* filename, size_t* filesize, int dirfd) {
#ifdef _WIN32
  return 0;
#else
  int fd = openat(dirfd, filename, O_RDONLY);
  char* map;

  if(fd >= 0) {
    seek_pos o = seek_end(fd);

    if(o == 0 || (sizeof(seek_pos) != sizeof(size_t) && o > (seek_pos)(size_t)-1)) {
      close(fd);
      return 0;
    }
    *filesize = (size_t)o;

    if(o > 0) {
      map = (char*)mmap(0, *filesize, PROT_READ, MAP_SHARED, fd, 0);

      if(map == (char*)-1)
        map = 0;
    } else
      map = "";
    close(fd);
    return map;
  }
  return 0;
#endif
}
