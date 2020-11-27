#ifndef TLS_H
#define TLS_H 1

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssl_st tls_t;
typedef struct x509_store_ctx_st tls_cert_t;
typedef struct ssl_method_st tls_method_t;
typedef struct ssl_ctx_st tls_ctx_t;

int tls_accept(fd_t fd);
tls_t* tls_by_fd(fd_t fd);
tls_t* tls_client(fd_t fd);
int tls_close(fd_t fd);
int tls_connect(fd_t fd);
void tls_free(fd_t fd);
int tls_init(const char* key_file, const char* cert_file);
void tls_io(fd_t fd);
ssize_t tls_read(fd_t fd, void* data, size_t len);
tls_t* tls_server(fd_t fd);
int tls_verify(int ok, tls_cert_t* cert);
ssize_t tls_write(fd_t fd, const void* data, size_t len);
int tls_established(fd_t fd);

#ifdef __cplusplus
}
#endif

#endif /* defined TLS_H */
