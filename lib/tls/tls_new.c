#include "../tls_internal.h"
#include "../iarray.h"
#include "../buffer.h"
#include "../byte.h"
#include "../str.h"
#include <assert.h>
#if WINDOWS_NATIVE
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

iarray tls_list = {
    /* .pages =  */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* .elemsize =  */ sizeof(tls_instance_t),
    /* .elemperpage =  */ 127,
    /* .bytesperpage =  */ 4096,
    /* .len =  */ 0,
};

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

static tls_t*
tls_new(tls_ctx_t* ctx, fd_t fd) {
  SSL* ssl;
  tls_instance_t* i = 0;

  if(ctx == 0) {
    buffer_putsflush(buffer_2, "ERROR tls_new ctx=0\n");
    ssl = 0;
  } else if((ssl = SSL_new(ctx)) == 0) {
    buffer_putm_internal(buffer_2, "ERROR tls_new ssl=0 : ", ERR_lib_error_string(ERR_get_error()), NULL);
    buffer_putnlflush(buffer_2);
  } else {
    i = tls_instance_new(fd);
    i->ssl = ssl;

    i->bio = tls_bio_socket(fd);

    SSL_set_bio(ssl, i->bio, i->bio);
  }

  return ssl;
}

tls_t*
tls_client(fd_t fd) {
  if(!tls_initialized)
    tls_init(0, 0);
  if(tls_client_ctx == 0)
    tls_client_ctx = tls_context(tls_client_method);
  return tls_new(tls_client_ctx, fd);
}

tls_t*
tls_server(fd_t fd) {
  if(!tls_initialized)
    tls_init(0, 0);
  if(tls_server_ctx == 0)
    tls_server_ctx = tls_context(tls_server_method);
  return tls_new(tls_server_ctx, fd);
}
#endif
