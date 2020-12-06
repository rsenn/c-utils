#ifndef TLS_INTERNAL_H
#define TLS_INTERNAL_H

#include "iarray.h"
#include "tls.h"

#include <errno.h>
#include <string.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/bio.h>

typedef void want_func(fd_t);

typedef enum {
  TLS_OP_WRITE = 0,
  TLS_OP_READ,
  TLS_OP_ACCEPT,
  TLS_OP_CONNECT,
  TLS_OP_SHUTDOWN
} tls_op_t;

typedef struct bio_method_st {
  int type;
  char* name;
  int (*bwrite)(struct bio_st*, const char*, size_t, size_t*);
  int (*bwrite_old)(struct bio_st*, const char*, int);
  int (*bread)(struct bio_st*, char*, size_t, size_t*);
  int (*bread_old)(struct bio_st*, char*, int);
  int (*bputs)(struct bio_st*, const char*);
  int (*bgets)(struct bio_st*, char*, int);
  long (*ctrl)(struct bio_st*, int, long, void*);
  int (*create)(struct bio_st*);
  int (*destroy)(struct bio_st*);
  long (*callback_ctrl)(struct bio_st*, int, BIO_info_cb*);
} bio_method_t;

typedef struct bio_st {
  const bio_method_t* method;
  /* bio, mode, argp, argi, argl, ret */
  void* callback;
  void* callback_ex;
  char* cb_arg; /* first argument for the callback */
  int init;
  int shutdown;
  int flags; /* extra storage */
  int retry_reason;
  int num;
  void* ptr;
  struct bio_st* next_bio; /* used by filter BIOs */
  struct bio_st* prev_bio; /* used by filter BIOs */
} tls_bio_t;

typedef struct tls_instance_st {
  tls_t* ssl;
  tls_op_t op;
  tls_bio_t* bio;
  int retval;
  int error;
  want_func *wantread, *wantwrite;
  char errstr[128];
} tls_instance;

extern const char *tls_key, *tls_certificate;
extern iarray tls_list;
extern int tls_initialized;

extern tls_method_t const *tls_client_method, *tls_server_method;
extern tls_ctx_t *tls_client_ctx, *tls_server_ctx;

tls_ctx_t* tls_context(tls_method_t const* method);
tls_instance* tls_instance_new(fd_t fd);
ssize_t tls_instance_return(tls_instance* i, tls_op_t, int ret);
tls_ctx_t* tls_context(tls_method_t const* method);
const char* tls_instance_strerror(tls_instance* i);
int tls_instance_errno(tls_instance* i);
void tls_instance_debug(tls_instance* i);
int tls_instance_handshake(tls_instance* i);
const bio_method_t* tls_bio_method(void);
tls_bio_t* tls_bio_socket(fd_t fd);

#endif
#endif
