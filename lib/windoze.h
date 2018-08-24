#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(_WIN64)
# include <io.h>
# include <windows.h>
# ifdef __cplusplus
extern "C" {
# endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED 1
typedef SIZE_T size_t;
#endif

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED 1
typedef SSIZE_T ssize_t;
#endif

typedef int socklen_t;

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);

#if defined(__MINGW32__) || defined(__MINGW64__) || defined(_MSC_VER)
static inline size_t
getpagesize() {
  SYSTEM_INFO si;
  GetNativeSystemInfo(&si);
  return si.dwPageSize;
}
#endif

# ifdef __cplusplus
}
# endif
# else
#  define winsock2errno(fnord) (fnord)
#  define __winsock_init()
# endif /* defined(_WIN32) || defined(_WIN64) */

#endif /* defined(WINDOZE_H) */
