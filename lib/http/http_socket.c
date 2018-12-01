#include "../windoze.h"
//#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <stdio.h>

static SSL_CTX* http_sslctx;

size_t http_read_internal(http* h, char* buf, size_t n);

ssize_t http_socket_read(fd_t fd, void* buf, size_t len, buffer* b);

static ssize_t
http_socket_write(fd_t fd, void* buf, size_t len, buffer* b) {
  http* h = b->cookie;

  return winsock2errno(send(fd, buf, len, 0));
}

static SSL_CTX*
new_sslctx(void) {
  const SSL_METHOD* method;
  SSL_CTX* ctx;

#if OPENSSL_API_COMPAT >= 0x10100000L
  const OPENSSL_INIT_SETTINGS* settings = OPENSSL_INIT_new();
  OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, settings);
  method = TLS_client_method(); /* create new server-method instance */
#else
  SSL_library_init();
  OpenSSL_add_all_algorithms();    /* load & register all cryptos, etc. */
  SSL_load_error_strings();        /* load all error messages */
  method = SSLv23_client_method(); /* create new server-method instance */
#endif
  ctx = SSL_CTX_new(method); /* create new context from method */
  if(ctx == NULL) {
    ERR_print_errors_fp(stderr);
    abort();
  }
  return ctx;
}
int
http_socket(http* h) {

  h->sock = socket_tcp4();
  io_nonblock(h->sock);

#ifdef HAVE_OPENSSL
  if(!http_sslctx)
    http_sslctx = new_sslctx();

  h->ssl = SSL_new(http_sslctx);
  SSL_set_fd(h->ssl, h->sock);
#endif

  if(h->q.in.x) {
    h->q.in.fd = h->sock;
  } else {
    buffer_read_fd(&h->q.in, h->sock);
    h->q.in.cookie = (void*)h;
  }

  h->q.in.op = (buffer_op_proto*)&http_socket_read;

  if(h->q.out.x) {
    h->q.out.fd = h->sock;
  } else {
    buffer_write_fd(&h->q.out, h->sock);
    h->q.out.cookie = (void*)h;
  }
  h->q.out.op = (buffer_op_proto*)&http_socket_write;
}