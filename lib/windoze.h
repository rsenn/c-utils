#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
# if !(defined(__MSYS__) || defined(__CYGWIN__))
#  define WINDOWS_NATIVE 1
# endif
# define WINDOWS 1
#endif

#if WINDOWS || defined(__MSYS__)
# include <io.h>
# include <windows.h>
#endif

# ifdef __cplusplus
extern "C" {
# endif

#if WINDOWS || defined(__MSYS__)

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED 1
typedef SIZE_T size_t;
#endif

#if WINDOWS && !defined(__ssize_t_defined) && !defined(_SSIZE_T_DECLARED) && !defined(_SSIZE_T_DEFINED) && !defined(__DEFINED_ssize_t) && !defined(__dietlibc__)
#define __ssize_t_defined 1
#define _SSIZE_T_DECLARED 1
#define _SSIZE_T_DEFINED 1
#ifdef _WIN64
typedef __int64 ssize_t;
#elif defined(_WIN32)
typedef __int32 ssize_t;
#endif
#endif

typedef int socklen_t;

#if defined(__MINGW32__) || defined(__MINGW64__) || defined(_MSC_VER)
inline static size_t
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

# endif /* WINDOWS */

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
#  define closesocket(s) close(s)
#endif

# ifdef __cplusplus
}
# endif


#endif /* defined(WINDOZE_H) */
