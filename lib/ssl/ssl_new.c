#include "../ssl_internal.h"
#include "../ssl.h"
#include "../iarray.h"
#include "../buffer.h"
#include "../byte.h"
#include <assert.h>

iarray ssl_list = {.pages = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   .elemsize = sizeof(ssl_instance),
                   .elemperpage = 127,
                   .bytesperpage = 4096,
                   .len = 0};

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

static ssl_t*
ssl_new(SSL_CTX* ctx, fd_t fd) {
  SSL* ssl;
  if(ctx == 0) {
    buffer_putsflush(buffer_2, "ERROR ssl_new ctx=0\n");
    ssl = 0;
  } else if((ssl = SSL_new(ctx)) == 0) {
    buffer_putm_internal(
        buffer_2, "ERROR ssl_new ssl=0 : ", ERR_lib_error_string(ERR_get_error()), NULL);
    buffer_putnlflush(buffer_2);
  } else {
    ssl_instance* inst = ssl_instance_new(fd);
    SSL_set_fd(ssl, fd);
    inst->ssl = ssl;
  }
  return ssl;
}

ssl_t*
ssl_client(fd_t fd) {
  if(ssl_client_ctx == 0)
    ssl_client_ctx = ssl_context(ssl_client_method);
  return ssl_new(ssl_client_ctx, fd);
}

ssl_t*
ssl_server(fd_t fd) {
  if(ssl_server_ctx == 0)
    ssl_server_ctx = ssl_context(ssl_server_method);
  return ssl_new(ssl_server_ctx, fd);
}
#endif
