#include "../buffer.h"
#include "../tls.h"
#include "../tls_internal.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

const char *tls_key = 0, *tls_certificate = 0;

int tls_initialized = 0;

SSL_METHOD const *tls_client_method = 0, *tls_server_method = 0;
SSL_CTX *tls_client_ctx = 0, *tls_server_ctx = 0;

int
tls_verify(int ok, X509_STORE_CTX* cert) {
  buffer_puts(buffer_2, "verify callback ok=");
  buffer_putlong(buffer_2, ok);
  buffer_puts(buffer_2, " cert=");
  buffer_putptr(buffer_2, cert);
  buffer_putnlflush(buffer_2);

  if(!ok)
    return 0; // stop immediately
  return 1;
}

tls_ctx_t*
tls_context(tls_method_t const* method) {
  tls_ctx_t* ctx;

  ctx = SSL_CTX_new(method);
  if(ctx == NULL) {
    ERR_print_errors_fp(stderr);
    abort();
    return 0;
  }
  if(tls_key) {
    if(!SSL_CTX_use_RSAPrivateKey_file(ctx, tls_key, SSL_FILETYPE_PEM)) {
      buffer_putm_internal(buffer_2, "ERROR loading key: ", ERR_lib_error_string(ERR_get_error()), NULL);
      buffer_putnlflush(buffer_2);
    }
  }

  if(tls_certificate) {
    if(!SSL_CTX_use_certificate_file(ctx, tls_certificate, SSL_FILETYPE_PEM)) {
      buffer_putm_internal(buffer_2, "ERROR loading certificate: ", ERR_lib_error_string(ERR_get_error()), NULL);
      buffer_putnlflush(buffer_2);
    }
  }
  if(tls_key && tls_certificate) {
    if(!SSL_CTX_check_private_key(ctx)) {
      buffer_putm_internal(buffer_2, "ERROR checking key: ", ERR_lib_error_string(ERR_get_error()), NULL);
      buffer_putnlflush(buffer_2);
    }
  }
  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, tls_verify);

  return ctx;
}

int
tls_init(const char* key_file, const char* cert_file) {

#if OPENSSL_API_COMPAT >= 0x10100000L
  const OPENSSL_INIT_SETTINGS* settings = OPENSSL_INIT_new();
  OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS,
                   settings);
#else
  SSL_library_init();
  ERR_load_crypto_strings();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();
#endif
  /* method = SSLv23_client_method();
   method = TLSv1_2_client_method();*/
  tls_client_method = TLS_client_method();
  tls_server_method = TLS_server_method();

  tls_key = key_file;
  tls_certificate = cert_file;

  tls_client_ctx = tls_context(tls_client_method);
  tls_server_ctx = tls_context(tls_server_method);

  tls_initialized = 1;

  return 1;
}
#endif
