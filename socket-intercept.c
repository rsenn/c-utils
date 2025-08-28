#define _GNU_SOURCE 1

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
//#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/poll.h>

#include "lib/alloc.h"
#include "lib/stralloc.h"
#include "lib/open.h"
#include "lib/bool.h"
#include "lib/cas.h"
#include "lib/list.h"
#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/textcode.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/uint64.h"
#include "lib/str.h"
#include "lib/thread.h"
#include "lib/util.h"

typedef struct {
  struct list_head link;
  int fd, ret_val, error;
  uint64 read, written;
  bool closed;
} Sock;

static long intercept_init(void);
static Sock* intercept_find(struct list_head*, int);
static Sock* intercept_findall(int);
static Sock* intercept_ssl(void*);
static void intercept_close(Sock*);
static Sock* intercept_new(int);
static void intercept_delete(Sock*);
static void intercept_seterror(Sock*, int);
static void intercept_cleanup(void);

static thread_local long initialized;
static thread_local struct list_head intercept_fds, ssl_fds, closed_fds;
static thread_local buffer o;
static thread_local char procname[64];

typedef int intercept_function(int, int, int);
typedef int listen_function(int, int);
typedef int bind_function(int, const struct sockaddr*, socklen_t);
typedef int connect_function(int, const struct sockaddr*, socklen_t);
typedef int accept_function(int, struct sockaddr*, socklen_t*);
typedef int accept4_function(int, struct sockaddr*, socklen_t*, int);
typedef ssize_t send_function(int, const void*, size_t, int);
typedef ssize_t sendto_function(int, const void*, size_t, int, const struct sockaddr*, socklen_t);
typedef ssize_t sendmsg_function(int, const struct msghdr*, int);
typedef ssize_t write_function(int, const void*, size_t);
typedef ssize_t writev_function(int, const struct iovec*, int);
typedef ssize_t recv_function(int, void*, size_t, int);
typedef ssize_t recvfrom_function(int, void*, size_t, int, struct sockaddr*, socklen_t*);
typedef ssize_t recvmsg_function(int, struct msghdr*, int);
typedef ssize_t read_function(int, void*, size_t);
typedef ssize_t readv_function(int, const struct iovec*, int);
typedef int getsockopt_function(int, int, int, void*, socklen_t*);
typedef int setsockopt_function(int, int, int, const void*, socklen_t);
typedef int getsockname_function(int, struct sockaddr*, socklen_t*);
typedef int getpeername_function(int, struct sockaddr*, socklen_t*);
typedef int close_function(int);
typedef int shutdown_function(int, int);
typedef int recvmmsg_function(int sockfd, struct mmsghdr* msgvec, unsigned vlen, int flags, struct timespec* timeout);
typedef int sendmmsg_function(int sockfd, struct mmsghdr* msgvec, unsigned vlen, int flags);
typedef int fcntl_function(int, int, ...);
typedef int poll_function(struct pollfd*, nfds_t, int);
typedef int SSL_connect_function(void*);
typedef int SSL_accept_function(void*);
typedef int SSL_read_function(void* ssl, void* buf, int num);
typedef int SSL_write_function(void* ssl, const void* buf, int num);
typedef int SSL_get_rfd_function(const void* ssl);

typedef int SSL_get_wfd_function(const void* ssl);
typedef int SSL_get_error_function(const void* ssl, int ret);
typedef int SSL_shutdown_function(void*);

static thread_local intercept_function* libc_socket;
static thread_local listen_function* libc_listen;
static thread_local bind_function* libc_bind;
static thread_local connect_function* libc_connect;
static thread_local accept_function* libc_accept;
static thread_local accept4_function* libc_accept4;
static thread_local send_function* libc_send;
static thread_local sendto_function* libc_sendto;
static thread_local sendmsg_function* libc_sendmsg;
static thread_local sendmmsg_function* libc_sendmmsg;
static thread_local write_function* libc_write;
static thread_local writev_function* libc_writev;
static thread_local recv_function* libc_recv;
static thread_local recvfrom_function* libc_recvfrom;
static thread_local recvmsg_function* libc_recvmsg;
static thread_local recvmmsg_function* libc_recvmmsg;
static thread_local read_function* libc_read;
static thread_local readv_function* libc_readv;
static thread_local getsockopt_function* libc_getsockopt;
static thread_local setsockopt_function* libc_setsockopt;
static thread_local getsockname_function* libc_getsockname;
static thread_local getpeername_function* libc_getpeername;
static thread_local close_function* libc_close;
static thread_local shutdown_function* libc_shutdown;
static thread_local fcntl_function* libc_fcntl;
static thread_local poll_function* libc_poll;
static thread_local SSL_connect_function* openssl_ssl_connect;
static thread_local SSL_accept_function* openssl_ssl_accept;
static thread_local SSL_read_function* openssl_ssl_read;
static thread_local SSL_write_function* openssl_ssl_write;
static thread_local SSL_get_rfd_function* openssl_ssl_get_rfd;
static thread_local SSL_get_wfd_function* openssl_ssl_get_wfd;
static thread_local SSL_get_error_function* openssl_ssl_get_error;
static thread_local SSL_shutdown_function* openssl_ssl_shutdown;

static const char* const address_families[AF_MAX] = {
    "UNSPEC", "LOCAL",  "INET",   "AX25",   "IPX",     "APPLETALK", "NETROM", "BRIDGE",  "ATMPVC",
    "X25",    "INET6",  "ROSE",   "DECnet", "NETBEUI", "SECURITY",  "KEY",    "NETLINK", "PACKET",
    "ASH",    "ECONET", "ATMSVC", NULL,     "SNA",     "IRDA",      "PPPOX",  "WANPIPE",
};

static size_t
fmt_sockaddr(char x[64], const struct sockaddr* addr, socklen_t addrlen) {
  size_t n = 0;
  uint16 port = 0;

  if(addr == NULL)
    return str_copy(x, "NULL");

  if(addrlen == 0)
    return str_copy(x, "[]");

  const int af = addr->sa_family;

  switch(af) {
    case AF_INET: {
      struct sockaddr_in* const sa_in = (struct sockaddr_in*)addr;

      n += fmt_ip4(&x[n], (char*)&sa_in->sin_addr);
      port = ntohs(sa_in->sin_port);
      break;
    }

    case AF_INET6: {
      struct sockaddr_in6* const sa_in6 = (struct sockaddr_in6*)addr;

      if((port = ntohs(sa_in6->sin6_port)))
        x[n++] = '[';

      n += fmt_ip6(&x[n], (char*)&sa_in6->sin6_addr);

      if(port)
        x[n++] = ']';

      break;
    }

    case AF_UNIX: {
      const struct sockaddr_un* const sa_un = (struct sockaddr_un*)addr;

      n += str_copy(&x[n], "unix:");
      n += str_copy(&x[n], sa_un->sun_path);

      break;
    }

    default: {
      const char* str;

      if((str = (af >= 0 && af < countof(address_families)) ? address_families[af] : 0)) {
        n += str_copy(&x[n], "unhandled address family: AF_");
        n += str_copy(&x[n], str);
      } else {
        n += str_copy(&x[n], "unknown address family: ");
        n += fmt_ulong(&x[n], af);
      }

      break;
    }
  }

  if(port) {
    x[n++] = ':';
    n += fmt_ulong(&x[n], port);
  }

  return n;
}

static size_t
fmt_buf(char* x, const void* buf, size_t len) {
  const char* y = (const char*)buf;
  size_t j = 0;

  x[j++] = '"';
  j += fmt_cescape(&x[j], buf, len);
  x[j++] = '"';

  return j;
}

static size_t
fmt_lenptr(char* x, socklen_t* lenptr) {
  size_t j = 0;

  if(lenptr == 0)
    j += str_copy(&x[j], "NULL");
  else {
    x[j++] = '[';
    j += fmt_ulong(&x[j], *lenptr);
    x[j++] = ']';
  }

  return j;
}

static size_t
fmt_events(char* x, int ev) {
  size_t j = 0;

  if(ev & POLLIN)
    j += str_copy(&x[j], "IN");

  if(ev & POLLOUT) {
    if(j > 0)
      x[j++] = '|';
    j += str_copy(&x[j], "OUT");
  }

  if(ev & POLLERR) {
    if(j > 0)
      x[j++] = '|';
    j += str_copy(&x[j], "ERR");
  }

  if(ev & POLLHUP) {
    if(j > 0)
      x[j++] = '|';
    j += str_copy(&x[j], "HUP");
  }

  return j;
}

static void
put_process(void) {
  pid_t pid = getpid(), tid = gettid();

  buffer_puts(&o, "[");
  buffer_putulong(&o, pid);

  if(tid != pid) {
    buffer_putc(&o, ',');
    buffer_putulong(&o, tid);
  }

  if(procname[0]) {
    buffer_putc(&o, '/');
    buffer_puts(&o, procname);
  }

  buffer_puts(&o, "] ");
}

static inline int
put_sockaddr(const struct sockaddr* addr, socklen_t addrlen) {
  char buf[64];
  return buffer_put(&o, buf, fmt_sockaddr(buf, addr, addrlen));
}

static inline int
put_lenptr(socklen_t* addrlen) {
  char buf[64];
  return buffer_put(&o, buf, fmt_lenptr(buf, addrlen));
}

static void
put_buf(const void* buf, size_t len) {
  char tmp[len * 4 + 2 + 1];
  buffer_put(&o, tmp, fmt_buf(tmp, buf, len));
}

static void
put_msg(const struct msghdr* msg) {
  size_t len;

  if(msg->msg_name) {
    buffer_puts(&o, "Name: ");
    put_buf(msg->msg_name, msg->msg_namelen);
    buffer_puts(&o, ", ");
  }

  if((len = msg->msg_iovlen) > 0) {
    buffer_puts(&o, "[ ");

    for(size_t i = 0; i < len; i++) {
      if(i > 0)
        buffer_puts(&o, ", ");

      put_buf(msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len);
    }

    buffer_puts(&o, " ]");
  }
}

static void
put_mmsgs(const struct mmsghdr* msgvec, unsigned vlen) {
  buffer_puts(&o, "[ ");

  for(unsigned i = 0; i < vlen; i++) {
    if(i > 0)
      buffer_puts(&o, ", ");

    put_msg(&msgvec[i].msg_hdr);
  }

  buffer_puts(&o, " ]");
}

static void
put_events(int ev) {
  char buf[64];
  buffer_put(&o, buf, fmt_events(buf, ev));
}

VISIBLE int
socket(int af, int type, int protocol) {
  int r = libc_socket(af, type, protocol);

  if(r >= 0)
    intercept_new(r);

  put_process();
  buffer_puts(&o, "socket(");
  buffer_putlong(&o, af);
  buffer_puts(&o, ", ");
  buffer_putlong(&o, type);
  buffer_puts(&o, ", ");
  buffer_putlong(&o, protocol);
  buffer_puts(&o, ") = ");
  buffer_putlong(&o, r);
  buffer_putnlflush(&o);

  return r;
}

VISIBLE int
listen(int sockfd, int backlog) {
  Sock* s;
  int r = libc_listen(sockfd, backlog);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "listen(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, backlog);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
  Sock* s;
  int r = libc_bind(sockfd, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "bind(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_sockaddr(addr, addrlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
  Sock* s;
  int r = libc_connect(sockfd, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "connect(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_sockaddr(addr, addrlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);

    if(r < 0) {
      buffer_puts(&o, " [");
      buffer_puts(&o, strerror(errno));
      buffer_puts(&o, "]");
    }

    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_accept(sockfd, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "accept(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    put_lenptr(addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  if(r >= 0)
    intercept_new(r);

  return r;
}

VISIBLE int
accept4(int sockfd, struct sockaddr* addr, socklen_t* addrlen, int flags) {
  Sock* s;
  int r = libc_accept4(sockfd, addr, addrlen, flags);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "accept4(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    put_lenptr(addrlen);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  if(r >= 0)
    intercept_new(r);

  return r;
}

VISIBLE ssize_t
send(int sockfd, const void* buf, size_t len, int flags) {
  Sock* s;
  int r = libc_send(sockfd, buf, len, flags);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "send(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_buf(buf, len);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      s->written += r;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
sendto(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* addr, socklen_t addrlen) {
  Sock* s;
  int r = libc_sendto(sockfd, buf, len, flags, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "sendto(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_buf(buf, len);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ", ");
    put_sockaddr(addr, addrlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      s->written += r;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
sendmsg(int sockfd, const struct msghdr* msg, int flags) {
  Sock* s;
  int r = libc_sendmsg(sockfd, msg, flags);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "sendmsg(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_msg(msg);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      s->written += r;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE int
sendmmsg(int sockfd, struct mmsghdr* msgvec, unsigned vlen, int flags) {
  Sock* s;
  int r = libc_sendmmsg(sockfd, msgvec, vlen, flags);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "sendmmsg(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_mmsgs(msgvec, vlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, vlen);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      for(size_t i = 0; i < r; i++)
        s->written += msgvec[i].msg_len;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
write(int fd, const void* buf, size_t len) {
  Sock* s;
  int r = libc_write(fd, buf, len);

  if((s = intercept_find(&intercept_fds, fd))) {
    put_process();
    buffer_puts(&o, "write(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    put_buf(buf, len);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      s->written += r;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
writev(int fd, const struct iovec* iov, int iovcnt) {
  Sock* s;
  int r = libc_writev(fd, iov, iovcnt);

  if((s = intercept_find(&intercept_fds, fd))) {
    put_process();
    buffer_puts(&o, "writev(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    buffer_putptr(&o, iov);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, iovcnt);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      s->written += r;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
recv(int sockfd, void* buf, size_t len, int flags) {
  Sock* s;
  int r = libc_recv(sockfd, buf, len, flags);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "recv(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r > 0)
      put_buf(buf, r);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_recvfrom(sockfd, buf, len, flags, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "recvfrom(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r > 0)
      put_buf(buf, r);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");

    if(r >= 0)
      put_lenptr(addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
recvmsg(int sockfd, struct msghdr* msg, int flags) {
  Sock* s;
  int r = libc_recvmsg(sockfd, msg, flags);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "recvmsg(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_msg(msg);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE int
recvmmsg(int sockfd, struct mmsghdr* msgvec, unsigned vlen, int flags, struct timespec* t) {
  Sock* s;
  int r = libc_recvmmsg(sockfd, msgvec, vlen, flags, t);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "recvmmsg(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_mmsgs(msgvec, vlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, vlen);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ", ");
    buffer_putptr(&o, t);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r > 0)
      for(size_t i = 0; i < r; i++)
        s->read += msgvec[i].msg_len;
    else if(r == 0)
      s->closed = 1;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
read(int fd, void* buf, size_t len) {
  Sock* s;
  int r = libc_read(fd, buf, len);

  if((s = intercept_find(&intercept_fds, fd))) {
    put_process();
    buffer_puts(&o, "read(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");

    if(r > 0)
      put_buf(buf, r);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);

    if(r < 0) {
      buffer_puts(&o, " [");
      buffer_puts(&o, strerror(errno));
      buffer_puts(&o, "]");
    }

    buffer_putnlflush(&o);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE ssize_t
readv(int fd, const struct iovec* iov, int iovcnt) {
  Sock* s;
  int r = libc_readv(fd, iov, iovcnt);

  if((s = intercept_find(&intercept_fds, fd))) {
    put_process();
    buffer_puts(&o, "readv(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    buffer_putptr(&o, iov);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, iovcnt);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      intercept_seterror(s, r);
  }

  return r;
}

VISIBLE int
getsockopt(int sockfd, int level, int optname, void* optval, socklen_t* optlen) {
  Sock* s;
  int r = libc_getsockopt(sockfd, level, optname, optval, optlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "getsockopt(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, level);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, optname);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_buf(optval, *optlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");

    if(r >= 0)
      buffer_putulong(&o, *optlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
  Sock* s;
  int r = libc_setsockopt(sockfd, level, optname, optval, optlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "setsockopt(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, level);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, optname);
    buffer_puts(&o, ", ");
    put_buf(optval, optlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, optlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
getsockname(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_getsockname(sockfd, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "getsockname(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    put_lenptr(addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
getpeername(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_getpeername(sockfd, addr, addrlen);

  if((s = intercept_find(&intercept_fds, sockfd))) {
    put_process();
    buffer_puts(&o, "getpeername(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    put_lenptr(addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
close(int fd) {
  Sock* s;
  int r = libc_close(fd);

  if((s = intercept_find(&intercept_fds, fd))) {
    intercept_close(s);

    put_process();
    buffer_puts(&o, "close(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
shutdown(int fd, int how) {
  Sock* s;
  int r = libc_shutdown(fd, how);

  if((s = intercept_find(&intercept_fds, fd))) {
    put_process();
    buffer_puts(&o, "shutdown(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    buffer_puts(&o, ((const char* []){"SHUT_RD", "SHUT_WR", "SHUT_RDWR", "INVALID"})[how & 0b11]);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE int
fcntl(int fd, int cmd, ...) {
  Sock* s;
  va_list ptr;
  va_start(ptr, cmd);
  void* arg = va_arg(ptr, void*);

  int r = libc_fcntl(fd, cmd, arg);

  if((s = intercept_find(&intercept_fds, fd))) {
    put_process();
    buffer_puts(&o, "fcntl(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, cmd);
    buffer_puts(&o, ", 0x");
    buffer_putxlong(&o, (long)arg);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
  }

  return r;
}

VISIBLE
int fcntl64(int fd, int cmd, ...) __attribute__((/*weak,*/ alias("fcntl")));

VISIBLE int
poll(struct pollfd* pfds, nfds_t nfds, int timeout) {
  for(nfds_t i = 0; i < nfds; i++) {
    Sock* s;

    if((s = intercept_findall(pfds[i].fd))) {
      if(pfds[i].events == 0)
        continue;

      put_process();
      buffer_puts(&o, "poll(pfds, ");
      buffer_putulong(&o, nfds);
      buffer_puts(&o, ", ");
      buffer_putlong(&o, timeout);
      buffer_puts(&o, ") socket ");
      buffer_putlong(&o, pfds[i].fd);
      buffer_puts(&o, " wants ");
      put_events(pfds[i].events);
      buffer_putnlflush(&o);
    }
  }

  int r = libc_poll(pfds, nfds, timeout);

  for(nfds_t i = 0; i < nfds; i++) {
    Sock* s;

    if((s = intercept_findall(pfds[i].fd))) {
      if(pfds[i].revents == 0)
        continue;

      put_process();
      buffer_puts(&o, "poll(pfds, ");
      buffer_putulong(&o, nfds);
      buffer_puts(&o, ", ");
      buffer_putlong(&o, timeout);
      buffer_puts(&o, ") socket ");
      buffer_putlong(&o, pfds[i].fd);
      buffer_puts(&o, " got ");
      put_events(pfds[i].revents);

      buffer_putnlflush(&o);
    }
  }

  return r;
}

VISIBLE int
SSL_read(void* ssl, void* buf, int len) {
  int r = openssl_ssl_read(ssl, buf, len);
  Sock* s;

  if(ssl && (s = intercept_ssl(ssl))) {
    put_process();
    buffer_puts(&o, "SSL_read(");
    buffer_putlong(&o, s->fd);
    buffer_puts(&o, ", ");

    if(r > 0)
      put_buf(buf, r);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r > 0)
      s->read += r;
    else if(openssl_ssl_get_error(ssl, r) == 6 /* SSL_ERROR_ZERO_RETURN */)
      s->closed = 1;
    else
      intercept_seterror(s, -1);
  }

  return r;
}

VISIBLE int
SSL_write(void* ssl, void* buf, int len) {
  int r = openssl_ssl_write(ssl, buf, len);
  Sock* s;

  if(ssl && (s = intercept_ssl(ssl))) {
    put_process();
    buffer_puts(&o, "SSL_write(");
    buffer_putlong(&o, s->fd);
    buffer_puts(&o, ", ");
    put_buf(buf, len);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r >= 0)
      s->written += r;
    else
      intercept_seterror(s, -1);
  }

  return r;
}

VISIBLE int
SSL_connect(void* ssl) {
  int r = openssl_ssl_connect(ssl);
  Sock* s;

  if(ssl && (s = intercept_ssl(ssl))) {
    put_process();
    buffer_puts(&o, "SSL_connect(");
    buffer_putlong(&o, s->fd);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r <= 0)
      intercept_seterror(s, -1);
  }

  return r;
}

VISIBLE int
SSL_accept(void* ssl) {
  int r = openssl_ssl_accept(ssl);
  Sock* s;

  if(ssl && (s = intercept_ssl(ssl))) {
    put_process();
    buffer_puts(&o, "SSL_accept(");
    buffer_putlong(&o, s->fd);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r <= 0)
      intercept_seterror(s, -1);
  }

  return r;
}

VISIBLE int
SSL_shutdown(void* ssl) {
  int r = openssl_ssl_shutdown(ssl);
  Sock* s;

  if(ssl && (s = intercept_ssl(ssl))) {
    put_process();
    buffer_puts(&o, "SSL_shutdown(");
    buffer_putlong(&o, s->fd);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);

    if(r <= 0)
      intercept_seterror(s, -1);
    else {
      list_del(&s->link);
      list_add(&s->link, &intercept_fds);
    }
  }

  return r;
}

CTOR static void
intercept_constructor(void) {
  long was_initialized = intercept_init();

  if(was_initialized)
    return;

#ifdef DEBUG_OUTPUT
  put_process();
  buffer_puts(&o, "intercept_constructor() called (was_initialized = ");
  buffer_putlong(&o, was_initialized);
  buffer_putsflush(&o, ")\n");
#endif
}

static long
intercept_init(void) {
  long prev_val = __CAS(&initialized, 0, 1);

  if(prev_val == 0) {
    init_list_head(&intercept_fds);
    init_list_head(&ssl_fds);
    init_list_head(&closed_fds);

    libc_socket = dlsym(RTLD_NEXT, "socket");
    libc_listen = dlsym(RTLD_NEXT, "listen");
    libc_bind = dlsym(RTLD_NEXT, "bind");
    libc_connect = dlsym(RTLD_NEXT, "connect");
    libc_accept = dlsym(RTLD_NEXT, "accept");
    libc_accept4 = dlsym(RTLD_NEXT, "accept4");
    libc_send = dlsym(RTLD_NEXT, "send");
    libc_sendto = dlsym(RTLD_NEXT, "sendto");
    libc_sendmsg = dlsym(RTLD_NEXT, "sendmsg");
    libc_sendmmsg = dlsym(RTLD_NEXT, "sendmmsg");
    libc_write = dlsym(RTLD_NEXT, "write");
    libc_writev = dlsym(RTLD_NEXT, "writev");
    libc_recv = dlsym(RTLD_NEXT, "recv");
    libc_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
    libc_recvmsg = dlsym(RTLD_NEXT, "recvmsg");
    libc_recvmmsg = dlsym(RTLD_NEXT, "recvmmsg");
    libc_read = dlsym(RTLD_NEXT, "read");
    libc_readv = dlsym(RTLD_NEXT, "readv");
    libc_getsockopt = dlsym(RTLD_NEXT, "getsockopt");
    libc_setsockopt = dlsym(RTLD_NEXT, "setsockopt");
    libc_getsockname = dlsym(RTLD_NEXT, "getsockname");
    libc_getpeername = dlsym(RTLD_NEXT, "getpeername");
    libc_close = dlsym(RTLD_NEXT, "close");
    libc_shutdown = dlsym(RTLD_NEXT, "shutdown");
    libc_fcntl = dlsym(RTLD_NEXT, "fcntl");
    libc_poll = dlsym(RTLD_NEXT, "poll");

    void* libssl = dlopen("libssl.so", RTLD_NOW);

    openssl_ssl_connect = dlsym(libssl, "SSL_connect");
    openssl_ssl_accept = dlsym(libssl, "SSL_accept");
    openssl_ssl_read = dlsym(libssl, "SSL_read");
    openssl_ssl_write = dlsym(libssl, "SSL_write");
    openssl_ssl_get_rfd = dlsym(libssl, "SSL_get_rfd");
    openssl_ssl_get_wfd = dlsym(libssl, "SSL_get_wfd");
    openssl_ssl_get_error = dlsym(libssl, "SSL_get_error");
    openssl_ssl_shutdown = dlsym(libssl, "SSL_shutdown");

    char* file = getenv("SOCKET_INTERCEPT_LOG");
    bool append = false;

    if(file && file[0] == '+') {
      ++file;
      append = true;
    }

    int fd;

    if((fd = (append ? open_append : open_trunc)(file && file[0] ? file : "socket-intercept.log")) == -1)
      fd = STDERR_FILENO;

    buffer_write_fd(&o, fd);
    buffer_2->op = o.op = (buffer_op_proto*)libc_write;

    ssize_t n = 0;

    if((fd = open_read("/proc/self/cmdline")) != -1) {
      n = read(fd, procname, sizeof(procname));
      close(fd);
    }

    for(ssize_t i = 0; i < n; i++) {
      if(iscntrl(procname[i]) || isspace(procname[i])) {
        n = i;
        break;
      }
    }

    size_t p;

    if((p = byte_rchrs(procname, n, "/\\", 2)) + 1 < n) {
      size_t l = n - ++p;
      str_copyn(procname, &procname[p], l);
    }
  }

  return prev_val;
}

static Sock*
intercept_find(struct list_head* list, int fd) {
  struct list_head* el;

  list_for_each(el, list) {
    Sock* s = list_entry(el, Sock, link);

    if(s->fd == fd)
      return s;
  }

  return 0;
}

static Sock*
intercept_findall(int fd) {
  Sock* s;

  if((s = intercept_find(&ssl_fds, fd)))
    return s;

  if((s = intercept_find(&intercept_fds, fd)))
    return s;

  return 0;
}

static Sock*
intercept_ssl(void* ssl) {
  int fd;
  Sock* s;

  if((fd = openssl_ssl_get_rfd(ssl)) < 0) {
    buffer_putsflush(buffer_2, "SSL_get_rfd error\n");
    exit(EXIT_FAILURE);
    return 0;
  }

  if(!(s = intercept_find(&intercept_fds, fd)))
    s = intercept_new(fd);

  list_del(&s->link);
  list_add(&s->link, &ssl_fds);

  return s;
}

static Sock*
intercept_new(int fd) {
  Sock* s;

  intercept_constructor();

  if(intercept_find(&intercept_fds, fd)) {
    buffer_puts(buffer_2, "Socket ");
    buffer_putlong(buffer_2, fd);
    buffer_putsflush(buffer_2, " already opened\n");
    return 0;
  }

  if((s = intercept_find(&closed_fds, fd)))
    list_del(&s->link);

  if(s || (s = alloc(sizeof(Sock)))) {
    memset(s, 0, sizeof(*s));

    list_add(&s->link, &intercept_fds);
    s->fd = fd;
  }

  return s;
}

static void
intercept_close(Sock* s) {
  list_del(&s->link);
  list_add(&s->link, &closed_fds);

  s->closed = 1;
}

static void
intercept_delete(Sock* s) {
  list_del(&s->link);
  alloc_free(s);
}

static void
intercept_list_clean(struct list_head* list) {
  struct list_head *el, *el2;

  list_for_each_safe(el, el2, list) {
    Sock* s = list_entry(el, Sock, link);

    intercept_delete(s);
  }
}

static void
intercept_cleanup(void) {
  intercept_list_clean(&intercept_fds);
  intercept_list_clean(&closed_fds);
}

static void
intercept_seterror(Sock* s, int retval) {
  s->ret_val = retval;

  if(retval < 0)
    s->error = errno;
}
