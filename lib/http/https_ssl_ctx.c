#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void* http_sslctx = 0;

void*
https_ssl_ctx(void) {
  const SSL_METHOD* method;
  SSL_CTX* ctx;
#if 1
  const OPENSSL_INIT_SETTINGS* settings = OPENSSL_INIT_new();
  OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, settings);
  method = TLS_client_method();
#else
  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
  method = SSLv23_client_method();
#endif
  ctx = SSL_CTX_new(method);
  if(ctx == NULL) {
    ERR_print_errors_fp(stderr);
    abort();
  }
  if(!SSL_CTX_use_RSAPrivateKey_file(ctx, "../c-utils/http.key", SSL_FILETYPE_PEM) ||
     !SSL_CTX_use_certificate_file(ctx, "../c-utils/http.crt", SSL_FILETYPE_PEM) ||
     !SSL_CTX_check_private_key(ctx)) {
    buffer_putm_internal(buffer_2, "certificate error: ", NULL);
    buffer_putnlflush(buffer_2);
  }
  return ctx;
}
#endif
