/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef SOCKET_INTERNAL_H
#define SOCKET_INTERNAL_H

#ifdef __ORANGEC__
#undef USE_WS2_32
#endif

#if ((defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)) && !defined(__MSYS__) && !defined(__CYGWIN__)) || defined(WINDOWS_NATIVE) || defined(WINDOWS_OVERRIDE)
# if USE_WS2_32
# define _WINSOCKAPI_
#  include <winsock2.h>
#  include <mswsock.h>
# else
#  include <winsock.h>
# endif
//# include <ws2ipdef.h>
# ifndef _MSC_VER
# include <ws2tcpip.h>
# endif
#endif

#include "windoze.h"
#include "socket.h"

#if !WINDOWS_NATIVE && !WINDOWS_OVERRIDE
# include <sys/param.h>
#  if !WINDOWS || !(defined(_WINSOCKAPI_) || defined(_WINSOCK2API_))
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netdb.h>
#  endif
# include <unistd.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#if WINDOWS_NATIVE || WINDOWS_OVERRIDE
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

#if WINDOWS_NATIVE || WINDOWS_OVERRIDE
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

#if WINDOWS_NATIVE
#define USE_SELECT 1
#endif

#endif