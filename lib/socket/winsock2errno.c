#define USE_WS2_32 1
#include "../windoze.h"

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"

#if WINDOWS_NATIVE
#include <errno.h>
#undef winsock2errno

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef ENAMETOOLONG
#define ENAMETOOLONG 91 /* File or path name too long */
#endif

#ifndef ENOTEMPTY
#define ENOTEMPTY 90 /* Directory not empty */
#endif

#include "../buffer.h"

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif /* !defined(EPROTONOSUPPORT) */

#ifndef ETIMEDOUT
#define ETIMEDOUT WSAETIMEDOUT
#endif /* !defined(ETIMEDOUT) */

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif /* !defined(EWOULDBLOCK) */

#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif /* !defined(EINPROGRESS) */

#ifndef ECONNREFUSED
#define ECONNREFUSED WSAECONNREFUSED
#endif /* !defined(ECONNREFUSED) */

int
winsock2errno(long l) {
  long x;

  if(l == -1)

    switch((x = WSAGetLastError())) {
      case WSANOTINITIALISED:
        // buffer_putsflush(buffer_1, "WSANOTINITIALISED!\n");
        exit(111);
      case WSAENETDOWN:
        // buffer_putsflush(buffer_1, "WSAENETDOWN!\n");
        exit(112);
      case WSAEINTR: errno = EINTR; break;
      case WSAEBADF: errno = EBADF; break;
      case WSAEACCES: errno = EACCES; break;
      case WSAEFAULT: errno = EFAULT; break;
      case WSAEINVAL: errno = EINVAL; break;
      case WSAEMFILE: errno = EMFILE; break;
      case WSAENAMETOOLONG: errno = ENAMETOOLONG; break;
      case WSAENOTEMPTY: errno = ENOTEMPTY; break;
      case WSAEPROTONOSUPPORT: errno = EPROTONOSUPPORT; break;
      case WSAEWOULDBLOCK: errno = EWOULDBLOCK; break;
      case WSAEINPROGRESS: errno = EINPROGRESS; break;
      default: errno = x; break;
    }
  return l;
}
/*#else
#include <errno.h>
int winsock2errno(long ret) {
  if(ret == -1) {
    return errno;
  }
  return ret;
}*/
#endif
