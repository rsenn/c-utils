#ifndef WINDOZE_H
#define WINDOZE_H 1

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

#if !defined(__ssize_t_defined) && !defined(_SSIZE_T_DECLARED)
#define __ssize_t_defined 1
#define _SSIZE_T_DECLARED 1
typedef SSIZE_T ssize_t;
#endif

typedef int socklen_t;

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);

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
# else
#  define winsock2errno(fnord) (fnord)
#  define __winsock_init()
# endif /* WINDOWS */

#if defined(_WIN32) || defined(_WIN64)
# if !(defined(__MSYS__) || defined(__CYGWIN__))
#  define WINDOWS_NATIVE 1
# endif
# define WINDOWS 1
#endif

#endif /* defined(WINDOZE_H) */
