/*
 * tcping.c
 *
 * Copyright (c) 2002-2008 Marc Kirchner
 * <mail(at)marc(dash)kirchner(dot)de>
 *
 * tcping is free software: you can
 * redistribute it and/or modify it
 * under the terms of the GNU Lesser
 * General Public License as published
 * by the Free Software Foundation,
 * either version 3 of the License, or
 * (at your option) any later version.
 *
 * tcping is distributed in the hope
 * that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of
 * the GNU Lesser General Public License
 * along with ms++. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * tcping does a nonblocking connect to
 * test if a port is reachable. Its exit
 * codes are: -1 an error occured 0 port
 * is open 1  port is closed 2  user
 * timeout
 */

#define VERSION 1.3.5

#include "lib/windoze.h"
#include "lib/tai.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/socket.h"
#include "lib/socket_internal.h"
#include "lib/ndelay.h"
#include "lib/unix.h"
#include "lib/errmsg.h"
#include "lib/safemult.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/byte.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/io.h"
#include "lib/dns.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/fmt.h"
#include "lib/alloc.h"

#include <stdlib.h>
#include "lib/bool.h"
#include <errno.h>

#include "lib/map.h"
#include "address.h"
#include "ftplib.h"

#if WINDOWS_NATIVE
#include <io.h>
#define HOSTS_FILE                                                                                                                                                                                     \
  "C:"                                                                                                                                                                                                 \
  "\\Windows\\System32\\drivers\\etc"                                                                                                                                                                  \
  "\\hosts"
#else
#include <unistd.h>
#ifndef closesocket
#define closesocket close
#endif
#define HOSTS_FILE "/etc/hosts"
#endif

typedef union {
  struct {
    char ip[4];
    char port[2];
  };
  uint8_t u8[6];
} u8seq;

typedef enum { CONNECTED = 1, LOGGED_IN = 2, PASSIVE = 3, TRANSFERRING = 4 } ftp_state;

typedef struct {
  fd_t control_sock;
  fd_t data_sock;
  ftp_state state;
  buffer data;
} ftp_client;

static MAP_T hosts_db;
static stralloc allowed_ips;
static bool ip6, no_ip6 = false;
static char ipbuf[IP6_FMT];
static struct {
  address_t addr;
  uint16 port;
} passive;

void
usage(char* prog) {
  buffer_putm_internal(buffer_2,
                       "Usage: ",
                       str_basename(prog),
                       " [-q] [-t timeout_sec] [-u "
                       "timeout_usec] <host> <port>",
                       NULL);
  buffer_putnlflush(buffer_2);
}

int
read_hosts(const char* file) {
  const char* p;
  size_t s, l, e, i;
  char* x;
  size_t n;
  address_t addr;
  stralloc hostname;
  stralloc_init(&hostname);

  if((x = (char*)mmap_read(file, &n)) == 0)
    return -1;

  str_foreach_skip(x, p, l + 1) {
    e = n - (p - x);
    s = scan_whitenskip(p, e);

    l = byte_chr(p, e, '\n');

    if(s >= l)
      continue;
    if(p[s] == '#')
      continue;

    i = address_scan(&p[s], &addr);

    if(i) {
      size_t hlen;

      s += i;
      s += scan_whitenskip(&p[s], l - s);

      stralloc_zero(&hostname);

      while(s < l && (hlen = scan_nonwhitenskip(&p[s], l - s))) {
        stralloc_copyb(&hostname, &p[s], hlen);

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_1, "IP: ");
        buffer_put(buffer_1, ipbuf, address_fmt(ipbuf, &addr));

        buffer_puts(buffer_1, ", Hostname: ");
        buffer_putsa(buffer_1, &hostname);
        buffer_putnlflush(buffer_1);
#endif
        stralloc_nul(&hostname);

        MAP_INSERT(hosts_db, hostname.s, hostname.len + 1, &addr, sizeof(addr));

        s += hlen;
        s += scan_whitenskip(&p[s], l - s);
      }
    }
  }
  mmap_unmap(x, n);
  return 0;
}

int
lookup_hosts(stralloc* name, address_t* addr) {
  address_t* ptr;
  stralloc_nul(name);
  if((ptr = MAP_GET(hosts_db, name->s, name->len + 1))) {
    byte_copy(addr, sizeof(address_t), ptr);
    return 1;
  }
  return 0;
}

void
dump_hosts() {
  char buf[256];
  MAP_PAIR_T ptr;
  MAP_FOREACH(hosts_db, ptr) {
    const char* host = MAP_KEY(ptr);
    address_t* addr = MAP_DATA(ptr);

    buffer_put(buffer_2, buf, address_fmt(buf, addr));
    buffer_putspace(buffer_2);
    buffer_puts(buffer_2, host);
    buffer_putnlflush(buffer_2);
  }
}

void
dump_addr_port(const char* name, address_t addr, uint16 port) {
  buffer_puts(buffer_1, name);
  buffer_puts(buffer_1, " address: ");
  buffer_put(buffer_1, ipbuf, address_fmt(ipbuf, &addr));
  buffer_puts(buffer_1, " port: ");
  buffer_putulong(buffer_1, port);
  buffer_putnlflush(buffer_1);
}

void
handle_data(ftp_client* ftp, stralloc* data) {
#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_1, "Data: ");
  buffer_putsa(buffer_1, data);
  buffer_putnlflush(buffer_1);
#endif
}

ssize_t
ftp_read(int fd, void* b, size_t len, void* arg) {
  return recv(fd, b, len, 0);
}

ssize_t
ftp_write(int fd, void* b, size_t len, void* arg) {

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_1, "Write ");
  buffer_putulong(buffer_1, len);
  buffer_puts(buffer_1, " bytes: ");
  buffer_put(buffer_1, b, len);
  buffer_putnlflush(buffer_1);
#endif
  return send(fd, b, len, 0);
}
void
handle_ftp(ftp_client* ftp, stralloc* line) {
  uint32 code;
  size_t i, n, pos;
  uint32 num;
#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_1, "Protocol: ");
  buffer_putsa(buffer_1, line);
  buffer_putnlflush(buffer_1);
#endif

  scan_uint(line->s, &code);

  switch(code) {
    case 230: {

      break;
    }
    case 227: {
      u8seq bytes;
      byte_zero(&bytes, sizeof(u8seq));
      pos = stralloc_findb(line, "(", 1);
      i = 0;
      while(pos < line->len) {
        pos++;
        n = scan_uint(&line->s[pos], &num);
        bytes.u8[i++] = num;
        pos += n;
        if(line->s[pos] == ')')
          break;
      }
      address_init(&passive.addr, bytes.ip, false, -1);
      passive.port = uint16_read(bytes.port);
#ifdef DEBUG_OUTPUT
      dump_addr_port("passive", passive.addr, passive.port);
#endif
      ftp->data_sock = socket_tcp();
      io_nonblock(ftp->data_sock);
      io_fd(ftp->data_sock);
      socket_connect4(ftp->data_sock, passive.addr.ip, passive.port);
      io_wantwrite(ftp->data_sock);

      buffer_init_free(&ftp->data, (buffer_op_sys*)(void*)&ftp_read, ftp->data_sock, alloc(1024), 1024);

      break;
    }
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_1, "Code: ");
  buffer_putlong(buffer_1, code);
  buffer_putnlflush(buffer_1);
#endif
}

void
list_ftp(ftp_client* ftp) {
  fd_t r, w;
  buffer in, out;
  stralloc meld;
  stralloc_init(&meld);
  stralloc_ready(&meld, 256);
  buffer_init_free(&in, (buffer_op_sys*)(void*)&ftp_read, ftp->control_sock, alloc(1024), 1024);
  buffer_init_free(&out, (buffer_op_sys*)(void*)&ftp_write, ftp->control_sock, alloc(1024), 1024);

  for(;;) {
    io_wait();

    while((w = io_canwrite()) != -1) {
      if(w == ftp->data_sock) {
        buffer_puts(buffer_1, "data connection");
        buffer_putnlflush(buffer_1);
        io_dontwantwrite(ftp->data_sock);
        io_wantread(ftp->data_sock);

        ftp->state = PASSIVE;
        io_wantwrite(ftp->control_sock);
        break;
      }
      if(w == ftp->control_sock /*|| w == ftp->data_sock*/) {

        switch(ftp->state) {
          case CONNECTED: {
            ftplib_write_cmd_s(&out,
                               "USER "
                               "anonymous\r\nPASS "
                               "ftp@");
            ftp->state = LOGGED_IN;
            break;
          }
          case LOGGED_IN: {
            ftplib_pasv(&in, &out, 1000, &allowed_ips, &meld, 3);
            io_dontwantwrite(ftp->control_sock);
            break;
          }
          case PASSIVE: {
            ftplib_write_cmd_s(&out, "LIST /");
            ftp->state = TRANSFERRING;
            break;
          }
          case TRANSFERRING: {
            break;
          }
        }
        if(in.p == 0 && ftp->state >= TRANSFERRING)
          io_dontwantwrite(ftp->control_sock);
      }
    }
    while((r = io_canread()) != -1) {
      buffer* b;

      if(r == ftp->data_sock) {
        b = &ftp->data;
        buffer_feed(b);
        stralloc_zero(&meld);
        while(b->p < b->n && byte_chr(&b->x[b->p], b->n - b->p, '\n') < (b->n - b->p)) {
          buffer_getline_sa(b, &meld);
        }

        if(meld.len > 0) {
          handle_data(ftp, &meld);
          stralloc_zero(&meld);
        }
      }

      if(r == ftp->control_sock) {
        b = &in;
        buffer_feed(b);

        while(b->p < b->n && byte_chr(&b->x[b->p], b->n - b->p, '\n') < (b->n - b->p)) {
          int done = 0;

          buffer_getnewline_sa(b, &meld);

          stralloc_trimr(&meld, "\r\n", 2);
          stralloc_nul(&meld);

          handle_ftp(ftp, &meld);
          stralloc_zero(&meld);
        }
      }
    }
  }
}

int
main(int argc, char* argv[]) {
  fd_t sock;
  int error = 0;
  int ret = 0;
  int verbose = 1;
  int c;
  uint64 timeout_sec = 10, timeout_usec = 0;
  uint64 result;
  int port = 0;
  stralloc host, ips;
  tai6464 now, deadline, timeout;
  static char seed[128];
  address_t addr;

  errmsg_iam(argv[0]);

  if(argc < 3) {
    usage(argv[0]);
    return 109;
  }

  while((c = getopt(argc, argv, "qt:u:4")) != -1) {
    switch(c) {
      case 'q': verbose = 0; break;
      case '4': no_ip6 = true; break;
      case 't':
        if(scan_ulonglong(optarg, &timeout_sec) == 0) {
          usage(argv[0]);
          return 108;
        }
        break;
      case 'u':
        timeout_sec = 0;

        if(scan_ulonglong(optarg, &timeout_usec) == 0)
          usage(argv[0]);
        break;
      default: usage(argv[0]); return 107;
    }
  }

  dns_random_init(seed);

  MAP_NEW(hosts_db);
  read_hosts(HOSTS_FILE);

#ifdef DEBUG_OUTPUT_
  dump_hosts();
#endif

  stralloc_init(&ips);
  stralloc_ready(&ips, IP6_FMT);

  stralloc_init(&host);
  stralloc_copys(&host, argv[optind]);
  stralloc_nul(&host);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_1, "Remote host: ");
  buffer_putsa(buffer_1, &host);
  buffer_putnlflush(buffer_1);
#endif

  if(!address_scan(host.s, &addr) && !lookup_hosts(&host, &addr) && !address_lookup(&host, &addr, no_ip6)) {
    ret = 111;
    goto fail;
  }

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_1, "IP address for ", argv[optind], ": ", NULL);
  buffer_put(buffer_1, ipbuf, address_fmt(ipbuf, &addr));
  buffer_putnlflush(buffer_1);
#endif

  if(argv[optind + 1]) {
    if(scan_int(argv[optind + 1], &port) == 0) {
      usage(argv[0]);
      return 106;
    }
  } else {
    usage(argv[0]);
    return 105;
  }

  sock = addr.ip6 ? socket_tcp6() : socket_tcp4();

  io_fd(sock);

  if((ret = addr.ip6 ? socket_connect6(sock, addr.ip, port, addr.scope_id) : socket_connect4(sock, addr.ip, port)) != 0) {
    if(errno != EINPROGRESS) {
#if 1 // def HAVE_SOLARIS
      /* solaris immediately returns
       * ECONNREFUSED on local ports */
      if(errno == ECONNREFUSED) {
        if(verbose) {
          buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " closed.", NULL);
          buffer_putnlflush(buffer_1);
        }
        closesocket(sock);
        return 1;
      } else
#endif
      {
        if(verbose)
          errmsg_warnsys("error: ", argv[optind], " port ", argv[optind + 1], ": ", 0);

        return 4;
      }
    }
    errno = 0;

    io_wantread(sock);
    io_wantwrite(sock);

    taia_now(&now);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Now: ");
    buffer_puttai(buffer_2, &now.sec);
    buffer_putnlflush(buffer_2);
#endif

    taia_uint(&timeout, timeout_sec + timeout_usec / 1000000);
    umult64(timeout_usec % 1000000, 1000, &result);

    timeout.nano = result;
    taia_add(&deadline, &now, &timeout);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Deadline: ");
    buffer_puttai(buffer_2, &deadline.sec);
    buffer_putnlflush(buffer_2);
#endif

    io_waituntil(deadline);

    if((ret = io_timeouted()) == sock) {
      /* timeout */
      closesocket(sock);
      if(verbose) {
        buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " user timeout.", NULL);
        buffer_putnlflush(buffer_1);
      }
      ret = 2;
      goto fail;
    }

    if(io_canread() == sock || io_canwrite() == sock) {
      if(socket_error(sock, &error) == 0) {
        /* getsockopt error */
        if(verbose) {
          errmsg_warn("error: ", argv[optind], " port ", argv[optind + 1], ": getsockopt: ", 0);
          buffer_putnlflush(buffer_2);
        }

        closesocket(sock);
        ret = error;
        goto fail;
      }
      if(error != 0) {
        if(verbose) {
          if(error == EHOSTUNREACH)
            buffer_putm_internal(buffer_1, argv[optind], ": host is down", NULL);
          else
            buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " closed.", NULL);
          buffer_putnlflush(buffer_1);
        }
        closesocket(sock);
        ret = 1;
        goto fail;
      }
    } else {
      if(verbose) {
        buffer_puts(buffer_1, "timeout");
        buffer_putnlflush(buffer_1);
      }
      ret = 3;
      goto fail;
    }
  }
  /* OK, connection established */
  if(verbose) {
    buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " open.", NULL);
    buffer_putnlflush(buffer_1);
  }
  {
    ftp_client client;
    client.state = CONNECTED;
    client.control_sock = sock;
    client.data_sock = -1;
    list_ftp(&client);
  }
  closesocket(sock);

fail:
  MAP_DESTROY(hosts_db);
  stralloc_free(&ips);
  stralloc_free(&host);
  return ret;
}
