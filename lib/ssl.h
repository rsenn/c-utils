#ifndef SSL_H
#define SSL_H 1

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssl_st ssl_t;

ssl_t* ssl_get_fd(fd_t);
int ssl_init(const char* key_file, const char* cert_file);
ssl_t* ssl_client(fd_t);
ssl_t* ssl_server(fd_t);
int ssl_accept(fd_t);
int ssl_connect(fd_t);
ssize_t ssl_read(fd_t, void* data, size_t len);
ssize_t ssl_write(fd_t, const void* data, size_t len);
void          ssl_io(fd_t fd);

#ifdef __cplusplus
}
#endif

#endif /* defined SSL_H */
