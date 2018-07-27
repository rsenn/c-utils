#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include "../io_internal.h"

int io_pipe(int64* d) {
#if defined(_WIN32) || defined(_WIN64)
  HANDLE fds[2];
  if(CreatePipe(fds,fds+1,0,0)==0)
    return 0;
#else
  int fds[2];
  if(pipe(fds)==-1)
    return 0;
#endif
  if(io_fd(fds[1]) && io_fd(fds[0])) {
    d[0]=fds[0];
    d[1]=fds[1];
    return 1;
  }
  io_close(fds[1]);
  io_close(fds[0]);
  return 0;
}
