#include "buffer.h"
#include "http.h"
#include "io.h"
#include "iopause.h"
#include "taia.h"
#include "byte.h"
#include <sys/socket.h>
#include <errno.h>.h>

static int last_errno = 0;

static ssize_t
do_recv(int64 s, void* buf, size_t len) {
  ssize_t ret = recv(s, buf, len, 0);
  if(ret == -1) {
    last_errno = errno;
    errno = 0;
/*    if(errno == EAGAIN) {
      errno = 0;
      ret = -1;
    }*/
  }
  return ret;
}

int main(int argc, char* argv[]) {

  http h;
  iopause_fd iop;
  static struct taia a1, a2;
  static buffer in;
  char inbuf[8192];
  int ret;

  http_init(&h, "map.bern.ch", 80);
  ret = http_get(&h, "/stadtplan/");

  buffer_init(&in, do_recv, h.sock, inbuf, sizeof(inbuf));

  buffer_puts(buffer_1, "http_get() = ");
  buffer_putlong(buffer_1, (long)ret);
  buffer_putnlflush(buffer_1);

  byte_zero(&iop, sizeof(iop));
  iop.fd = h.sock;
  iop.events = /*IOPAUSE_READ|*/IOPAUSE_WRITE;

  taia_uint(&a1, 30); taia_uint(&a2, 0);
  iopause(&iop, 1, &a1, &a2);

  http_sendreq(&h);

  iop.events = IOPAUSE_READ;

  taia_uint(&a1, 30); taia_uint(&a2, 0 );
  iopause(&iop, 1, &a1, &a2);

  stralloc line;
  stralloc_init(&line);

  io_block(h.sock);

  while((ret = buffer_getline_sa(&in, &line))) {
    buffer_puts(buffer_1, "Line: ");
    buffer_putsa(buffer_1, &line);
    buffer_puts(buffer_1, " (len==");
    buffer_putlong(buffer_1, line.len);
    buffer_puts(buffer_1, ")");
    buffer_putnlflush(buffer_1);
    stralloc_zero(&line);

    if(line.len > 0 && line.s[0] == '\0')
      break;

    //if(last_errno == EAGAIN)      break;
  }

  return 0;
}
