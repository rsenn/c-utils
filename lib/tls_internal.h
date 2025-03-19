#ifndef TLS_INTERNAL_H
#define TLS_INTERNAL_H

#define USE_WS2_32 1

#include "windoze.h"
#include "socket_internal.h"

#if OPENSSL_SYS_WINDOWS
#undef OPENSSL_OPT_WINDLL
#define OPENSSL_OPT_WINDLL 1
#endif

#include "iarray.h"
#include "tls.h"

#include <errno.h>
#include <string.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/bio.h>

typedef void want_func(fd_type);

typedef enum { TLS_OP_WRITE = 0, TLS_OP_READ, TLS_OP_ACCEPT, TLS_OP_CONNECT, TLS_OP_SHUTDOWN } tls_op_t;

typedef struct tls_instance_st {
  tls_t* ssl;
  tls_op_t op;
  tls_bio_t* bio;
  int retval, syserr;
  tls_err_t error;
  want_func *wantread, *wantwrite;
  char errstr[128];
} tls_instance_t;

extern const char *tls_key, *tls_certificate;
extern iarray tls_list;
extern int tls_initialized;

extern tls_method_t const *tls_client_method, *tls_server_method;
extern tls_ctx_t *tls_client_ctx, *tls_server_ctx;

void tls_instance_debug(tls_instance_t* i);
int tls_instance_errno(tls_instance_t* i);
tls_instance_t* tls_instance_get(fd_type fd);
int tls_instance_handshake(tls_instance_t* i);
tls_instance_t* tls_instance_new(fd_type fd);
ssize_t tls_instance_return(tls_instance_t* i, tls_op_t op, int ret);
const char* tls_instance_strerror(tls_instance_t* i);
const char* tls_instance_strerror(tls_instance_t* i);

#endif
#endif
