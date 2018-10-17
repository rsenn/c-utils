/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef SOCKET_INTERNAL_H
#define SOCKET_INTERNAL_H

#include "socket.h"
#include "windoze.h"

#if !WINDOWS_NATIVE
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#if USE_WS2_32 
#include <winsock2.h>
#else
#include <winsock.h>
#endif
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#if WINDOWS_NATIVE
#include <windows.h>
typedef int socklen_t;

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

#endif

