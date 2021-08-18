#include "../tls_internal.h"
#include "../str.h"
#include <errno.h>

#if WINDOWS_NATIVE
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/bio.h>

#define clear_socket_error() errno = 0

static long tls_bio_ctrl(tls_bio_t*, int, long, void*);
static int tls_bio_send(tls_bio_t*, const char*, size_t, size_t*);
static int tls_bio_recv(tls_bio_t*, char*, size_t, size_t*);
static int tls_bio_puts(tls_bio_t*, const char*);
static int tls_bio_create(tls_bio_t*);
static int tls_bio_destroy(tls_bio_t*);

static const bio_method_t tls_method_sockp = {
    .type = BIO_TYPE_SOCKET,
    .name = "socket",
    .bwrite = tls_bio_send,
    .bread = tls_bio_recv,
    .bputs = tls_bio_puts,
    .bgets = NULL,
    .ctrl = tls_bio_ctrl,
    .create = tls_bio_create,
    .destroy = tls_bio_destroy,
    .callback_ctrl = NULL,
};

const BIO_METHOD*
tls_method(void) {
  return (&tls_method_sockp);
}

static int
tls_bio_create(tls_bio_t* bi) {
  bi->init = 0;
  bi->num = 0;
  bi->ptr = NULL;
  bi->flags = 0;
  return (1);
}

static int
tls_bio_destroy(tls_bio_t* a) {
  if(a == NULL)
    return (0);
  if(a->shutdown) {
    if(a->init) {
      BIO_closesocket(a->num);
    }
    a->init = 0;
    a->flags = 0;
  }
  return (1);
}

static int
tls_bio_recv(tls_bio_t* b, char* out, size_t outl, size_t* lptr) {
  int ret = 0;

  if(out != NULL) {
    clear_socket_error();
    ret = recv(b->num, out, outl, 0);
    BIO_clear_retry_flags(b);
    if(ret <= 0) {
      if(BIO_fd_should_retry(b->num))
        BIO_set_retry_read(b);
    }
  }
  return (ret);
}

static int
tls_bio_send(tls_bio_t* b, const char* in, size_t inl, size_t* lptr) {
  int ret;

  clear_socket_error();
  ret = send(b->num, in, inl, 0);
  BIO_clear_retry_flags(b);
  if(ret <= 0) {
    if(BIO_fd_should_retry(b->num))
      BIO_set_retry_write(b);
  }
  return (ret);
}

long
tls_bio_ctrl(tls_bio_t* b, int cmd, long num, void* ptr) {
  long ret = 1;
  int* ip;

  switch(cmd) {
    case BIO_C_SET_FD:
      tls_bio_destroy(b);
      b->num = *((fd_t*)ptr);
      b->shutdown = (fd_t)num;
      b->init = 1;
      break;
    case BIO_C_GET_FD:
      if(b->init) {
        ip = (fd_t*)ptr;
        if(ip != NULL)
          *ip = b->num;
        ret = b->num;
      } else
        ret = -1;
      break;
    case BIO_CTRL_GET_CLOSE: ret = b->shutdown; break;
    case BIO_CTRL_SET_CLOSE: b->shutdown = (fd_t)num; break;
    case BIO_CTRL_DUP:
    case BIO_CTRL_FLUSH: ret = 1; break;
    default: ret = 0; break;
  }
  return (ret);
}

static int
tls_bio_puts(tls_bio_t* bp, const char* str) {
  int n, ret;

  n = str_len(str);
  ret = tls_bio_send(bp, str, n, 0);
  return (ret);
}
#endif
