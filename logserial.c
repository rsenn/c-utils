#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
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
#include "lib/sig.h"
#include "lib/charbuf.h"
#include "lib/mmap.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <setjmp.h>
#include "map.h"

typedef struct port {
  struct taia time;
} port_t;
typedef struct link {
  union {
    struct slink link;
    struct link* next;
  };
  char* name;
  struct taia time;
} link_t;

static strarray ports;
// static fd_t term_buf.fd = 0;
static buffer term_buf;
static stralloc serial_buf;
static int verbose, rawmode;
static MAP_T port_map;
static link_t* port_list;
static jmp_buf context;
static const char* send_file = 0;
static buffer send_buf;
volatile int running;
fd_t serial_fd;

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

  for(it = &port_list; *it; it = &(*it)->next) {

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

  free((char*)l->name);
  alloc_free((void*)l);
}

void
clear_ports() {
  struct link **it, **next;

  slink_foreach_safe(port_list, it, next) {

    struct link* l = *it;

    free((char*)l->name);
    alloc_free(l);
  }
}

/**
 * @brief      Gets the ports.
 *
 * @return     The ports.
 */
int64
get_ports(strarray* ports) {
  int64 j, n = 0, r = 0;
  char** pvec;
  strarray temp;
  strarray_init(&temp);
  strarray_zero(ports);
  dir_entries("/dev/", &temp);
  n = strarray_size(&temp);
  pvec = strarray_begin(&temp);
  for(j = 0; j < n; j++) {
    const char* port = pvec[j];
    size_t i = str_rchr(port, '/');
    if(port[i]) {
      i++;
      if(/*str_start(&port[i], "tnt") ||*/ str_start(&port[i], "ttyACM") ||
         str_start(&port[i], "ttyUSB") /*||   port[i + 3] == 'S'*/) {
        if(access(port, R_OK)) {
          if(errno != ENOENT && errno != ENODEV && errno != EACCES)
            errmsg_warnsys(port, 0);
          continue;
        }
        if(!strarray_contains(ports, port)) {
          strarray_push(ports, port);
          //          strarray_splice(ports, 0, 0, 1, &port);
          /*       buffer_puts(buffer_2, "detected new port: ");
                 buffer_puts(buffer_2, port);
                 buffer_putnlflush(buffer_2);*/
          r++;
        }
      }
    }
  }
  strarray_free(&temp);
  return strarray_size(ports);
}

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
int64
serial_ports(strarray* ports) {
  static int i;
  char** port;
  struct link **it, *entry;
  strarray temp;
  strarray_init(&temp);

  get_ports(&temp);

  // dir_entries("/dev/", &temp);

  strarray_foreach(&temp, port) {
    struct stat st;
    struct taia t;

    if(!str_start(*port, "/dev/ttyA") && !str_start(*port, "/dev/ttyUSB") && !str_start(*port, "/dev/tnt"))
      continue;

    byte_zero(&st, sizeof(st));

    if(stat(*port, &st) != -1) {

      taia_uint(&t, st.st_ctime);
#if _POSIX_C_SOURCE >= 200809L
      t.nano = st.st_ctim.tv_nsec;
#endif
    } else {
      taia_now(&t);
    }

    // if(strarray_index_of(&ports, *port) == -1) {

    if(!find_port(*port)) {
      port_t p;
      taia_uint(&p.time, 0);
      taia_add(&p.time, &p.time, &t);
      taia_now(&p.time);

      MAP_INSERT(port_map, *port, str_len(*port) + 1, &p, sizeof(port_t));

      struct link* l = alloc_zero(sizeof(struct link));

      l->next = port_list;
      l->name = str_dup(*port);

      byte_copy(&l->time, sizeof(l->time), &p.time);

      slist_insert(&port_list, &l->link);

      port_list = l;

      buffer_putlong(buffer_2, i);
      buffer_puts(buffer_2, ": detected new port: ");
      buffer_puts(buffer_2, *port);
      buffer_puts(buffer_2, " ");
      buffer_puttai(buffer_2, &l->time.sec);

      buffer_putnlflush(buffer_2);
    }
  }
  for(entry = port_list; entry; entry = entry->next) {

    char* port = entry->name;

    if(strarray_index_of(&temp, port) == -1) {
      MAP_DELETE(port_map, port, str_len(port) + 1);

      buffer_putlong(buffer_2, i);
      buffer_puts(buffer_2, ": disappeared port: ");
      buffer_puts(buffer_2, port);
      buffer_putnlflush(buffer_2);

      remove_port(port);
    }
  }

  strarray_free(&temp);
  strarray_zero(ports);

  slink_foreach(&port_list, it) { strarray_push(ports, (*it)->name); }

  i++;
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

  if(rawmode) {

    raw.c_lflag &= ~(ICANON | IEXTEN | ISIG);
    raw.c_lflag |= ECHONL | ISIG;

    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | IGNCR);
    raw.c_iflag |= INLCR;

    raw.c_cflag &= ~(CSIZE | PARENB);

    raw.c_cflag |= CS8;

    raw.c_oflag &= ~(OPOST);
    raw.c_oflag |= ONLCR;

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
  }

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
 * @param      ch    { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
/*ssize_t
term_process(char* ch) {

  ssize_t ret = charbuf_getc(&term_buf, ch);

  return ret;
}
*/
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
  size_t bytes = 0;
  if((ret = read(serial_fd, x, 1)) > 0) {

    /*  if(x[0] == '\r' || x[0] == '\n')
        buffer_puts(buffer_1, "\r\n");
      else*/
    buffer_put(buffer_1, x, ret);

    buffer_flush(buffer_1);

    /*    stralloc_catb(&serial_buf, x, ret);
        if(stralloc_contains(&serial_buf, "\n")) {
          buffer_putsa(buffer_1, &serial_buf);
          buffer_flush(buffer_1);
          stralloc_zero(&serial_buf);
        }*/
  } else if(ret == 0) {
    errmsg_warn("serial closed", 0);
    io_dontwantread(serial_fd);
    io_close(serial_fd);

    // longjmp(context, 1);

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
#define max(a, b) ((a) > (b) ? (a) : (b))
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
  char ch;
  ssize_t ret;
  int queue = 0, sent = 0;
  tai6464 t, deadline, msecs, diff;
  taia_now(&t);
  taia_uint(&msecs, timeout / 1000);
  msecs.nano = (timeout % 1000) * 1000000;

  buffer_free(&send_buf);
  buffer_free(&term_buf);
  buffer_write_fd(&send_buf, serial_fd);
  buffer_read_fd(&term_buf, STDIN_FILENO);

  io_fd(serial_fd);
  io_nonblock(serial_fd);
  io_wantread(serial_fd);

  io_fd(STDIN_FILENO);
  io_nonblock(STDIN_FILENO);
  io_wantread(STDIN_FILENO);

  if(send_file) {
    size_t n;
    char* x;

    if((x = mmap_read(send_file, &n))) {
      buffer_init_free(&send_buf, (buffer_op_proto*)&write, serial_fd, alloc_zero(max(n, 1024)), max(n, 1024));
      buffer_put(&send_buf, x, n);
      mmap_unmap(x, n);
    }

    io_wantwrite(serial_fd);
    queue = 1;
  } else {
    buffer_write_fd(&send_buf, serial_fd);
  }

  for(;;) {
    taia_now(&t);
    taia_add(&deadline, &t, &msecs);

    taia_sub(&diff, &deadline, &t);
    int64 wait_msecs = taia_approx(&diff) * 1000;
    if(wait_msecs < 0)
      wait_msecs = 0;
    fd_t read_fd, write_fd;
    //
    /*
       io_wantread(STDIN_FILENO);*/

    io_wantread(serial_fd);
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "wait until ");
    buffer_putlonglong(buffer_2, wait_msecs);
    buffer_putnlflush(buffer_2);

    io_dump(buffer_2);
#endif
    if((ret = io_waituntil2(wait_msecs)) < 0) {
      errmsg_warnsys("wait error: ", 0);
      break;
    } else {
      ret = 1;
    }
    while((write_fd = io_canwrite()) != -1) {
      if(write_fd == serial_fd) {
        ssize_t bytes;
        /* if(queue) */ {
          bytes = send_buf.p;
          buffer_flush(&send_buf);

          if(!sent) {
#ifdef DEBUG_OUTPUT
            buffer_puts(buffer_2, "Sent ");
            buffer_putulong(buffer_2, bytes);
            buffer_putsflush(buffer_2, " bytes\n");
#endif
            sent = 1;
          }

          /* if(send_buf.p == 0) */ {
            queue = 0;
            io_dontwantwrite(serial_fd);
          }
        }
      }
    }
    while((read_fd = io_canread()) != -1) {
      if(read_fd == serial_fd) {
        if((ret = process_serial(serial_fd) <= 0)) {
          if(!(ret == -1 && errno == EAGAIN))
            goto end;
        }
      }
      if(read_fd == term_buf.fd) {
        size_t bytes = 0;
        while((ret = read(STDIN_FILENO, &ch, 1)) > 0) {
#ifdef DEBUG_OUTPUT
          buffer_puts(buffer_2, "Read char '");
          buffer_putulong(buffer_2, (unsigned long)(unsigned char)ch);
          buffer_putsflush(buffer_2, "'\n");
#endif
          if(ch == '\r' || ch == '\n')
            buffer_puts(&send_buf, "\r\n");
          else
            buffer_putc(&send_buf, ch);
          bytes++;
        }
        if(bytes && queue == 0) {
          queue = 1;
          io_wantwrite(serial_fd);

#ifdef DEBUG_OUTPUT
          buffer_puts(buffer_2, "Queued ");
          buffer_putulong(buffer_2, bytes);
          buffer_puts(buffer_2, " bytes to serial port");
          buffer_putnlflush(buffer_2);
#endif
        }
      }
    }
    if(stralloc_length(&serial_buf)) {
      buffer_putsa(buffer_1, &serial_buf);
      buffer_flush(buffer_1);
      stralloc_zero(&serial_buf);
    }
    /* if(wait_msecs <= 0)
       break;*/
  }
end:
  return ret;
}

void
usage(char* progname) {
  buffer_putm_internal(buffer_1, "Usage: ", path_basename(progname), " [OPTIONS] [PORT] [BAUDRATE]\n", 0);
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1, "  --help, -h                        show this help\n");
  buffer_puts(buffer_1, "  --version                         print program version\n");

  buffer_puts(buffer_1, "  --verbose                         increase verbosity\n");
  buffer_puts(buffer_1, "  --send, -i FILE                   send file\n");
  buffer_puts(buffer_1, "  --debug         +                  show verbose debug information\n");
  buffer_putnlflush(buffer_1);
}

void
signal_handler(int sig) {
  buffer_puts(buffer_2, "Signal ");
  buffer_putulong(buffer_2, sig);
  buffer_putnlflush(buffer_2);

  if(sig == SIGINT) {
    longjmp(context, 1);
  }
  running = 0;

  if(sig == SIGTERM) {
    exit(2);
  }
}
extern buffer* optbuf;
int
main(int argc, char* argv[]) {
  char* portname = NULL;
  unsigned int baudrate = 0;
  struct termios tio;
  int c;
  int index = 0;

  ssize_t ret;
  int mode = 0;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"verbose", 0, NULL, 'v'},
      {"send", 1, NULL, 'i'},
      {"raw", 0, NULL, 'r'},

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
    c = getopt_long(argc, argv, "hvri:", opts, &index);
    if(c == -1 || opterr /* || argv[optind] == 0 */)
      break;
    if(c == 0)
      continue;

    switch(c) {

      case 'h': usage(argv[0]); return 0;
      case 'v': verbose++; break;
      case 'i': send_file = optarg; break;
      case 'r': rawmode = 1; break;

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

  setjmp(context);

  term_init(term_buf.fd, &tio);

  io_fd(term_buf.fd);
  io_nonblock(term_buf.fd);
  io_wantread(term_buf.fd);
  sig_blocknone();

  //  sig_catch(SIGINT, signal_handler);
  sig_catch(SIGTERM, signal_handler);
  sig_catch(SIGSTOP, signal_handler);
  running = 1;

  while(running) {
    int64 i, newports;
    char* port;

    if(portname) {
      port = portname;
    } else {

      newports = get_ports(&portArr);
      serial_ports(&portArr);
      /*buffer_puts(buffer_2, "num ports: ");
      buffer_putlonglong(buffer_2, newports);
      buffer_putnlflush(buffer_2);*/
      if(newports == 0) {
        usleep(250 * 1000);
        continue;
      }
      portname = strarray_at(&portArr, 0);
      //  strarray_free(&portArr);
    }

    buffer_puts(buffer_2, "portname: ");
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

    // serial.op = &read;
    if((ret = process_loop(serial_fd, 30000)) > 0)
      ;

    io_dontwantread(serial_fd);
    io_close(serial_fd);
    remove_port(portname);

    int64 idx = strarray_index_of(&ports, portname);
    if(idx != -1) {

      /*buffer_puts(buffer_2, "removed port: ");
      buffer_puts(buffer_2, strarray_at(&ports, idx));
      buffer_putnlflush(buffer_2); */
      strarray_splice(&ports, idx, 1, 0, NULL);
    }

    if(ret == 0)
      continue;
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

  term_restore(term_buf.fd, &tio);

  strarray_free(&portArr);
  stralloc_free(&serial_buf);
}
