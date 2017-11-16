#include "buffer.h"
#include "http.h"
#include "io.h"
#include "iopause.h"
#include "taia.h"

int main(int argc, char* argv[]) {

  http h;
  iopause_fd iop;
  static struct taia a1, a2;

  http_init(&h, "www.bern.ch", 80);
  int ret = http_get(&h, "/");

  buffer_puts(buffer_1, "http_get() = ");
  buffer_putlong(buffer_1, (long)ret);
  buffer_putnlflush(buffer_1);

  byte_zero(&iop, sizeof(iopause_fd));
  iop.fd = h.sock;
  iop.events = IOPAUSE_READ|IOPAUSE_WRITE;

  taia_uint(&a1, 30);
  iopause(&iop, 1, &a1, &a2);

  return 0;
}
