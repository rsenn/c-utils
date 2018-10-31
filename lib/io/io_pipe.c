#include "../windoze.h"
#include "../io_internal.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>

int
io_pipe(fd_t* d) {
#if WINDOWS_NATIVE

  SECURITY_ATTRIBUTES security_attributes;

  HANDLE fds[2];

  security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  security_attributes.bInheritHandle = TRUE;
  security_attributes.lpSecurityDescriptor = 0;

  if(CreatePipe(fds, fds + 1, &security_attributes, 0) == 0) return 0;
  d[0] = _open_osfhandle(fds[0], _O_RDONLY|_O_BINARY);
  d[1] = _open_osfhandle(fds[1], _O_WRONLY|_O_BINARY);
  return 1;
#else
  fd_t fds[2];
  if(pipe(fds) == -1) return 0;
 if(io_fd((size_t)fds[1]) && io_fd((size_t)fds[0])) {
    d[0] = (size_t)fds[0];
    d[1] = (size_t)fds[1];
    return 1;
  }
  io_close((size_t)fds[1]);
  io_close((size_t)fds[0]);
#endif
  return 0;
}
