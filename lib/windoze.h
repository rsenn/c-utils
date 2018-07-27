#if defined(_WIN32) || defined(_WIN64)

#include <io.h>

#ifndef _SSIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64 ssize_t;
#else
typedef _W64 unsigned int ssize_t;
#endif
#define _SSIZE_T_DEFINED
#endif

//typedef ssize_t socklen_t;

#ifdef __cplusplus
extern "C" {
#endif

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);

#ifdef __cplusplus
}
#endif

#else

#define winsock2errno(fnord) (fnord)
#define __winsock_init()

#endif
