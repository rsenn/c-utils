#include "../windoze.h"

#if WINDOWS_NATIVE

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

/* mingw/MSVC don't declare or implement plain POSIX pipe(2) at all (only
 * the non-standard _pipe(), with a different signature); provide it so
 * code written against the POSIX API (rather than this library's own
 * io_pipe()) still links on WINDOWS_NATIVE. */
int
pipe(int fd[2]) {
  SECURITY_ATTRIBUTES security_attributes;
  HANDLE h[2];

  security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  security_attributes.bInheritHandle = TRUE;
  security_attributes.lpSecurityDescriptor = 0;

  if(!CreatePipe(&h[0], &h[1], &security_attributes, 0)) {
    errno = EMFILE;
    return -1;
  }

  fd[0] = _open_osfhandle((intptr_t)h[0], _O_RDONLY | _O_BINARY);
  fd[1] = _open_osfhandle((intptr_t)h[1], _O_WRONLY | _O_BINARY);

  if(fd[0] == -1 || fd[1] == -1) {
    if(fd[0] == -1)
      CloseHandle(h[0]);
    else
      _close(fd[0]);

    if(fd[1] == -1)
      CloseHandle(h[1]);
    else
      _close(fd[1]);

    errno = EMFILE;
    return -1;
  }

  return 0;
}

#endif
