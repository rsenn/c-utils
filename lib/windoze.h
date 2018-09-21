#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(_WIN64)
# if !(defined(__MSYS__) || defined(__CYGWIN__))
#  define WINDOWS_NATIVE 1
# endif
# define WINDOWS 1
#endif

#if WINDOWS_NATIVE
#include <winsock2.h>
#endif

#if WINDOWS || defined(__MSYS__)

# include <io.h>
# include <windows.h>
# ifdef __cplusplus
extern "C" {
# endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED 1
typedef SIZE_T size_t;
#endif

#if !defined(__ssize_t_defined) && !defined(_SSIZE_T_DECLARED) && !defined(_SSIZE_T_DEFINED)
#define __ssize_t_defined 1
#define _SSIZE_T_DECLARED 1
#define _SSIZE_T_DEFINED 1
typedef SSIZE_T ssize_t;
#endif

typedef int socklen_t;

#if WINDOWS_NATIVE

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);
#else
#  define winsock2errno(fnord) (fnord)
#  define __winsock_init()
#endif

#if defined(__MINGW32__) || defined(__MINGW64__) || defined(_MSC_VER)
static size_t
getpagesize() {
  static DWORD cachedPageSize = 0;
  if(cachedPageSize == 0) {
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    cachedPageSize = si.dwPageSize;
  }
  return cachedPageSize;
}
#endif

# ifdef __cplusplus
}
# endif
# endif /* WINDOWS */

#endif /* defined(WINDOZE_H) */
