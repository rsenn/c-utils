
#include <stdio.h>
#include <errno.h>
#include "lib/errmsg.h"
#include "lib/socket.h"
#include "lib/io.h"
#include "lib/buffer.h"
#include "lib/open.h"
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>

static int g_running = 0;

static char* input_file = "proxy_from_dist.txt";
static char* output_file = "proxy_from_local.txt";
static char forward_addr[512];
static int proxy_port = 4545;
static int forward_port = -1;

void
usage(void) {
  buffer_puts(buffer_2,
              "proxy [-l <listening port>] -f "
              "<forward ip>:<forward port>");
  buffer_putnlflush(buffer_2);

  exit(0);
}

int
parse_args(int argc, char** argv) {
  if(argc == 5) {
    if(strcmp(argv[1], "-l") == 0) {
      proxy_port = atoi(argv[2]);
    } else if(strcmp(argv[3], "-l") == 0) {
      proxy_port = atoi(argv[4]);
    } else {
      return 0;
    }

    if(strcmp(argv[1], "-f") == 0) {
      strcpy(forward_addr, argv[2]);

      if(strstr(forward_addr, ":")) {
        char* ptr = strstr(forward_addr, ":");
        ptr[0] = 0;

        forward_port = atoi(ptr + 1);
      }
    } else if(strcmp(argv[3], "-f") == 0) {
      strcpy(forward_addr, argv[4]);

      if(strstr(forward_addr, ":")) {
        char* ptr = strstr(forward_addr, ":");
        ptr[0] = 0;

        forward_port = atoi(ptr + 1);
      }
    } else {
      return 0;
    }
  } else if(argc == 3) {
    if(strcmp(argv[1], "-f") == 0) {
      strcpy(forward_addr, argv[2]);

      if(strstr(forward_addr, ":")) {
        char* ptr = strstr(forward_addr, ":");
        ptr[0] = 0;

        forward_port = atoi(ptr + 1);
      }
    }
  } else {
    return 0;
  }
  return 1;
}

int
main(int argc, char** argv) {
  int status = 0;
  int proxy_in, proxy_client, proxy_out;
  struct sockaddr_in addr_from;
  struct sockaddr_in addr_dest;
  struct sockaddr_in client_addr;
  struct hostent* localHost;
  char* localIP;
  buffer dist_file, local_file;

  memset(forward_addr, 0, 512);

  if(parse_args(argc, argv) == 0) {
    usage();
  }

  if(forward_port < 0) {
    forward_port = proxy_port;
  }

  printf("proxy listening on 127.0.0.1:%d "
         "forward to %s:%d\n",
         proxy_port,

         forward_addr,

         forward_port);

  int opt_val = 1;
  unsigned int addrlen;

  buffer_write_fd(&dist_file, open_trunc(input_file));
  buffer_write_fd(&local_file, open_trunc(output_file));

  g_running = 1;

  /* init sockets */
  localHost = gethostbyname(forward_addr);
  localIP = inet_ntoa(*(struct in_addr*)*localHost->h_addr_list);

  addr_dest.sin_family = AF_INET;
  addr_dest.sin_port = htons(forward_port);
  addr_dest.sin_addr.s_addr = inet_addr(localIP);
  addr_from.sin_family = AF_INET;
  addr_from.sin_port = htons(proxy_port);
  addr_from.sin_addr.s_addr = htonl(INADDR_ANY);

  /* create & connect sockets */
  proxy_in = socket_tcp4b();
  proxy_out = socket_tcp4b();

  if(proxy_in < 0) {
    errmsg_warnsys("error in socket create", 0);
    return -1;
  }

  if(proxy_out < 0) {
    errmsg_warnsys("error in socket create", 0);
    return -1;
  }
  setsockopt(proxy_in, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt_val, sizeof(int));
  setsockopt(proxy_out, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt_val, sizeof(int));

  status = bind(proxy_in, (const struct sockaddr*)&addr_from, sizeof(struct sockaddr_in));

  if(status < 0) {
    errmsg_warnsys("error in bind", 0);
    return -1;
  }

  status = listen(proxy_in, 1);

  if(status < 0) {
    errmsg_warnsys("error in listen", 0);
    return -1;
  }

  addrlen = sizeof(struct sockaddr_in);
  proxy_client = accept(proxy_in, (struct sockaddr*)&client_addr, &addrlen);

  if(proxy_client < 0) {
    errmsg_warnsys("error in accept", 0);
    return -1;
  }

  status = connect(proxy_out, (struct sockaddr*)&addr_dest, sizeof(struct sockaddr));

  if(status < 0) {
    errmsg_warnsys("error in connect", 0);
    return -1;
  }

  while(g_running) {
    char c1, c2;

    int var1 = recv(proxy_client, &c1, 1, MSG_DONTWAIT);

    if(var1 > 0) {
      buffer_put(&local_file, &c1, 1);
      buffer_flush(&local_file);

      write(proxy_out, &c1, 1);
    } else if(var1 < 0) {
      if(errno != EAGAIN) {
        g_running = 0;
      }
    }

    int var2 = recv(proxy_out, &c2, 1, MSG_DONTWAIT);

    if(var2 > 0) {
      buffer_put(&dist_file, &c2, 1);
      buffer_flush(&dist_file);

      write(proxy_client, &c2, 1);
    } else if(var2 < 0) {
      if(errno != EAGAIN) {
        g_running = 0;
      }
    }

    if((var1 <= 0) && (var2 <= 0)) {
      struct timespec tts;
      tts.tv_sec = 0;
      tts.tv_nsec = 100000000;
      nanosleep(&tts, NULL);
    }
  }

  buffer_close(&dist_file);
  buffer_close(&local_file);
  close(proxy_client);
  close(proxy_in);
  close(proxy_out);

  return 0;
}
