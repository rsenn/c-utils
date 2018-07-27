#include <sys/types.h>

#if !(defined(_WIN32) || defined(_WIN64))
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "../windoze.h"
#include <errno.h>
#include "../io_internal.h"

int io_socketpair(int64* d) {
  int fds[2];
  __winsock_init();
#ifdef AF_UNIX
  if(socketpair(AF_UNIX,SOCK_STREAM,0,fds)==-1)
#endif
#ifdef AF_INET6
    if(socketpair(AF_INET6,SOCK_STREAM,IPPROTO_TCP,fds)==-1)
#endif
      if(socketpair(AF_INET,SOCK_STREAM,IPPROTO_TCP,fds)==-1)
	return 0;
  if(io_fd(fds[1]) && io_fd(fds[0])) {
    d[0]=fds[0];
    d[1]=fds[1];
    return 1;
  }
  io_close(fds[1]);
  io_close(fds[0]);
  return 0;
}
