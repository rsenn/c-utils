#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include "../io_internal.h"

int
io_pipe(fd_t* d) {
#if defined(_WIN32) || defined(_WIN64)
  HANDLE fds[2];
  if(CreatePipe(fds, fds + 1, 0, 0) == 0) return 0;
#else
  fd_t fds[2];
  if(pipe(fds) == -1) return 0;
#endif
  if(io_fd((size_t)fds[1]) && io_fd((size_t)fds[0])) {
    d[0] = (size_t)fds[0];
    d[1] = (size_t)fds[1];
    return 1;
  }
  io_close((size_t)fds[1]);
  io_close((size_t)fds[0]);
  return 0;
}
