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
#include "lib/buffer.h"
#include "lib/socket_internal.h"
#include "lib/ndelay.h"
#include "lib/unix.h"
#include "lib/errmsg.h"
#include "lib/stralloc.h"
#include "lib/dns.h"
#include "lib/ip4.h"

#include <stdlib.h>
#include <errno.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#define closesocket close
#endif

void
usage(char* prog) {
  buffer_putm_internal(
      buffer_2, "Usage: ", str_basename(prog), " [-q] [-t timeout_sec] [-u timeout_usec] <host> <port>", 0);
  buffer_putnlflush(buffer_2);
  exit(-1);
}

int
main(int argc, char* argv[]) {
  int sockfd;
  struct sockaddr_in addr;
  int error = 0;
  int ret;
  socklen_t errlen;
  struct timeval timeout;
  fd_set fdrset, fdwset;
  int verbose = 1;
  int c;
  char* cptr;
  long timeout_sec = 0, timeout_usec = 0;
  int port = 0;
  stralloc host, ips;
  static char seed[128];
  char str[IP4_FMT];

  errmsg_iam(argv[0]);

  if(argc < 3) {
    usage(argv[0]);
  }

  while((c = getopt(argc, argv, "qt:u:")) != -1) {
    switch(c) {
      case 'q': verbose = 0; break;
      case 't':
        if(scan_long(optarg, &timeout_sec) == 0)
          usage(argv[0]);
        break;
      case 'u':
        if(scan_long(optarg, &timeout_usec) == 0)
          usage(argv[0]);
        break;
      default: usage(argv[0]); break;
    }
  }

  dns_random_init(seed);

  sockfd = socket_tcp4();

  memset(&addr, 0, sizeof(addr));

  stralloc_init(&ips);

  stralloc_init(&host);
  stralloc_copys(&host, argv[optind]);

  if(dns_ip4(&ips, &host) == -1) {
    errmsg_warnsys("unable to find IP address for ", argv[optind], 0);
    return 111;
  }

  buffer_putm_internal(buffer_1, "IP address for ", argv[optind], ": ", 0);
  buffer_put(buffer_1, str, ip4_fmt(str, ips.s));
  buffer_putnlflush(buffer_1);

  if(argv[optind + 1]) {
  if(scan_int(argv[optind + 1], &port) == 0)
      usage(argv[0]);
  } else {
    usage(argv[0]);
  }
  if((ret = socket_connect4(sockfd, ips.s, port)) != 0) {
    if(errno != EINPROGRESS) {
#if 1 // def HAVE_SOLARIS
      /* solaris immediately returns ECONNREFUSED on local ports */
      if(errno == ECONNREFUSED) {
        if(verbose) {
          buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " closed.", 0);
          buffer_putnlflush(buffer_1);
        }
        closesocket(sockfd);
        return (1);
      } else
#endif
      {
        if(verbose)
          errmsg_warn("error: ", argv[optind], " port ", argv[optind + 1], ": ", 0);

        return (-1);
      }
    }

    FD_ZERO(&fdrset);
    FD_SET(sockfd, &fdrset);
    fdwset = fdrset;

    timeout.tv_sec = timeout_sec + timeout_usec / 1000000;
    timeout.tv_usec = timeout_usec % 1000000;

    if((ret = select(sockfd + 1, &fdrset, &fdwset, NULL, timeout.tv_sec + timeout.tv_usec > 0 ? &timeout : NULL)) ==
       0) {
      /* timeout */
      closesocket(sockfd);
      if(verbose) {
        buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " user timeout.", 0);
        buffer_putnlflush(buffer_1);
      }
      return (2);
    }
    if(FD_ISSET(sockfd, &fdrset) || FD_ISSET(sockfd, &fdwset)) {
      errlen = sizeof(error);
      if((ret = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &errlen)) != 0) {
        /* getsockopt error */
        if(verbose) {
          errmsg_warn("error: ", argv[optind], " port ", argv[optind + 1], ": getsockopt: ", 0);
          buffer_putnlflush(buffer_2);
        }
        closesocket(sockfd);
        return (-1);
      }
      if(error != 0) {
        if(verbose) {
          buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " closed.", 0);
          buffer_putnlflush(buffer_1);
        }
        closesocket(sockfd);
        return (1);
      }
    } else {
      if(verbose)
        buffer_putsflush(buffer_2, "error: select: sockfd not set\n");
      exit(-1);
    }
  }
  /* OK, connection established */
  closesocket(sockfd);
  if(verbose) {
    buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " open.", 0);
    buffer_putnlflush(buffer_1);
  }
  return 0;
}
