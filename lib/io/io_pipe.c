#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include "../io_internal.h"

int
io_pipe(int64* d) {
#if defined(_WIN32) || defined(_WIN64)
  HANDLE fds[2];
  if(CreatePipe(fds, fds + 1, 0, 0) == 0) return 0;
#else
  int fds[2];
  if(pipe(fds) == -1) return 0;
#endif
  if(io_fd((intptr_t)fds[1]) && io_fd((intptr_t)fds[0])) {
    d[0] = (intptr_t)fds[0];
    d[1] = (intptr_t)fds[1];
    return 1;
  }
  io_close((intptr_t)fds[1]);
  io_close((intptr_t)fds[0]);
  return 0;
}
