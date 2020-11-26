#include "lib/buffer.h"
#include "lib/unix.h"
#include "lib/errmsg.h"
#include "lib/ssl.h"
#include "lib/uint16.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/socket.h"
#include "lib/ndelay.h"
#include "lib/alloc.h"

#include "address.h"
#include <signal.h>

static address_t host;
static uint16 port;
fd_t sock = -1;

fd_t
ssltest_connect(const address_t* addr, uint16 port) {
  fd_t s;
  int ret;

  if((s = (addr->ip6 ? socket_tcp6 : socket_tcp4)()) != -1) {

    ndelay_on(s);
    io_fd(s);

    if(addr->ip6)
      ret = socket_connect6(s, addr->ip, port, addr->scope_id);
    else
      ret = socket_connect4(s, addr->ip, port);

    if(ret == 0 || (ret == -1 && errno == EINPROGRESS)) {
      errno = 0;
      io_wantwrite(s);
    } else {
      errmsg_warnsys("socket connect:", 0);
      io_close(s);
      s = -1;
    }
  }
  return s;
}

void
ssltest_loop(fd_t s) {
  buffer in, out;
  fd_t fd;
  ssize_t ret;
  ssl_t* ssl;

  buffer_init_free(&in, (buffer_op_sys*)(void*)&ssl_read, s, alloc(1024), 1024);
  buffer_init_free(&out, (buffer_op_sys*)(void*)&ssl_write, s, alloc(1024), 1024);

  ssl = ssl_client(s);

  ssl_io(s);

  for(;;) {
    io_wait();

    while((fd = io_canread()) != -1) {
      if(fd == s) {
#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "can read: ");
        buffer_putlong(buffer_2, fd);
        buffer_putnlflush(buffer_2);
#endif

        if((ret = buffer_feed(&in)) > 0) {
          buffer_puts(buffer_2, "data: ");
          buffer_copy(buffer_2, &in);
          buffer_putnlflush(buffer_2);
        } else if(ret == -1) {
          if(errno != EAGAIN) {
            errmsg_warnsys("socket read error: ", 0);
            return;
          }
        } else {
          errmsg_warn("socket eof", 0);
          return;
        }
      }
    }

    while((fd = io_canwrite()) != -1) {
      if(fd == s) {

        buffer_puts(buffer_2, "can write: ");
        buffer_putlong(buffer_2, fd);
        buffer_putnlflush(buffer_2);

        if(ssl_connect(s) == 1) {

          buffer_puts(buffer_2, "handshare complete");
          buffer_putnlflush(buffer_2);

          buffer_puts(&out, "USER x x x :Roman Senn\r\n");
          buffer_puts(&out, "NICK roman\r\n");
          buffer_flush(&out);
        }

        //     io_onlywantread(fd);
      }
    }
  }
}

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] <HOST> <PORT>\n"
                       "\n"
                       "Options:\n"
                       "\n"
                       "  -h, --help              Show this help\n"
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int c, index;
  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {0, 0, 0, 0}};

  errmsg_iam(str_basename(argv[0]));
#if !WINDOWS_NATIVE
  signal(SIGPIPE, SIG_IGN);
#endif
  ssl_init(0, 0);
  // ssl_new_client(0);

  for(;;) {
    c = unix_getopt_long(argc, argv, "ho:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;

      default: usage(argv[0]); return 1;
    }
  }
  ssl_init("http.key", "http.crt");

  if(unix_optind < argc) {
    if(!address_scan(argv[unix_optind++], &host))
      address_init(&host, "\377\0\0\1", false, 0);
  } else {
    errmsg_warn("ERROR: ", "host address required!", 0);
    usage(argv[0]);
    return 1;
  }
  if(unix_optind < argc) {
    if(!scan_ushort(argv[unix_optind++], &port))
      port = 6679;
  } else {
    errmsg_warn("ERROR: ", "port number required!", 0);
    usage(argv[0]);
    return 1;
  }

  sock = ssltest_connect(&host, port);
  ssltest_loop(sock);

  return 0;
}
