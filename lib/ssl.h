#ifndef SSL_H
#define SSL_H 1

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssl_st ssl_t;
typedef struct x509_store_ctx_st ssl_cert_t;
typedef struct ssl_method_st ssl_method_t;
typedef struct ssl_ctx_st ssl_ctx_t;

int ssl_accept(fd_t fd);
ssl_t* ssl_by_fd(fd_t fd);
ssl_t* ssl_client(fd_t fd);
int ssl_close(fd_t fd);
int ssl_connect(fd_t fd);
void ssl_free(fd_t fd);
int ssl_init(const char* key_file, const char* cert_file);
void ssl_io(fd_t fd);
ssize_t ssl_read(fd_t fd, void* data, size_t len);
ssl_t* ssl_server(fd_t fd);
int ssl_verify(int ok, ssl_cert_t* cert);
ssize_t ssl_write(fd_t fd, const void* data, size_t len);
int ssl_established(fd_t fd);

#ifdef __cplusplus
}
#endif

#endif /* defined SSL_H */
