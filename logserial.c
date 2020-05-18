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
#include "lib/ndelay.h"
#include "lib/tai.h"
#include "lib/taia.h"
#include "lib/dir.h"
#include "lib/rdir.h"
#include <stdlib.h>
#include <unistd.h>

static strarray ports;
static fd_t input_fd = 0;
static stralloc input_buf;

/**
 * @brief      { function_description }
 *
 * @param[in]  dir   The dir
 * @param      arr   The arr
 */
void
dir_entries(const char* dir, strarray* arr) {
  rdir_t d;
  const char* entry;
  if(rdir_open(&d, dir) != 0) {
    errmsg_warnsys("ERROR: Opening directory /dev", 0);
    return;
  }
  while((entry = rdir_read(&d))) strarray_push(arr, entry);
  rdir_close(&d);
}

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
int64
serial_ports() {
  static int i;
  char** port;
  strarray temp, newports;
  strarray_init(&temp);
  strarray_init(&newports);
  dir_entries("/dev/", &temp);

  strarray_foreach(&temp, port) {
    if(!str_start(*port, "/dev/ttyA"))
      continue;

    // if(strarray_index_of(&ports, *port) == -1) {
    if(strarray_push_unique(&newports, *port)) {
      /*  buffer_putlong(buffer_2, i);
       buffer_puts(buffer_2, ": detected new port: ");
       buffer_puts(buffer_2, *port);
       buffer_putnlflush(buffer_2); */
    }
  }

  strarray_foreach(&ports, port) {
    if(strarray_index_of(&newports, *port) == -1) {
      /*   buffer_putlong(buffer_2, i);
        buffer_puts(buffer_2, ": disappeared port: ");
        buffer_puts(buffer_2, *port);
        buffer_putnlflush(buffer_2); */
    }
  }

  strarray_free(&ports);
  ports = newports;
  i++;
  return strarray_size(&ports);
}

/**
 * @brief      Gets the ports.
 *
 * @return     The ports.
 */
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
          // strarray_push(&ports, port);
          strarray_splice(&ports, 0, 0, 1, &port);
          /*  buffer_puts(buffer_2, "detected new port: ");
           buffer_puts(buffer_2, port);
           buffer_putnlflush(buffer_2); */
          r++;
        }
      }
    }
  }
  free(pvec);
  return strarray_size(&ports);
}

/**
 * @brief      { function_description }
 *
 * @param[in]  serial_fd  The serial fd
 * @param      buf        The buffer
 * @param[in]  len        The length
 * @param      ptr        The pointer
 *
 * @return     { description_of_the_return_value }
 */
ssize_t
serial_read(fd_t serial_fd, char* buf, size_t len, void* ptr) {
  ssize_t ret = read(serial_fd, buf, len);
  if(ret <= 0)
    return 0;
  return ret;
}

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
ssize_t
process_term() {
  char x[128];
  ssize_t ret;

  if((ret = read(input_fd, x, sizeof(x))) > 0) {
    buffer_puts(buffer_1, "Read ");
    buffer_putlong(buffer_1, ret);
    buffer_puts(buffer_1, " bytes from terminal");
    buffer_putnlflush(buffer_1);
  }

  return ret;
}

/**
 * @brief      { function_description }
 *
 * @param[in]  serial_fd  The serial fd
 *
 * @return     { description_of_the_return_value }
 */
ssize_t
process_serial(fd_t serial_fd) {
  char x[1024];
  ssize_t ret;
  if((ret = read(serial_fd, x, sizeof(x))) > 0) {
    stralloc_catb(&input_buf, x, ret);
    if(stralloc_contains(&input_buf, "\n")) {
      buffer_putsa(buffer_1, &input_buf);
      buffer_flush(buffer_1);
      stralloc_zero(&input_buf);
    }
  } else if(ret == 0) {
    errmsg_warn("serial closed", 0);
    io_dontwantread(serial_fd);
    io_close(serial_fd);
  } else if(ret < 0) {
    if(errno == EAGAIN) {
      io_eagain_read(serial_fd);
    } else {
      errmsg_warnsys("serial error", 0);
      io_dontwantread(serial_fd);
      io_close(serial_fd);
    }
  }
  return ret;
}

/**
 * @brief      { function_description }
 *
 * @param[in]  serial_fd  The serial fd
 *
 * @return     { description_of_the_return_value }
 */
ssize_t
process_loop(fd_t serial_fd, int64 timeout) {
  char x[1024];
  ssize_t ret;
  tai6464 t, deadline, msecs, diff;
  taia_now(&t);
  taia_uint(&msecs, timeout / 1000);
  msecs.nano = (timeout % 1000) * 1000000;
  taia_add(&deadline, &t, &msecs);
  for(;;) {
    taia_now(&t);
    taia_sub(&diff, &deadline, &t);
    int64 wait_msecs = taia_approx(&diff) * 1000;
    if(wait_msecs < 0)
      wait_msecs = 0;
    fd_t read_fd;
    // io_wantread(input_fd);
    // io_wantread(serial_fd);
    /* if(wait_msecs > 0) {
      buffer_puts(buffer_2, "wait msecs: ");
      buffer_putlonglong(buffer_2, wait_msecs);
      buffer_putnlflush(buffer_2);
    } */
    if((ret = io_waituntil2(wait_msecs)) < 0) {
      errmsg_warnsys("wait error: ", 0);
      break;
    } else {
      ret = 1;
    }
    while((read_fd = io_canread()) != -1) {
      if(read_fd == serial_fd) {
        if((ret = process_serial(serial_fd) <= 0)) {
          if(!(ret == -1 && errno == EAGAIN))
            return ret;
        }
      }
      if(read_fd == input_fd) {
        if((ret = process_term() <= 0))
          return ret;
      }
    }
    if(stralloc_length(&input_buf)) {
      buffer_putsa(buffer_1, &input_buf);
      buffer_flush(buffer_1);
      stralloc_zero(&input_buf);
    }
    if(wait_msecs <= 0)
      break;
  }
  return ret;
}

int
main() {
  int running = 1;
  fd_t serial_fd;
  io_fd(input_fd);
  io_nonblock(input_fd);

  while(running) {
    int64 i, newports;
    const char* portname = NULL;
    newports = get_ports();
    /*buffer_puts(buffer_2, "num ports: ");
    buffer_putlonglong(buffer_2, newports);
    buffer_putnlflush(buffer_2);*/
    if(newports == 0) {
      usleep(250 * 1000);
      continue;
    }
    portname = strarray_at(&ports, 0);
    buffer_puts(buffer_2, "portname: ");
    buffer_puts(buffer_2, portname);
    buffer_putnlflush(buffer_2);
    serial_fd = serial_open(portname, 38400);
    io_nonblock(serial_fd);
    io_closeonexec(serial_fd);

    if(serial_fd == -1) {
      usleep(250 * 1000);
      continue;
    }
    /*buffer_puts(buffer_2, "port opened: ");
    buffer_putlong(buffer_2, serial_fd);
    buffer_putnlflush(buffer_2); */
    // buffer_read_fd(&serial, serial_fd);
    io_fd(serial_fd);
    io_wantread(serial_fd);
    // serial.op = &read;
    while(process_loop(serial_fd, 250) > 0)
      ;
    io_dontwantread(serial_fd);
    io_close(serial_fd);
    int64 idx = strarray_index_of(&ports, portname);
    if(idx != -1) {
      /*buffer_puts(buffer_2, "removed port: ");
      buffer_puts(buffer_2, strarray_at(&ports, idx));
      buffer_putnlflush(buffer_2); */
      strarray_splice(&ports, idx, 1, 0, NULL);
    }
    /*buffer_puts(buffer_1, "Ports (");
    buffer_putulong(buffer_1, strarray_size(&ports));
    buffer_puts(buffer_1, "): "); */
    /*for(i = 0; i < strarray_size(&ports); i++) {
      const char* portstr = strarray_at(&ports, i);
      if(portstr == NULL)
        continue;
      if(i > 0)
        buffer_puts(buffer_1, " ");
      buffer_puts(buffer_1, portstr);
    }
    buffer_putnlflush(buffer_1); */
  }
  stralloc_free(&input_buf);
}
