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

#include "lib/buffer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>

void usage();

int
main(int argc, char* argv[]) {
  int sockfd;
  struct sockaddr_in addr;
  struct hostent* host;
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

  if(argc < 3) {
    usage(argv[0]);
  }

  while((c = getopt(argc, argv, "qt:u:")) != -1) {
    switch(c) {
      case 'q': verbose = 0; break;
      case 't':
        cptr = NULL;
        timeout_sec = strtol(optarg, &cptr, 10);
        if(cptr == optarg)
          usage(argv[0]);
        break;
      case 'u':
        cptr = NULL;
        timeout_usec = strtol(optarg, &cptr, 10);
        if(cptr == optarg)
          usage(argv[0]);
        break;
      default: usage(argv[0]); break;
    }
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&addr, 0, sizeof(addr));

  if((host = gethostbyname(argv[optind])) == NULL) {
    if(verbose)
      errmsg_warn("error: ", 0);
    exit(-1);
  }

  memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);
  addr.sin_family = host->h_addrtype; /* always AF_INET */
  if(argv[optind + 1]) {
    cptr = NULL;
    port = strtol(argv[optind + 1], &cptr, 10);
    if(cptr == argv[optind + 1])
      usage(argv[0]);
  } else {
    usage(argv[0]);
  }
  addr.sin_port = htons(port);

  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  if((ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr))) != 0) {
    if(errno != EINPROGRESS) {
#ifdef HAVE_SOLARIS
      /* solaris immediately returns ECONNREFUSED on local ports */
      if(errno == ECONNREFUSED) {
        if(verbose) {
          buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " closed.", 0);
          buffer_putnlflush(buffer_1);
        }
        close(sockfd);
        return (1);
      } else {
#endif
        if(verbose)
          errmsg_warn("error: ", argv[optind], " port ", argv[optind + 1], ": ", 0);

        return (-1);
#ifdef HAVE_SOLARIS
      }
#endif
    }

    FD_ZERO(&fdrset);
    FD_SET(sockfd, &fdrset);
    fdwset = fdrset;

    timeout.tv_sec = timeout_sec + timeout_usec / 1000000;
    timeout.tv_usec = timeout_usec % 1000000;

    if((ret = select(sockfd + 1, &fdrset, &fdwset, NULL, timeout.tv_sec + timeout.tv_usec > 0 ? &timeout : NULL)) ==
       0) {
      /* timeout */
      close(sockfd);
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
        close(sockfd);
        return (-1);
      }
      if(error != 0) {
        if(verbose) {
          buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " closed.", 0);
          buffer_putnlflush(buffer_1);
        }
        close(sockfd);
        return (1);
      }
    } else {
      if(verbose)
        buffer_putsflush(buffer_2, "error: select: sockfd not set\n");
      exit(-1);
    }
  }
  /* OK, connection established */
  close(sockfd);
  if(verbose) {
    buffer_putm_internal(buffer_1, argv[optind], " port ", argv[optind + 1], " open.", 0);
    buffer_putnlflush(buffer_1);
  }
  return 0;
}

void
usage(char* prog) {
  buffer_putm_internal(
      buffer_2, "Usage: ", str_basename(prog), " [-q] [-t timeout_sec] [-u timeout_usec] <host> <port>", 0);
  buffer_putnlflush(buffer_2);
  exit(-1);
}
