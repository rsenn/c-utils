#include "../ssl_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
ssl_read(fd_t fd, void* data, size_t len) {
  ssize_t ret;
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);

  if(SSL_get_state(i->ssl) != TLS_ST_OK) {
    int server = SSL_get_ssl_method(i->ssl) == ssl_server_method;

    int (*sslfn)(ssl_t*);
#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "ssl ", server ? "server" : "client", " ", 0);
    buffer_putlong(buffer_2, SSL_get_fd(i->ssl));
    buffer_putsflush(buffer_2, " continuing handshake...");
#endif
    sslfn = server ? SSL_accept : SSL_connect;

    ret = ssl_instance_return(i, sslfn(i->ssl));
#ifdef DEBUG_OUTPUT
   buffer_putm_internal(buffer_2, "ssl ", server ? "server" : "client", " ", 0);
    buffer_putlong(buffer_2, SSL_get_fd(i->ssl));
  buffer_puts(buffer_2, " resumed handshake: ");
     buffer_putlong(buffer_2,ret);
     if(ret == -1) 
      buffer_putm_internal(buffer_2, " (", ssl_instance_error(i), ")", 0);
  
   buffer_putnlflush(buffer_2);
#endif
  }

  ret = ssl_instance_return(i, SSL_read(i->ssl, data, len));

  return ret;
}
#endif
