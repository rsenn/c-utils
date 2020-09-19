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
#include "lib/path.h"
#include "lib/getopt.h"
#include "lib/scan.h"
#include "lib/taia.h"
#include "lib/tai.h"
#include "lib/slist.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "map.h"

typedef struct port {
  struct taia time;
} port_t;
typedef struct link {
  union {
    struct slink link;
    struct link* next;
  };
  const char* name;
  struct port port;
} link_t;

static strarray ports;
static fd_t input_fd = 0;
static stralloc input_buf;
static int verbose = 0;
static MAP_T port_map;
static link_t* port_list;

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

struct link**
find_port(const char* name) {
  struct link** it;

  slink_foreach(&port_list, it) {

    if(str_equal(name, (*it)->name))
      return it;
  }
  return NULL;
}

void
remove_port(const char* name) {
  struct link** it = find_port(name);
  struct link* l = *it;

  *it = (*it)->next;

  free((void*)l->name);
  alloc_free((void*)l);
}

void
clear_ports() {
  struct link **it, **next;

  slist_foreach_safe(&port_list, it, next) {

    struct link* l = *it;

    alloc_free(l->name);
    alloc_free(l);
  }
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
  struct link** it;
  strarray temp, newports;
  strarray_init(&temp);
  strarray_init(&newports);
  dir_entries("/dev/", &temp);

  strarray_foreach(&temp, port) {
    if(!str_start(*port, "/dev/ttyA") && !str_start(*port, "/dev/ttyUSB") && !str_start(*port, "/dev/tnt"))
      continue;

    // if(strarray_index_of(&ports, *port) == -1) {
    if(strarray_push_unique(&newports, *port)) {

      port_t p;
      taia_now(&p.time);
      MAP_INSERT(port_map, *port, str_len(*port) + 1, &p, sizeof(port_t));

      if(!find_port(*port)) {
        struct link* l = alloc(sizeof(struct link));

        l->link.next = &port_list->link;
        l->name = str_dup(*port);
        l->port = p;

        slist_insert(&port_list, &l->link);

        port_list = l;
      }

      /*  buffer_putlong(buffer_2, i);
       buffer_puts(buffer_2, ": detected new port: ");
       buffer_puts(buffer_2, *port);
       buffer_putnlflush(buffer_2); */
    }
  }

  slink_foreach(&port_list, it) {

    buffer_puts(buffer_2, (*it)->name);
    buffer_putnlflush(buffer_2);
  }
  strarray_foreach(&ports, port) {
    if(strarray_index_of(&newports, *port) == -1) {
      MAP_DELETE(port_map, *port, str_len(*port) + 1);

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
get_ports(strarray* ports) {
  int64 n = 0, r = 0;
  char** pvec;
  strarray_zero(ports);
  for(pvec = get_serial_ports(); pvec[n]; n++) {
    const char* port = pvec[n];
    size_t i = str_rchr(port, '/');
    if(port[i]) {
      i++;
      if(str_start(&port[i], "tnt") || str_start(&port[i], "ACM") ||
         str_start(&port[i], "USB") /*||   port[i + 3] == 'S'*/) {
        if(access(port, R_OK)) {
          if(errno != ENOENT && errno != ENODEV && errno != EACCES)
            errmsg_warnsys(port, 0);
          continue;
        }
        if(!strarray_contains(ports, port)) {
          // strarray_push(&ports, port);
          strarray_splice(ports, 0, 0, 1, &port);
          /*  buffer_puts(buffer_2, "detected new port: ");
           buffer_puts(buffer_2, port);
           buffer_putnlflush(buffer_2); */
          r++;
        }
      }
    }
  }
  free(pvec);
  return strarray_size(ports);
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

void
term_init(fd_t fd, struct termios* state) {
  struct termios old, raw, actual;

  /* Discard all unread input and untransmitted output. */
  tcflush(fd, TCIOFLUSH);

  /* Get current terminal settings. */
  if(tcgetattr(fd, &old))
    return;

  /* Store state, if requested. */
  if(state)
    *state = old; /* Structures can be assigned! */

  /* New terminal settings are based on current ones. */
  raw = old;

  /* Because the terminal needs to be restored to the original state,
   * you want to ignore CTRL-C (break). */
  //  raw.c_iflag |= IGNBRK;  /* do ignore break, */
  raw.c_iflag &= ~BRKINT; /* do not generate INT signal at break. */

  /* Make sure we are enabled to receive data. */
  raw.c_cflag |= CREAD;

  /* Do not generate signals from special keypresses. */
  // raw.c_lflag &= ~ISIG;

  /* Do not echo characters. */
  raw.c_lflag &= ~ECHO;

  /* Most importantly, disable "canonical" mode. */
  raw.c_lflag &= ~ICANON;

  /* In non-canonical mode, we can set whether getc() returns immediately
   * when there is no data, or whether it waits until there is data.
   * You can even set the wait timeout in tenths of a second.
   * This sets indefinite wait mode. */
  raw.c_cc[VMIN] = 1;  /* Wait until at least one character available, */
  raw.c_cc[VTIME] = 0; /* Wait indefinitely. */

  /* Set the new terminal settings. */
  if(tcsetattr(fd, TCSAFLUSH, &raw))
    return;

  /* tcsetattr() is happy even if it did not set *all* settings.
   * We need to verify. */
  if(tcgetattr(fd, &actual)) {
    const int saved_errno = errno;
    /* Try restoring the old settings! */
    tcsetattr(fd, TCSANOW, &old);
    return;
  }

  if(actual.c_iflag != raw.c_iflag || actual.c_oflag != raw.c_oflag || actual.c_cflag != raw.c_cflag ||
     actual.c_lflag != raw.c_lflag) {
    /* Try restoring the old settings! */
    tcsetattr(fd, TCSANOW, &old);
  }
}

void
term_restore(fd_t fd, const struct termios* state) {
  int result;
  /* Discard all unread input and untransmitted output. */
  do {
    result = tcflush(fd, TCIOFLUSH);
  } while(result == -1 && errno == EINTR);

  /* Restore terminal state. */
  do {
    result = tcsetattr(fd, TCSAFLUSH, state);
  } while(result == -1 && errno == EINTR);
}

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
ssize_t
term_process() {
  char x[128];
  ssize_t ret;

  if((ret = read(input_fd, x, sizeof(x))) > 0) {
    buffer_puts(buffer_1, "Read ");
    buffer_putlong(buffer_1, ret);
    buffer_puts(buffer_1, " bytes from terminal: ");
    buffer_putxlong0(buffer_1, (unsigned char)x[0], 2);
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
    //
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
        if((ret = term_process() <= 0))
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
void
usage(char* progname) {
  buffer_putm_3(buffer_1, "Usage: ", path_basename(progname), " [OPTIONS] [PORT] [BAUDRATE]\n");
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1, "  --help, -h                        show this help\n");
  buffer_puts(buffer_1, "  --version                         print program version\n");

  buffer_puts(buffer_1, "  --verbose                         increase verbosity\n");
  buffer_puts(buffer_1, "  --debug                           show verbose debug information\n");
  buffer_putnlflush(buffer_1);
}

extern buffer* optbuf;
int
main(int argc, char* argv[]) {
  const char* portname = NULL;
  unsigned int baudrate = 0;
  struct termios tio;
  int c;
  int index = 0;
  int mode = 0;
  int running = 1;
  fd_t serial_fd;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"verbose", 0, NULL, 'v'},

      {0, 0, 0, 0},
  };
  strarray portArr;
  strarray_init(&portArr);
  errmsg_iam(argv[0]);

  MAP_NEW(port_map);

#ifdef _MSC_VER
  optbuf = buffer_1;
#endif
  opterr = 0;

  for(;;) {
    c = getopt_long(argc, argv, "hv", opts, &index);
    if(c == -1 || opterr /* || argv[optind] == 0 */)
      break;
    if(c == 0)
      continue;

    switch(c) {

      case 'h': usage(argv[0]); return 0;
      case 'v': verbose++; break;

      default:
        buffer_puts(buffer_2, "WARNING: Invalid argument -");
        buffer_putc(buffer_2, isprint(c) ? c : '?');
        buffer_putm_internal(buffer_2, " '", optarg ? optarg : argv[optind], "'", NULL);
        buffer_putnlflush(buffer_2);
        usage(argv[0]);
        return 1;
    }
  }
getopt_end:
  if(optind < argc) {
    portname = argv[optind++];

    if(optind < argc && scan_uint(argv[optind], &baudrate))
      optind++;
    else
      baudrate = 38400;
  }

  io_fd(input_fd);
  io_nonblock(input_fd);

  while(running) {
    int64 i, newports;

    if(portname == NULL) {

      newports = get_ports(&portArr);
      serial_ports();
      buffer_puts(buffer_2, "num ports: ");
      buffer_putlonglong(buffer_2, newports);
      buffer_putnlflush(buffer_2);
      if(newports == 0) {
        usleep(250 * 1000);
        continue;
      }
      portname = strarray_at(&portArr, 0);
      strarray_free(&portArr);
    }

    /*    buffer_puts(buffer_2, "portname: ");
        buffer_puts(buffer_2, portname);*/
    buffer_putnlflush(buffer_2);
    serial_fd = serial_open(portname, baudrate);
    io_nonblock(serial_fd);
    io_closeonexec(serial_fd);

    if(serial_fd == -1) {
      usleep(250 * 1000);
      continue;
    }
    buffer_puts(buffer_2, "port opened: ");
    buffer_puts(buffer_2, portname);
    buffer_putnlflush(buffer_2);
    // buffer_read_fd(&serial, serial_fd);
    io_fd(serial_fd);
    io_wantread(serial_fd);
    term_init(input_fd, &tio);
    io_wantread(input_fd);

    // serial.op = &read;
    while(process_loop(serial_fd, 250) > 0)
      ;

    term_restore(input_fd, &tio);
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
