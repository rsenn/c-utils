#define _GNU_SOURCE 1

#include <stdatomic.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>
//#include <pthread.h>

#include "lib/alloc.h"
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

static void init(void);
static Sock* socket_intercept_find(struct list_head*, int);
static void socket_intercept_close(Sock*);
static Sock* socket_intercept_new(int);
static void socket_intercept_delete(Sock*);
static void socket_intercept_seterror(Sock*, int);
static void socket_intercept_cleanup(void);

// static pthread_mutex_t log_mut = PTHREAD_MUTEX_INITIALIZER;
static thread_local long initialized;
static thread_local struct list_head socket_intercept_fds, closed_fds;
static thread_local buffer o;

typedef int socket_intercept_function(int, int, int);
typedef int bind_function(int, const struct sockaddr*, socklen_t);
typedef int connect_function(int, const struct sockaddr*, socklen_t);
typedef int accept_function(int, struct sockaddr*, socklen_t*);
typedef ssize_t send_function(int, const void*, size_t, int);
typedef ssize_t sendto_function(int, const void*, size_t, int, const struct sockaddr*, socklen_t);
typedef ssize_t sendmsg_function(int, const struct msghdr*, int);
typedef ssize_t write_function(int, const void*, size_t);
typedef ssize_t recv_function(int, void*, size_t, int);
typedef ssize_t recvfrom_function(int, void*, size_t, int, struct sockaddr*, socklen_t*);
typedef ssize_t recvmsg_function(int, struct msghdr*, int);
typedef ssize_t read_function(int, void*, size_t);
typedef int getsockopt_function(int, int, int, void*, socklen_t*);
typedef int setsockopt_function(int, int, int, const void*, socklen_t);
typedef int getsockname_function(int, struct sockaddr*, socklen_t*);
typedef int getpeername_function(int, struct sockaddr*, socklen_t*);
typedef int close_function(int);

static thread_local socket_intercept_function* libc_socket;
static thread_local bind_function* libc_bind;
static thread_local connect_function* libc_connect;
static thread_local accept_function* libc_accept;
static thread_local send_function* libc_send;
static thread_local sendto_function* libc_sendto;
static thread_local sendmsg_function* libc_sendmsg;
static thread_local write_function* libc_write;
static thread_local recv_function* libc_recv;
static thread_local recvfrom_function* libc_recvfrom;
static thread_local recvmsg_function* libc_recvmsg;
static thread_local read_function* libc_read;
static thread_local getsockopt_function* libc_getsockopt;
static thread_local setsockopt_function* libc_setsockopt;
static thread_local getsockname_function* libc_getsockname;
static thread_local getpeername_function* libc_getpeername;
static thread_local close_function* libc_close;
static const char* address_families[] = {"AF_UNSPEC", "AF_LOCAL",  "AF_INET",   "AF_AX25",   "AF_IPX",     "AF_APPLETALK", "AF_NETROM", "AF_BRIDGE",  "AF_ATMPVC",
                                         "AF_X25",    "AF_INET6",  "AF_ROSE",   "AF_DECnet", "AF_NETBEUI", "AF_SECURITY",  "AF_KEY",    "AF_NETLINK", "AF_PACKET",
                                         "AF_ASH",    "AF_ECONET", "AF_ATMSVC", 0,           "AF_SNA",     "AF_IRDA",      "AF_PPPOX",  "AF_WANPIPE"};
static void
put_process(buffer* b) {
  char buf[FMT_LONG];

  buffer_puts(b, "[");
  buffer_put(b, buf, fmt_ulong(buf, getpid()));
  buffer_puts(b, "] ");
}

static size_t
fmt_sockaddr(char x[64], const struct sockaddr* addr, socklen_t addrlen) {
  size_t n = 0;
  uint16 port = 0;

  if(addr == 0)
    return str_copy(x, "NULL");

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
        n += str_copy(&x[n], "unhandled address str: ");
        n += str_copy(&x[n], str);
      } else {
        n += str_copy(&x[n], "unknown address str: ");
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

static inline int
put_sockaddr(buffer* b, const struct sockaddr* addr, socklen_t addrlen) {
  char buf[64];
  return buffer_put(b, buf, fmt_sockaddr(buf, addr, addrlen));
}

static void
put_msg(buffer* b, const struct msghdr* msg) {
  char buf[msg->msg_namelen * 3 + 1];
  size_t len;

  if((len = fmt_hexbs(buf, msg->msg_name, msg->msg_namelen))) {
    buffer_puts(b, "Name: ");
    buffer_put(b, buf, len);
    buffer_puts(b, ", ");
  }

  if((len = msg->msg_iovlen) > 0) {
    buffer_puts(b, "IOV len: ");
    buffer_putulong(b, len);
    buffer_puts(b, " [ ");
  }

  for(size_t i = 0; i < len; i++) {
    const struct iovec* const iv = &msg->msg_iov[i];
    char buf2[iv->iov_len * 3 + 1];

    if(i > 0)
      buffer_puts(b, ", ");

    buffer_put(b, buf2, fmt_hexbs(buf2, iv->iov_base, iv->iov_len));
  }
}

static long
initialize(void) {
  long prev_val = __CAS(&initialized, 1, 0);

  if(prev_val == 0) {
    buffer_write_fd(&o, open("socket-intercept.log", O_WRONLY | O_TRUNC, 0644));

    init_list_head(&socket_intercept_fds);
    init_list_head(&closed_fds);

    libc_socket = dlsym(RTLD_NEXT, "socket");
    libc_bind = dlsym(RTLD_NEXT, "bind");
    libc_connect = dlsym(RTLD_NEXT, "connect");
    libc_accept = dlsym(RTLD_NEXT, "accept");
    libc_send = dlsym(RTLD_NEXT, "send");
    libc_sendto = dlsym(RTLD_NEXT, "sendto");
    libc_sendmsg = dlsym(RTLD_NEXT, "sendmsg");
    libc_write = dlsym(RTLD_NEXT, "write");
    libc_recv = dlsym(RTLD_NEXT, "recv");
    libc_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
    libc_recvmsg = dlsym(RTLD_NEXT, "recvmsg");
    libc_read = dlsym(RTLD_NEXT, "read");
    libc_getsockopt = dlsym(RTLD_NEXT, "getsockopt");
    libc_setsockopt = dlsym(RTLD_NEXT, "setsockopt");
    libc_getsockname = dlsym(RTLD_NEXT, "getsockname");
    libc_getpeername = dlsym(RTLD_NEXT, "getpeername");
    libc_close = dlsym(RTLD_NEXT, "close");
  }

  return prev_val;
}

__attribute__((constructor))
/*[[constructor]]*/ static void
init(void) {
  buffer_puts(buffer_2, "init() called (initialized: ");
  buffer_putlong(buffer_2, initialize());
  buffer_putsflush(buffer_2, ")\n");
}

int
socket(int af, int type, int protocol) {
  int r = libc_socket(af, type, protocol);

  if(r >= 0)
    socket_intercept_new(r);

  return r;
}

int
bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
  Sock* s;
  int r = libc_bind(sockfd, addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "bind(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_sockaddr(&o, addr, addrlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

int
connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
  Sock* s;
  int r = libc_connect(sockfd, addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "connect(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_sockaddr(&o, addr, addrlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

int
accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_accept(sockfd, addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char buf[*addrlen * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "accept(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(&o, addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, *addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  if(r >= 0)
    socket_intercept_new(r);

  return r;
}

ssize_t
send(int sockfd, const void* buf, size_t len, int flags) {
  Sock* s;
  int r = libc_send(sockfd, buf, len, flags);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char hexbuf[len * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "send(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_put(&o, hexbuf, fmt_hexbs(hexbuf, buf, len));
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r >= 0)
      s->written += r;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
sendto(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen) {
  Sock* s;
  int r = libc_sendto(sockfd, buf, len, flags, dest_addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char hexbuf[len * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "sendto(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_put(&o, hexbuf, fmt_hexbs(hexbuf, buf, len));
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ", ");
    put_sockaddr(&o, dest_addr, addrlen);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r >= 0)
      s->written += r;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
sendmsg(int sockfd, const struct msghdr* msg, int flags) {
  Sock* s;
  int r = libc_sendmsg(sockfd, msg, flags);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "sendmsg(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    put_msg(&o, msg);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r >= 0)
      s->written += r;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
write(int fd, const void* buf, size_t len) {
  Sock* s;
  int r = libc_write(fd, buf, len);

  if((s = socket_intercept_find(&socket_intercept_fds, fd))) {
    char hexbuf[len * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "write(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    buffer_put(&o, hexbuf, fmt_hexbs(hexbuf, buf, len));
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r >= 0)
      s->written += r;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
recv(int sockfd, void* buf, size_t len, int flags) {
  Sock* s;
  int r = libc_recv(sockfd, buf, len, flags);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char hexbuf[len * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "recv(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    buffer_put(&o, r > 0 ? hexbuf : "[]", r > 0 ? fmt_hexbs(hexbuf, buf, r) : 2);

    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char hexbuf[len * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "recvfrom(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_put(&o, r > 0 ? hexbuf : "[]", r > 0 ? fmt_hexbs(hexbuf, buf, r) : 2);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(&o, src_addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");

    if(r >= 0)
      buffer_putulong(&o, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
recvmsg(int sockfd, struct msghdr* msg, int flags) {
  Sock* s;
  int r = libc_recvmsg(sockfd, msg, flags);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "recvmsg(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_msg(&o, msg);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putlong(&o, flags);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

ssize_t
read(int fd, void* buf, size_t len) {
  Sock* s;
  int r = libc_read(fd, buf, len);

  if((s = socket_intercept_find(&socket_intercept_fds, fd))) {
    char hexbuf[len * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "read(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ", ");
    buffer_put(&o, r > 0 ? hexbuf : "[]", r > 0 ? fmt_hexbs(hexbuf, buf, r) : 2);
    buffer_puts(&o, ", ");
    buffer_putulong(&o, len);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);

    if(r > 0)
      s->read += r;
    else if(r == 0)
      s->closed = 1;
    else
      socket_intercept_seterror(s, r);
  }

  return r;
}

int
getsockopt(int sockfd, int level, int optname, void* optval, socklen_t* optlen) {
  Sock* s;
  int r = libc_getsockopt(sockfd, level, optname, optval, optlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char hexbuf[*optlen * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "getsockopt(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, level);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, optname);
    buffer_puts(&o, ", ");
    buffer_put(&o, r >= 0 ? hexbuf : "[]", r >= 0 ? fmt_hexbs(hexbuf, optval, *optlen) : 2);
    buffer_puts(&o, ", ");

    if(r >= 0)
      buffer_putulong(&o, *optlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

int
setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
  Sock* s;
  int r = libc_setsockopt(sockfd, level, optname, optval, optlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    char hexbuf[optlen * 3 + 1];

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "setsockopt(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, level);
    buffer_puts(&o, ", ");
    buffer_putlong(&o, optname);
    buffer_puts(&o, ", ");
    buffer_put(&o, hexbuf, fmt_hexbs(hexbuf, optval, optlen));
    buffer_puts(&o, ", ");
    buffer_putulong(&o, optlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

int
getsockname(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_getsockname(sockfd, addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "getsockname(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");

    if(r >= 0)
      put_sockaddr(&o, addr, *addrlen);
    else
      buffer_puts(&o, "[]");

    buffer_puts(&o, ", ");
    buffer_putulong(&o, *addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

int
getpeername(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
  Sock* s;
  int r = libc_getpeername(sockfd, addr, addrlen);

  if((s = socket_intercept_find(&socket_intercept_fds, sockfd))) {
    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "getpeername(");
    buffer_putlong(&o, sockfd);
    buffer_puts(&o, ", ");
    if(r >= 0)
      put_sockaddr(&o, addr, *addrlen);
    else
      buffer_puts(&o, "[]");
    buffer_puts(&o, ", ");
    buffer_putulong(&o, *addrlen);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

int
close(int fd) {
  Sock* s;
  int r = libc_close(fd);

  if((s = socket_intercept_find(&socket_intercept_fds, fd))) {
    socket_intercept_close(s);

    // pthread_mutex_lock(&log_mut);
    put_process(&o);
    buffer_puts(&o, "close(");
    buffer_putlong(&o, fd);
    buffer_puts(&o, ") = ");
    buffer_putlong(&o, r);
    buffer_putnlflush(&o);
    // pthread_mutex_unlock(&log_mut);
  }

  return r;
}

static Sock*
socket_intercept_find(struct list_head* list, int fd) {
  struct list_head* el;

  list_for_each(el, list) {
    Sock* s = list_entry(el, Sock, link);

    if(s->fd == fd)
      return s;
  }

  return 0;
}

static Sock*
socket_intercept_new(int fd) {
  Sock* s;

  init();

  // assert(!socket_intercept_find(&socket_intercept_fds, fd));

  if(socket_intercept_find(&socket_intercept_fds, fd)) {
    buffer_puts(buffer_2, "Socket ");
    buffer_putlong(buffer_2, fd);
    buffer_putsflush(buffer_2, " already opened\n");
    return 0;
  }

  if((s = socket_intercept_find(&closed_fds, fd)))
    list_del(&s->link);

  if(s || (s = alloc(sizeof(Sock)))) {
    memset(s, 0, sizeof(*s));

    list_add(&s->link, &socket_intercept_fds);
    s->fd = fd;
  }

  return s;
}

static void
socket_intercept_close(Sock* s) {
  list_del(&s->link);
  list_add(&s->link, &closed_fds);

  s->closed = 1;
}

static void
socket_intercept_delete(Sock* s) {
  list_del(&s->link);
  alloc_free(s);
}

static void
socket_intercept_list_clean(struct list_head* list) {
  struct list_head *el, *el2;

  list_for_each_safe(el, el2, list) {
    Sock* s = list_entry(el, Sock, link);

    socket_intercept_delete(s);
  }
}

static void
socket_intercept_cleanup(void) {
  socket_intercept_list_clean(&socket_intercept_fds);
  socket_intercept_list_clean(&closed_fds);
}

static void
socket_intercept_seterror(Sock* s, int retval) {
  s->ret_val = retval;

  if(retval < 0)
    s->error = errno;
}
