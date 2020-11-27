#include "../tls_internal.h"
#include "../tls.h"
#include "../iarray.h"
#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

iarray tls_list = {.pages = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   .elemsize = sizeof(tls_instance),
                   .elemperpage = 127,
                   .bytesperpage = 4096,
                   .len = 0};

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

static tls_t*
tls_new(tls_ctx_t* ctx, fd_t fd) {
  SSL* ssl;
  if(ctx == 0) {
    buffer_putsflush(buffer_2, "ERROR tls_new ctx=0\n");
    ssl = 0;
  } else if((ssl = SSL_new(ctx)) == 0) {
    buffer_putm_internal(
        buffer_2, "ERROR tls_new ssl=0 : ", ERR_lib_error_string(ERR_get_error()), NULL);
    buffer_putnlflush(buffer_2);
  } else {
    tls_instance* i = tls_instance_new(fd);
    SSL_set_fd(ssl, fd);
    i->ssl = ssl;
  }
  return ssl;
}

tls_t*
tls_client(fd_t fd) {
  if(tls_client_ctx == 0)
    tls_client_ctx = tls_context(tls_client_method);
  return tls_new(tls_client_ctx, fd);
}

tls_t*
tls_server(fd_t fd) {
  if(tls_server_ctx == 0)
    tls_server_ctx = tls_context(tls_server_method);
  return tls_new(tls_server_ctx, fd);
}
#endif
