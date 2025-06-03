#include "../windoze.h"
#include "../unix.h"

#if WINDOWS_NATIVE
#include <windows.h>
#include <io.h>
int
ftruncate(int fd, long length) {
  long pos = lseek64(fd, 0, SEEK_CUR);

  if(lseek64(fd, length, SEEK_SET) == length) {
    HANDLE h = (HANDLE)_get_osfhandle(fd);

    if(SetEndOfFile(h)) {
      if(pos > length)
        pos = length;
      lseek64(fd, pos, SEEK_SET);
      return 0;
    }
  }
  return -1;
}
#endif /* !defined(HAVE_FTRUNCATE) */
