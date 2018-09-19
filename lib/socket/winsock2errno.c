#include "../windoze.h"

#if WINDOWS
#include <winsock2.h>
#include <errno.h>
#undef winsock2errno

#if WINDOWS_NATIVE
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#define ETIMEDOUT WSAETIMEDOUT
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EINPROGRESS WSAEINPROGRESS
#define ECONNREFUSED WSAECONNREFUSED
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif

int
winsock2errno(long l) {
  long x;
  if(l == -1)
    switch((x = WSAGetLastError())) {
    case WSANOTINITIALISED:
      printf("WSANOTINITIALISED!\n");
      exit(111);
    case WSAENETDOWN:
      printf("WSAENETDOWN!\n");
      exit(111);
    case WSAEINTR:
      errno = EINTR; break;
    case WSAEBADF:
      errno = EBADF; break;
    case WSAEACCES:
      errno = EACCES; break;
    case WSAEFAULT:
      errno = EFAULT; break;
    case WSAEINVAL:
      errno = EINVAL; break;
    case WSAEMFILE:
      errno = EMFILE; break;
    case WSAENAMETOOLONG:
      errno = ENAMETOOLONG; break;
    case WSAENOTEMPTY:
      errno = ENOTEMPTY; break;
    case WSAEPROTONOSUPPORT:
      errno = EPROTONOSUPPORT; break;
    default:
      errno = x;
      break;
    }
  return l;
}
#else
#include <errno.h>

int winsock2errno(long ret) {
  if(ret == -1) {
    return errno;
  }
  return l;
}
  
#endif
