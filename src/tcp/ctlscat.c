#include "../../lib/buffer.h"
#include "../../lib/errmsg.h"
#include "../../lib/ndelay.h"
#include "../../lib/scan.h"
#include "../../lib/socket.h"
#include "../../lib/stralloc.h"
#include "../../lib/str.h"
#include "../../lib/tls.h"
#include "../../lib/unix.h"
#include "../dnsutils/address.h"

#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

static int plain, listening;
static const char *key_file, *cert_file;
static uint16 port;

static void
usage(const char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [-n] HOST PORT\n"
                       "       ",
                       str_basename(argv0),
                       " -l [-n] [-C CERT -K KEY] [-p PORT | PORT]\n"
                       "\n"
                       "Connects to HOST PORT, or waits for one connection with -l, and relays\n"
                       "standard input to it and it to standard output, like netcat does. The\n"
                       "connection is TLS unless -n is given.\n"
                       "\n"
                       "  -l, --listen         wait for a connection\n"
                       "  -n, --plain          plain TCP, no TLS\n"
                       "  -C, --cert FILE      certificate (PEM) for -l, with TLS\n"
                       "  -K, --key FILE       private key (PEM) for -l, with TLS\n"
                       "  -p, --port PORT      port to listen on\n"
                       "  -h, --help           show this help\n",
                       NULL);
  buffer_flush(buffer_1);
}

static ssize_t
net_read(int s, void* buf, size_t n) {
  return plain ? read(s, buf, n) : tls_read(s, buf, n);
}

static ssize_t
net_write(int s, const void* buf, size_t n) {
  return plain ? write(s, buf, n) : tls_write(s, buf, n);
}

/* waits until fd can do what the TLS layer asked for, or just a while */
static void
wait_fd(int fd, short events) {
  struct pollfd p;

  p.fd = fd;
  p.events = events;
  p.revents = 0;
  poll(&p, 1, 1000);
}

/* writes all n bytes, the socket is non-blocking */
static int
send_all(int s, const char* x, size_t n) {
  ssize_t w;

  while(n) {
    if((w = net_write(s, x, n)) > 0) {
      x += w;
      n -= (size_t)w;
    } else if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
      wait_fd(s, POLLIN | POLLOUT);
    } else {
      errmsg_warnsys("write error", 0);
      return -1;
    }
  }
  return 0;
}

static int
accept_from(uint16 p) {
  char ip[16];
  uint16 peer;
  int l, s;

  if((l = socket_tcp4b()) == -1) {
    errmsg_warnsys("socket", 0);
    return -1;
  }

  if(socket_bind4_reuse(l, "\0\0\0\0", p) == -1 || socket_listen(l, 1) == -1) {
    errmsg_warnsys("cannot listen on the port", 0);
    return -1;
  }

  if((s = socket_accept4(l, ip, &peer)) == -1) {
    errmsg_warnsys("accept", 0);
    return -1;
  }
  close(l);
  return s;
}

/* the TLS handshake on a blocking socket, as client or as server */
static int
handshake(int s) {
  if(listening) {
    if(!tls_server(s) || tls_accept(s) != 1) {
      errmsg_warn("TLS handshake failed: ", tls_strerror(s), 0);
      return -1;
    }
  } else {
    if(!tls_client(s) || tls_connect(s) != 1) {
      errmsg_warn("TLS handshake failed: ", tls_strerror(s), 0);
      return -1;
    }
  }
  return 0;
}

/* copies stdin to the socket and the socket to stdout until the peer is gone */
static int
relay(int s) {
  struct pollfd p[2];
  char buf[4096];
  ssize_t n;
  int stdin_open = 1;

  ndelay_on(s);

  for(;;) {
    p[0].fd = stdin_open ? 0 : -1;
    p[0].events = POLLIN;
    p[1].fd = s;
    p[1].events = POLLIN;
    p[0].revents = p[1].revents = 0;

    if(poll(p, 2, -1) < 0) {
      if(errno == EINTR)
        continue;
      errmsg_warnsys("poll", 0);
      return 1;
    }

    if(p[0].revents & (POLLIN | POLLHUP)) {
      if((n = read(0, buf, sizeof(buf))) > 0) {
        if(send_all(s, buf, (size_t)n) < 0)
          return 1;
      } else {
        stdin_open = 0;

        /* a plain peer is told there is no more, a TLS peer closes after the reply */
        if(plain)
          shutdown(s, SHUT_WR);
      }
    }

    if(p[1].revents & (POLLIN | POLLHUP | POLLERR)) {
      /* TLS keeps decrypted bytes the socket does not show, read until it would block */
      for(;;) {
        if((n = net_read(s, buf, sizeof(buf))) > 0) {
          buffer_put(buffer_1, buf, (size_t)n);
          buffer_flush(buffer_1);
          continue;
        }

        if(n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
          break;

        return n < 0 && errno != 0 && errno != ECONNRESET ? 1 : 0;
      }
    }
  }
}

int
main(int argc, char* argv[]) {
  static const struct unix_longopt opts[] = {{"listen", 0, NULL, 'l'},
                                             {"plain", 0, NULL, 'n'},
                                             {"cert", 1, NULL, 'C'},
                                             {"key", 1, NULL, 'K'},
                                             {"port", 1, NULL, 'p'},
                                             {"help", 0, NULL, 'h'},
                                             {0, 0, 0, 0}};
  int c, index = 0, s, ret;
  unsigned long v;

  errmsg_iam(argv[0]);

  while((c = unix_getopt_long(argc, argv, "lnC:K:p:h", opts, &index)) != -1) {
    switch(c) {
      case 'l': listening = 1; break;
      case 'n': plain = 1; break;
      case 'C': cert_file = unix_optarg; break;
      case 'K': key_file = unix_optarg; break;
      case 'p':
        if(!scan_ulong(unix_optarg, &v) || v > 65535) {
          errmsg_warn("invalid port ", unix_optarg, 0);
          return 2;
        }
        port = (uint16)v;
        break;
      case 'h': usage(argv[0]); return 0;
      default: return 2;
    }
  }

  signal(SIGPIPE, SIG_IGN);

  if(listening) {
    if(unix_optind < argc && !port) {
      if(!scan_ulong(argv[unix_optind], &v) || v > 65535) {
        errmsg_warn("invalid port ", argv[unix_optind], 0);
        return 2;
      }
      port = (uint16)v;
    }

    if(!port) {
      usage(argv[0]);
      return 2;
    }

    if(!plain && (!cert_file || !key_file)) {
      errmsg_warn("listening with TLS needs -C CERT and -K KEY", 0);
      return 2;
    }

    if(!plain)
      tls_init(key_file, cert_file);

    if((s = accept_from(port)) == -1)
      return 1;
  } else {
    if(argc - unix_optind != 2 || !scan_ulong(argv[unix_optind + 1], &v) || v > 65535) {
      usage(argv[0]);
      return 2;
    }

    if(!plain)
      tls_init(0, 0);

    if((s = address_connect(argv[unix_optind], (uint16)v)) == -1)
      return 1;
  }

  if(!plain && handshake(s) < 0)
    return 1;

  ret = relay(s);

  if(!plain)
    tls_close(s);
  close(s);
  return ret;
}
