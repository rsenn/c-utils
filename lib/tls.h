/**
 * @defgroup   tls
 * @brief      TLS module.
 * @{
 */
#ifndef TLS_H
#define TLS_H 1

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ssl_st tls_t;
typedef struct x509_store_ctx_st tls_cert_t;
typedef struct ssl_method_st tls_method_t;
typedef struct bio_method_st bio_method_t;
typedef struct bio_st tls_bio_t;
typedef struct ssl_ctx_st tls_ctx_t;

int tls_accept(fd_t fd);

const bio_method_t* tls_bio_method(void);
tls_bio_t* tls_bio_socket(fd_t fd);
tls_t* tls_by_fd(fd_t fd);
tls_t* tls_client(fd_t fd);
int tls_close(fd_t fd);
int tls_connect(fd_t fd);
tls_ctx_t* tls_context(tls_method_t const* method);
int tls_error(fd_t fd);
int tls_established(fd_t fd);
void tls_free(fd_t fd);
fd_t tls_get_fd(tls_t* tls);
int tls_init(const char* key_file, const char* cert_file);
void tls_io(fd_t fd);
const bio_method_t* tls_method(void);
ssize_t tls_read(fd_t fd, void* data, size_t len);
tls_t* tls_server(fd_t fd);
const char* tls_strerror(fd_t fd);
int tls_verify(int ok, tls_cert_t* cert);
int tls_want(fd_t fd, void (*wantread)(fd_t), void (*wantwrite)(fd_t));
ssize_t tls_write(fd_t fd, const void* data, size_t len);

#define tls_instance_t(fd) tls_instance_get((fd))
#define is_tls(fd) !!tls_instance_get((fd))

//#define tls_error(fd) (tls_instance_get((fd))->error)
#define tls_errno(fd) (tls_instance_errno(tls_instance_get((fd))))
int tls_want(fd_t fd, void (*)(fd_t), void (*)(fd_t));

#ifdef __cplusplus
}
#endif

#endif /* defined TLS_H */
/** @} */
