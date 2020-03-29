#include "serial.h"
#include "serial.c"
#include "lib/str.h"
#include "lib/alloc.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/buffer.h"
#include "lib/errmsg.h"
#include "lib/uint64.h"
#include "lib/io.h"
#include <stdlib.h>
#include <unistd.h>

static strarray ports;

int64
get_ports() {

  int64 n = 0, r = 0;
  char** pvec;

  for(pvec = get_serial_ports(); pvec[n]; n++) {
    const char* port = pvec[n];
    size_t i = str_find(port, "tty");

    if(port[i]) {

      if(port[i + 3] == 'A' || port[i + 3] == 'U') {

        if(access(port, R_OK)) {
          if(errno != ENOENT && errno != ENODEV && errno != EACCES)
            errmsg_warnsys(port, 0);
          continue;
        }

        if(!strarray_contains(&ports, port)) {

          strarray_splice(&ports, 0, 0, 1, &port);

          buffer_puts(buffer_2, "detected new port: ");
          buffer_puts(buffer_2, port);
          buffer_putnlflush(buffer_2);

          r++;
        }
      }
    }
  }

  free(pvec);
  return r;
}

ssize_t
serial_read(fd_t fd, char* buf, size_t len, void* ptr) {

  ssize_t ret = read(fd, buf, len);

  if(ret <= 0)
    return 0;

  return ret;
}

ssize_t
process_input(buffer* input) {
  stralloc sa;
  char x[1024];
  ssize_t ret = 0;
  stralloc_init(&sa);

  for(;;) {
    ret = io_waitread(input->fd, x, sizeof(x));

    /*   ret = buffer_get(input, x, sizeof(x)); */

    if(ret > 0) {
      stralloc_catb(&sa, x, ret);

      if(stralloc_contains(&sa, "\n")) {
        // buffer_puts(buffer_1, "Output: ");
        buffer_putsa(buffer_1, &sa);
        buffer_flush(buffer_1);
        stralloc_zero(&sa);
      }
    } else if(ret == 0) {
      errmsg_warn("port closed", 0);
      break;
    } else if(ret < 0) {
      errmsg_warnsys("serial port:", 0);
      break;
    }
  }

  buffer_putsa(buffer_1, &sa);
  buffer_flush(buffer_1);
  stralloc_free(&sa);
  return ret;
}

int
main() {

  int running = 1;

  while(running) {

    int64 i, newports;
    fd_t fd;

    buffer serial;
    const char* portname = NULL;
    newports = get_ports();
    if(strarray_size(&ports) == 0) {
      usleep(1000);
      continue;
    }

    portname = strarray_at(&ports, 0);

    /*    buffer_puts(buffer_2, "portname: ");
       buffer_puts(buffer_2, portname);
       buffer_putnlflush(buffer_2); */

    fd = serial_open(portname, 38400);

    if(fd == -1) {
      usleep(1000000);
      continue;
    }
    /*
        buffer_puts(buffer_2, "port opened: ");
        buffer_putlong(buffer_2, fd);
        buffer_putnlflush(buffer_2); */

    buffer_read_fd(&serial, fd);
    io_fd(fd);

    // serial.op = &read;

    process_input(&serial);

    buffer_close(&serial);

    int64 idx = strarray_index_of(&ports, portname);

    if(idx != -1) {
      /*     buffer_puts(buffer_2, "removed port: ");
          buffer_puts(buffer_2, strarray_at(&ports, idx));
          buffer_putnlflush(buffer_2); */
      strarray_splice(&ports, idx, 1, 0, NULL);
    }

    buffer_puts(buffer_1, "Ports (");
    buffer_putulong(buffer_1, strarray_size(&ports));
    buffer_puts(buffer_1, "): ");

    for(i = 0; i < strarray_size(&ports); i++) {
      const char* portstr = strarray_at(&ports, i);
      if(portstr == NULL)
        continue;
      if(i > 0)
        buffer_puts(buffer_1, " ");
      buffer_puts(buffer_1, portstr);
    }
    buffer_putnlflush(buffer_1);
  }
}