#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(_WIN64)
# include <io.h>
# include <windows.h>
# ifdef __cplusplus
extern "C" {
# endif

typedef SIZE_T size_t;
typedef SSIZE_T ssize_t;

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);

# ifdef __cplusplus
}
# endif
# else
#  define winsock2errno(fnord) (fnord)
#  define __winsock_init()
# endif /* defined(_WIN32) || defined(_WIN64) */
#endif /* defined(WINDOZE_H) */