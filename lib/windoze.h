#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MSYS__)
#if !(defined(__MSYS__) || defined(__CYGWIN__))
#define WINDOWS_NATIVE 1
#endif
#define WINDOWS 1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define MINGW 1
#endif

#if WINDOWS
#include <io.h>
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if WINDOWS_NATIVE
size_t getpagesize();

#define getpid() GetCurrentProcessId()
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH

#endif /* WINDOWS_NATIVE */

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);
#else
#define winsock2errno(fnord) (fnord)
#define __winsock_init()
#define closesocket(s) close(s)
#endif

#ifdef __cplusplus
}
#endif
#endif /* defined(WINDOZE_H) */

