#if defined(_WIN32) || defined(_WIN64)

#include <io.h>
#include <windows.h>

typedef SSIZE_T ssize_t;

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
