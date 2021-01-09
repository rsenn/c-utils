#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/unix.h"
#include "lib/errmsg.h"
#include "lib/tls.h"
#include "lib/uint16.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/socket.h"
#include "lib/ndelay.h"
#include "lib/alloc.h"
#include "lib/fmt.h"
#include "lib/strlist.h"
#include "lib/textcode.h"
#include "lib/byte.h"

#include "address.h"
#include <signal.h>
#include <stdarg.h>
#include <ctype.h>

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

int
sendline_sa(buffer* b, const stralloc* data) {

  buffer_puts(buffer_2, "DATA → ");
  buffer_putsa(buffer_2, data);
  buffer_putnlflush(buffer_2);

  buffer_putsa(b, data);
  buffer_put(b, "\r\n", 2);
  return buffer_flush(b);
}

int
sendline_m(buffer* b, ...) {
  int r = 0;
  va_list a;
  const char* s;
  stralloc sa;
  stralloc_init(&sa);
  va_start(a, b);
  while((s = va_arg(a, const char*)))
    if(stralloc_cats(&sa, s) == -1) {
      r = -1;
      break;
    }
  va_end(a);

  r = sendline_sa(b, &sa);
  stralloc_free(&sa);
  return r;
}

void
ssltest_process_message(int ac, char** av, buffer* b) {
  if(isdigit(av[1][0])) {
    unsigned int num;
    if(scan_uint(av[1], &num) > 0) {

      switch(num) {

        case 422:
        case 376: {
          sendline_m(b, "JOIN #bots", 0);
          return;
        }
        case 366: {
          sendline_m(b,
                     "PRIVMSG #bots :Hello, "
                     "I'am an idiot.",
                     0);
          return;
        }
      }
    }
  }
}

void
ssltest_process_line(stralloc* line, buffer* b) {
  char* x;
  size_t p, n;
  strlist toks;
  char** av;
  int ac;

  buffer_puts(buffer_2, "DATA ← ");
  buffer_putsa(buffer_2, line);
  buffer_putnlflush(buffer_2);

  if(line->len > 5 && byte_equal(line->s, 5, "PING ")) {
    line->s[1] = 'O';
    sendline_sa(b, line);
    return;
  }

  x = line->s;
  n = line->len;
  p = 0;
  if(n == 0)
    return;
  strlist_init(&toks, '\0');
  if(*x == ':') {
    x++;
    n--;
  } else {
    strlist_push(&toks, "");
  }
  while(p < n) {
    size_t i = scan_nonwhitenskip(&x[p], n - p);
    strlist_pushb(&toks, &x[p], i);
    p += i;
    if(p == n)
      break;
    p += scan_whitenskip(&x[p], n - p);
    if(p == n)
      break;
    if(x[p] == ':') {
      p++;
      strlist_pushb(&toks, &x[p], n - p);
      break;
    }
  }
  av = strlist_to_argv(&toks);
  ac = strlist_count(&toks);

  if(ac >= 2)
    ssltest_process_message(ac, av, b);

  alloc_free(av);

  // strlist_dump(buffer_2, &toks);
  strlist_free(&toks);
}

void
ssltest_loop(fd_t s) {
  buffer in, out;
  fd_t fd;
  ssize_t ret;
  tls_t* ssl;
  bool login_sent = false;
  size_t iter = 0;
  stralloc line;
  stralloc_init(&line);
  buffer_init_free(&in, (buffer_op_sys*)(void*)&tls_read, s, alloc(1024), 1024);
  buffer_init_free(
      &out, (buffer_op_sys*)(void*)&tls_write, s, alloc(1024), 1024);
  ssl = tls_client(s);
  tls_io(s);
  for(;; iter++) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Iteration ");
    buffer_putulong(buffer_2, iter);
    buffer_putnlflush(buffer_2);
#endif
    io_wait();
    while((fd = io_canread()) != -1) {
      if(fd == s) {
#ifdef DEBUG_OUTPUT
        buffer_puts(buffer_2, "Can read fd#");
        buffer_putlong(buffer_2, fd);
        buffer_putnlflush(buffer_2);
#endif
        if((ret = buffer_feed(&in)) > 0) {
          while(buffer_getnewline_sa(&in, &line) > 0) {
            stralloc_trimr(&line, "\r\n", 2);
            stralloc_nul(&line);
            ssltest_process_line(&line, &out);
          }
        } else if(ret == -1) {
          if(errno != EAGAIN) {
            errmsg_warnsys("Socket read error: ", 0);
            if(errno > 0)
              return;
          }
        } else {
          errmsg_warn("Socket EOF", 0);
          return;
        }
      }
    }
    while((fd = io_canwrite()) != -1) {
      if(fd == s) {
#ifdef DEBUG_OUTPUT
        size_t i;
        char buf[16];
        buffer_puts(buffer_2, "Can write fd📡");
        buffer_putlong(buffer_2, fd);
        buffer_putspace(buffer_2);
        buffer_putlong(buffer_2, out.p);
        buffer_puts(buffer_2, ": ");
        for(i = 0; i < out.p; i++) {
          if(out.x[i] >= ' ')
            buffer_putc(buffer_2, out.x[i]);
          else
            buffer_put(buffer_2, buf, fmt_escapecharc(buf, out.x[i]));
        }
        //   buffer_putfmt(buffer_2,
        //   out.x, out.p,
        //   &fmt_escapecharnonprintable);
        buffer_putnlflush(buffer_2);
#endif
        buffer_flush(&out);
        if(out.p == 0)
          io_onlywantread(s);
      }
    }
    ret = 0;
    if(!tls_established(s)) {
      ret = tls_connect(s);
    }
    if((ret == 1 || tls_established(s)) && !login_sent) {
      buffer_puts(buffer_2, "Handshake complete.");
      buffer_putnlflush(buffer_2);
      sendline_m(&out,
                 "USER x x x :Roman "
                 "Senn\r\nNICK roman\r\n",
                 0);
      login_sent = true;
      //     io_onlywantwrite(s);
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
                       "  -h, --help              Show "
                       "this help\n"
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
  tls_init(0, 0);
  // tls_new_client(0);

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
  tls_init("http.key", "http.crt");

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
