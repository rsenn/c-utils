/*
 * tcping.c
 *
 * Copyright (c) 2002-2008 Marc Kirchner <mail(at)marc(dash)kirchner(dot)de>
 *
 * tcping is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tcping is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ms++. If not, see <http://www.gnu.org/licenses/>.
 *
 * tcping does a nonblocking connect to test if a port is reachable.
 * Its exit codes are:
 *     -1 an error occured
 *     0  port is open
 *     1  port is closed
 *     2  user timeout
 */

#define VERSION 1.3.5

#include "lib/windoze.h"
#include "lib/tai.h"
#include "lib/buffer.h"
#include "lib/socket.h"
#include "lib/socket_internal.h"
#include "lib/ndelay.h"
#include "lib/unix.h"
#include "lib/errmsg.h"
#include "lib/stralloc.h"
#include "lib/dns.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/safemult.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/byte.h"
#include "lib/uint32.h"

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "map.h"

#if WINDOWS_NATIVE
#include <io.h>
#define HOSTS_FILE "C:\\Windows\\System32\\drivers\\etc\\hosts"
#else
#include <unistd.h>
#ifndef closesocket
#define closesocket close
#endif
#define HOSTS_FILE "/etc/hosts"
#endif

typedef struct {
  bool ip6;
  uint32 scope_id;
  char ip[16];
} address_t;

static MAP_T hosts_db;
static char ipbuf[IP4_FMT];

void
usage(char* prog) {
  buffer_putm_internal(
      buffer_2, "Usage: ", str_basename(prog), " [-q] [-t timeout_sec] [-u timeout_usec] <host> <port>", NULL);
  buffer_putnlflush(buffer_2);
}

size_t
scan_address(const char* x, address_t* addr) {
  size_t i;
  byte_zero(addr, sizeof(address_t));

  if((i = scan_ip4(x, addr->ip)) == 0) {
    if((i = scan_ip6if(x, addr->ip, &addr->scope_id)))
      addr->ip6 = true;
  } else {
    addr->ip6 = false;
  }
  return i;
}

size_t
fmt_address(char* x, const address_t* addr) {
  size_t n;

  if(addr->ip6)
    n = fmt_ip6if(x, addr->ip, addr->scope_id);
  else
    n = fmt_ip4(x, addr->ip);
  return n;
}

int
read_hosts(const char* file) {
  const char* p;
  size_t s, l, e, i;
  char* x;
  size_t n;
  bool ip6;
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

    i = scan_address(&p[s], &addr);

    if(i) {
      size_t hlen;

      s += i;
      s += scan_whitenskip(&p[s], l - s);

      stralloc_zero(&hostname);

      while(s < l && (hlen = scan_nonwhitenskip(&p[s], l - s))) {
        stralloc_copyb(&hostname, &p[s], hlen);

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_1, "IP: ");
        buffer_put(buffer_1, ipbuf, fmt_address(ipbuf, &addr));

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

    buffer_put(buffer_2, buf, fmt_address(buf, addr));
    buffer_putspace(buffer_2);
    buffer_puts(buffer_2, host);
    buffer_putnlflush(buffer_2);
  }
}

int
main(int argc, char* argv[]) {
  fd_t sock;
  int error = 0;
  int ret;
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

  while((c = getopt(argc, argv, "qt:u:")) != -1) {
    switch(c) {
      case 'q': verbose = 0; break;
      case 't':
        if(scan_ulonglong(optarg, &timeout_sec) == 0) {
          usage(argv[0]);
          return 108;
        }
        break;
      case 'u':
        if(scan_ulonglong(optarg, &timeout_usec) == 0)
          usage(argv[0]);
        break;
      default: usage(argv[0]); return 107;
    }
  }

  dns_random_init(seed);

  MAP_NEW(hosts_db);
  read_hosts(HOSTS_FILE);

  dump_hosts();

  stralloc_init(&ips);

  stralloc_init(&host);
  stralloc_copys(&host, argv[optind]);

  if(!lookup_hosts(&host, &addr)) {
    if(dns_ip6(&ips, &ips) == -1) {
      if(dns_ip4(&ips, &ips) == -1) {
        errmsg_warnsys("unable to find IP address for ", argv[optind], 0);
        return 111;
      } else {
        addr.ip6 = false;
      }
    } else {
      addr.ip6 = true;
    }
    byte_copy(addr.ip, addr.ip6 ? 16 : 4, ips.s);
  }

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_1, "IP address for ", argv[optind], ": ", NULL);
  buffer_put(buffer_1, ipbuf, ip6 ? ip6_fmt(ipbuf, ips.s) : ip4_fmt(ipbuf, ips.s));
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

  if((ret = addr.ip6 ? socket_connect6(sock, addr.ip, port, addr.scope_id) : socket_connect4(sock, addr.ip, port)) !=
     0) {
    if(errno != EINPROGRESS) {
#if 1 // def HAVE_SOLARIS
      /* solaris immediately returns ECONNREFUSED on local ports */
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
          errmsg_warn("error: ", argv[optind], " port ", argv[optind + 1], ": ", 0);

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
      return 2;
    }

    if(io_canread() == sock || io_canwrite() == sock) {
      if(socket_error(sock, &error) == 0) {
        /* getsockopt error */
        if(verbose) {
          errmsg_warn("error: ", argv[optind], " port ", argv[optind + 1], ": getsockopt: ", 0);
          buffer_putnlflush(buffer_2);
        }
        closesocket(sock);
        return error;
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
        return 1;
      }
    } else {
      if(verbose)
        errmsg_warn("error: select: sock not set", 0);
      return 3;
    }
  }
  /* OK, connection established */
  closesocket(sock);
  if(verbose) {
    buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " open.", NULL);
    buffer_putnlflush(buffer_1);
  }
  return 0;
}
