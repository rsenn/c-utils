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

typedef enum {
  TLS_ERR_NONE = 0,
  TLS_ERR_SSL,
  TLS_ERR_WANT_READ,
  TLS_ERR_WANT_WRITE,
  TLS_ERR_WANT_X509_LOOKUP,
  TLS_ERR_SYSCALL,
  TLS_ERR_ZERO_RETURN,
  TLS_ERR_WANT_CONNECT,
  TLS_ERR_WANT_ACCEPT,
} tls_err_t;

typedef struct ssl_st tls_t;
typedef struct x509_store_ctx_st tls_cert_t;
typedef struct ssl_method_st tls_method_t;
typedef struct bio_method_st bio_method_t;
typedef struct bio_st tls_bio_t;
typedef struct ssl_ctx_st tls_ctx_t;

int tls_accept(fd_type fd);

const bio_method_t* tls_bio_method(void);
tls_bio_t* tls_bio_socket(fd_type fd);
tls_t* tls_by_fd(fd_type fd);
tls_t* tls_client(fd_type fd);
int tls_close(fd_type fd);
int tls_connect(fd_type fd);
tls_ctx_t* tls_context(tls_method_t const* method);
tls_err_t tls_error(fd_type fd);
int tls_errno(fd_type fd);
int tls_established(fd_type fd);
void tls_free(fd_type fd);
fd_type tls_get_fd(tls_t* tls);
int tls_init(const char* key_file, const char* cert_file);
void tls_io(fd_type fd);
const bio_method_t* tls_method(void);
ssize_t tls_read(fd_type fd, void* data, size_t len);
tls_t* tls_server(fd_type fd);
const char* tls_strerror(fd_type fd);
int tls_verify(int ok, tls_cert_t* cert);
int tls_want(fd_type fd,
             void (*wantread)(fd_type),
             void (*wantwrite)(fd_type));
ssize_t tls_write(fd_type fd, const void* data, size_t len);
int tls_want(fd_type fd, void (*)(fd_type), void (*)(fd_type));

#ifdef __cplusplus
}
#endif

#endif /* defined TLS_H */
/** @} */
