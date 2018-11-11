/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef SOCKET_INTERNAL_H
#define SOCKET_INTERNAL_H

#ifdef __ORANGEC__
#undef USE_WS2_32
#endif

#if defined(_WIN32) || defined(_WIN64)
# if USE_WS2_32
#  ifndef _WINSOCKAPI_
#   define _WINSOCKAPI_
#  endif
#  include <winsock2.h>
# else
#  include <winsock.h>
# endif
#endif

#include "windoze.h"
#include "socket.h"

#if !WINDOWS_NATIVE
#include <sys/param.h>
# if !defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <netdb.h>
# endif
#include <unistd.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#if WINDOWS_NATIVE
#include <windows.h>

#ifndef EAGAIN
#define EAGAIN 11
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT WSAETIMEDOUT
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED WSAECONNREFUSED
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif
#endif

#if WINDOWS_NATIVE

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);
#else
#define winsock2errno(fnord) (fnord)
#define __winsock_init()
#define closesocket(s) close(s)
#endif

#if WINDOWS
# if !defined(_SYS_SOCKET_H)
typedef int socklen_t;
# endif
#endif

#endif

