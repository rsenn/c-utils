#include "../buffer.h"
#include "../ssl.h"
#include "../ssl_internal.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

const char *ssl_key = 0, *ssl_certificate = 0;

SSL_METHOD const *ssl_client_method = 0, *ssl_server_method = 0;
SSL_CTX *ssl_client_ctx = 0, *ssl_server_ctx = 0;

int
ssl_verify(int ok, X509_STORE_CTX* cert) {
  buffer_puts(buffer_2, "ssl_verify callback ok=");
  buffer_putlong(buffer_2, ok);
  buffer_puts(buffer_2, " cert=");
  buffer_putptr(buffer_2, cert);
  buffer_putnlflush(buffer_2);

  if(!ok)
    return 0; // stop immediately
  return 1;
}

ssl_ctx_t*
ssl_context(ssl_method_t const* method) {
  ssl_ctx_t* ctx;

  ctx = SSL_CTX_new(method);
  if(ctx == NULL) {
    ERR_print_errors_fp(stderr);
    abort();
    return 0;
  }
  if(ssl_key) {
    if(!SSL_CTX_use_RSAPrivateKey_file(ctx, ssl_key, SSL_FILETYPE_PEM)) {
      buffer_putm_internal(
          buffer_2, "ERROR loading key: ", ERR_lib_error_string(ERR_get_error()), NULL);
      buffer_putnlflush(buffer_2);
    }
  }

  if(ssl_certificate) {
    if(!SSL_CTX_use_certificate_file(ctx, ssl_certificate, SSL_FILETYPE_PEM)) {
      buffer_putm_internal(
          buffer_2, "ERROR loading certificate: ", ERR_lib_error_string(ERR_get_error()), NULL);
      buffer_putnlflush(buffer_2);
    }
  }
  if(ssl_key && ssl_certificate) {
    if(!SSL_CTX_check_private_key(ctx)) {
      buffer_putm_internal(
          buffer_2, "ERROR checking key: ", ERR_lib_error_string(ERR_get_error()), NULL);
      buffer_putnlflush(buffer_2);
    }
  }
  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, ssl_verify);

  return ctx;
}

int
ssl_init(const char* key_file, const char* cert_file) {
#if OPENSSL_API_COMPAT >= 0x10100000L
  const OPENSSL_INIT_SETTINGS* settings = OPENSSL_INIT_new();
  OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, settings);
#else
  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
#endif
  /* method = SSLv23_client_method();
  method = TLSv1_2_client_method();*/
  ssl_client_method = TLS_client_method();
  ssl_server_method = TLS_server_method();

  ssl_key = key_file;
  ssl_certificate = cert_file;

  ssl_client_ctx = ssl_context(ssl_client_method);
  ssl_server_ctx = ssl_context(ssl_server_method);

  return 1;
}
#endif
