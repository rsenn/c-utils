/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef SOCKET_INTERNAL_H
#define SOCKET_INTERNAL_H

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MSYS__) || defined(__MINGW32__)

#ifdef __ORANGEC__
#undef USE_WS2_32
#endif

#if((defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)) && !defined(__CYGWIN__)) || WINDOWS_NATIVE || defined(WINDOWS_OVERRIDE)
#ifdef USE_WS2_32
#define _WINSOCKAPI_
#include <winsock2.h>
#else
#include <winsock.h>
#endif
//# include <ws2ipdef.h>
#ifndef _MSC_VER
//# include <ws2tcpip.h>
#endif
#endif

#include "windoze.h"
#endif

#include "socket.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#if WINDOWS && WINDOWS_OVERRIDE && !defined(WINDOWS_NATIVE)
#define WINDOWS_NATIVE 1
#endif
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

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
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#endif
#endif
#endif

#if !WINDOWS_NATIVE //&& !WINDOWS_OVERRIDE
#include <sys/param.h>
#if !(defined(_WINSOCKAPI_) || defined(_WINSOCK2API_))
#include <sys/socket.h>
#include <netinet/in.h>
#define __INSIDE_CYGWIN_NET__ 1
#ifndef __wasi__
#include <netdb.h>
#endif
#endif
#include <unistd.h>
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
#if !defined(_SYS_SOCKET_H)
typedef int socklen_t;
#endif
#endif

#if WINDOWS_NATIVE
#define USE_SELECT 1
#endif

#if 0 // defined(__POCC__) && defined(_M_AMD64)
#define __WSAFDIsSet _WSAFDIsSet
#endif

#endif
