/*
 * Tiny TCP proxy server
 *
 * Author: Krzysztof Kliś <krzysztof.klis@gmail.com>
 * Fixes and improvements: Jérôme Poulin <jeromepoulin@gmail.com>
 * IPv6 support: 04/2019 Rafael Ferrari <rafaelbf@hotmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version with the following modification:
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent modules,
 * and to copy and distribute the resulting executable under terms of your choice,
 * provided that you also meet, for each linked independent module, the terms
 * and conditions of the license of that module. An independent module is a
 * module which is not derived from or based on this library. If you modify this
 * library, you may extend this exception to your version of the library, but
 * you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "lib/uint16.h"
#include "lib/uint64.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/open.h"
#include "lib/io.h"
#include "lib/io_internal.h"
#include "lib/socket.h"
#include "lib/ndelay.h"
#include "lib/fmt.h"
#include "lib/scan.h"
#include "lib/getopt.h"
#include "lib/errmsg.h"
#include "lib/sig.h"
#include "lib/array.h"
#include "lib/slist.h"
#include <arpa/inet.h>
//#include <netdb.h>
#include <netinet/in.h>
//#include <resolv.h>
//#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/wait.h>
#ifdef USE_SYSTEMD
#include <systemd/sd-daemon.h>
#endif

#define BUF_SIZE 16384

#define READ 0
#define WRITE 1

#define SERVER_SOCKET_ERROR -1
#define SERVER_SETSOCKOPT_ERROR -2
#define SERVER_BIND_ERROR -3
#define SERVER_LISTEN_ERROR -4
#define CLIENT_SOCKET_ERROR -5
#define CLIENT_RESOLVE_ERROR -6
#define CLIENT_CONNECT_ERROR -7
#define CREATE_PIPE_ERROR -8
#define BROKEN_PIPE_ERROR -9
#define SYNTAX_ERROR -10

typedef enum { TRUE = 1, FALSE = 0 } bool;
typedef struct socketbuf_s {

  fd_t sock;
  buffer buf;

} socketbuf_t;

typedef struct connection_s {
  slink link;
  socketbuf_t client;
  socketbuf_t proxy;
  int connected;
} connection_t;

static slink* connections;

int check_ipversion(char* address);
int create_socket(int port);
void sigchld_handler(int signal);
void sigterm_handler(int signal);
void server_loop();

void handle_client(fd_t client_sock, char client_addr[16]);
ssize_t forward_data(socketbuf_t* source, socketbuf_t* destination);
void forward_data_ext(fd_t source_sock, fd_t destination_sock, char* cmd);
int create_connection();
int parse_options(int argc, char* argv[]);
void plog(int priority, const char* format, ...);
void log_data(fd_t sock, bool send, void* buffer, size_t len);

fd_t server_sock, remote_sock;
uint16 remote_port = 0, local_port = 0;
uint64 connections_processed = 0;
char *remote_host, *cmd_in, *cmd_out;
static char bind_addr[16], connect_addr[16];
static int bind_af = AF_INET, connect_af = AF_INET;
bool foreground = TRUE;
bool use_syslog = FALSE;

static char logbuf[1024];
static buffer log = BUFFER_INIT(write, STDOUT_FILENO, logbuf, sizeof(logbuf));

#define BACKLOG 20 // how many pending connections queue will hold

size_t
get_fds(array* arr) {
  slink* sl;
  array_catb(arr, &server_sock, sizeof(fd_t));
  for(sl = connections; sl; sl = sl->next) {
    connection_t* c = (connection_t*)sl;
    array_catb(arr, &c->client.sock, sizeof(fd_t));
    array_catb(arr, &c->proxy.sock, sizeof(fd_t));
  }
  return array_length(arr, sizeof(fd_t));
}

void
iarray_dump(iarray* ia) {
  size_t i, n = iarray_length(ia);

  for(i = 0; i <= n; i++) {

    buffer_puts(buffer_2, "Entry #");
    buffer_putulong(buffer_2, i);
    buffer_puts(buffer_2, ": ");
    buffer_putptr(buffer_2, iarray_get(ia, i));
    buffer_putnlflush(buffer_2);
  }
}
void
io_dump() {
  array fds;
  size_t num_fds, i;
  array_init(&fds);

  num_fds = get_fds(&fds);
  for(i = 0; i < num_fds; i++) {
    fd_t fd = *(fd_t*)array_get(&fds, sizeof(fd_t), i);

    io_entry* e = io_getentry(fd);

    buffer_puts(buffer_2, "FD #");
    buffer_putulong(buffer_2, fd);
    if(fd == server_sock)
      buffer_puts(buffer_2, " (listen) ");

    buffer_puts(buffer_2, " want:");
    if(e->wantread)
      buffer_puts(buffer_2, " read");
    if(e->wantwrite)
      buffer_puts(buffer_2, " write");
    buffer_puts(buffer_2, " can:");
    if(e->canread)
      buffer_puts(buffer_2, " read");
    if(e->canwrite)
      buffer_puts(buffer_2, " write");
    buffer_putnlflush(buffer_2);
  }
}

ssize_t
socket_send(fd_t fd, void* x, size_t n, void* ptr) {
  //  if(io_fd_canwrite(fd))
  ssize_t r;
  r = send(fd, x, n, 0);

  if(r > 0)
    log_data(fd, TRUE, x, n);
  if(r == -1 && errno == EWOULDBLOCK)
    r = 0;

  return r;
}

connection_t*
alloc_connection(fd_t client_sock) {
  connection_t* c = alloc_zero(sizeof(connection_t));
  slink_insert(&connections, &c->link);
  c->client.sock = client_sock;

  return c;
}

void
delete_connection(connection_t* c) {

  connection_t** ptr;
  slink_foreach(&connections, ptr) {
    if(*ptr == c)
      break;
  }
  *ptr = (connection_t*)c->link.next;
  alloc_free(c);
}

connection_t*
find_client(fd_t sock) {
  slink* sl;
  for(sl = connections; sl; sl = sl->next) {
    connection_t* c = (connection_t*)sl;
    if(c->client.sock == sock)
      return c;
  }
  return NULL;
}
connection_t*
find_proxy(fd_t sock) {
  slink* sl;
  for(sl = connections; sl; sl = sl->next) {
    connection_t* c = (connection_t*)sl;
    if(c->proxy.sock == sock)
      return c;
  }
  return NULL;
}
connection_t*
find_connection(fd_t sock) {
  connection_t* c;
  if((c = find_client(sock)) == NULL)
    c = find_proxy(sock);
  return c;
}
socketbuf_t*
find_socket(fd_t sock) {
  connection_t* c;
  if((c = find_client(sock)))
    return &c->client;
  if((c = find_proxy(sock)))
    return &c->proxy;
  return NULL;
}

void
check_socket(socketbuf_t* sb) {
  int wantwrite = sb->buf.p > 0;
  io_entry* e = io_getentry(sb->sock);

  if(wantwrite) {
    buffer_puts(&log, "Socket want write ");
    buffer_putulong(&log, sb->sock);
    buffer_putnlflush(&log);
    io_wantwrite(sb->sock);
   } else  {
    io_wantread(sb->sock);
  }

}

int
parse_addr(const char* opt, char ip[16]) {
  size_t n;

  if((n = scan_ip6(opt, ip)))
    return AF_INET6;
  if((n = scan_ip4(opt, ip)))
    return AF_INET;
  return -1;
}

int
bind_socket() {
  int s = bind_af == AF_INET ? socket_tcp4() : socket_tcp6();
  if(s == -1)
    return s;

  io_fd(s);

  if(bind_af == AF_INET6) {
    if(socket_bind6_reuse(s, bind_addr, local_port, 0) == 0)
      return s;
  } else {
    if(socket_bind4_reuse(s, bind_addr, local_port) == 0)
      return s;
  }

  io_close(s);
  return -1;
}

/* Program start */
int
main(int argc, char* argv[]) {
  int pid;
  errmsg_iam(argv[0]);

  local_port = parse_options(argc, argv);

  if(local_port < 0) {
    buffer_putm_internal(buffer_2,
                         "Syntax: ",
                         argv[0],
                         " [-b bind_address] -l local_port -h remote_host -p remote_port [-i \"input parser\"] [-o "
                         "\"output parser\"] [-f (stay in foreground)] [-s (use syslog)]\n",
                         0);
    buffer_flush(buffer_2);
    return local_port;
  }
  /*
    if(use_syslog)
      openlog("proxy", LOG_PID, LOG_DAEMON);
      */
  sig_block(SIGPIPE);

  if((server_sock = create_socket(local_port)) < 0) { // start server
    errmsg_warn("Cannot run server:", 0);

    return server_sock;
  }

  io_fd(server_sock);
  io_wantread(server_sock);

  // signal(SIGCHLD, sigchld_handler); // prevent ended children from becoming zombies
  // signal(SIGTERM, sigterm_handler); // handle KILL signal

  if(foreground) {
    server_loop();
  } else {
    switch(pid = fork()) {
      case 0: // deamonized child
        server_loop();
        break;
      case -1: // error
        // plog(LOG_CRIT, "Cannot daemonize: %m");
        return pid;
      default: // parent
        close(server_sock);
    }
  }

  if(use_syslog)
    closelog();

  return EXIT_SUCCESS;
}

/* Parse command line options */
int
parse_options(int argc, char* argv[]) {
  int c, index = 0;
  struct longopt opts[] = {{"local-port", 0, NULL, 'l'},
                           {"local-addr", 0, NULL, 'b'},
                           {"remote-port", 0, NULL, 'p'},
                           {"remote-addr", 0, NULL, 'h'},
                           {"input-parser", 0, NULL, 'i'},
                           {"output-parser", 0, NULL, 'O'},
                           {"foreground", 0, NULL, 'f'},
                           {"syslog", 0, NULL, 's'},
                           {"logfile", 0, NULL, 'o'},
                           {0}};

  while((c = getopt_long(argc, argv, "b:l:h:p:i:O:fso:", opts, &index)) != -1) {
    switch(c) {
      case 'l': local_port = atoi(optarg); break;
      case 'b': bind_af = parse_addr(optarg, bind_addr); break;
      case 'h': connect_af = parse_addr(optarg, connect_addr); break;
      case 'p': remote_port = atoi(optarg); break;
      case 'i': cmd_in = optarg; break;
      case 'O': cmd_out = optarg; break;
      case 'f': foreground = TRUE; break;
      case 's': use_syslog = TRUE; break;
      case 'o': log.fd = open_append(optarg); break;
    }
  }

  if(local_port && connect_af != -1 && remote_port) {
    return local_port;
  } else {
    return SYNTAX_ERROR;
  }
}

int
check_ipversion(char* address) {
  /* Check for valid IPv4 or Iv6 string. Returns AF_INET for IPv4, AF_INET6 for IPv6 */

  struct in6_addr bindaddr;

  if(inet_pton(AF_INET, address, &bindaddr) == 1) {
    return AF_INET;
  } else {
    if(inet_pton(AF_INET6, address, &bindaddr) == 1) {
      return AF_INET6;
    }
  }
  return 0;
}

/* Create server socket */
int
create_socket(int port) {
  int server_sock, optval = 1;
  int validfamily = 0;
  char portstr[12];

  server_sock = -1;

  portstr[fmt_ulong(portstr, port)] = '\0';

  if((server_sock = bind_socket()) == -1)
    return SERVER_BIND_ERROR;

  if(socket_listen(server_sock, BACKLOG) < 0)
    return SERVER_LISTEN_ERROR;

  return server_sock;
}

/* Send log message to stderr or syslog */
void
plog(int priority, const char* format, ...) {
  va_list ap;

  va_start(ap, format);

  if(use_syslog)
    vsyslog(priority, format, ap);
  else {
    int r = 0;
    const char* s;
    while((s = va_arg(ap, const char*)))
      if(buffer_puts(buffer_2, s) == -1) {
        r = -1;
        break;
      }
    va_end(ap);
    buffer_putnlflush(buffer_2);
  }

  va_end(ap);
}

/* Update systemd status with connection count */
void
update_connection_count() {
#ifdef USE_SYSTEMD
  sd_notifyf(0, "STATUS=Ready. %d connections processed.\n", connections_processed);
#endif
}

/* Handle finished child process */
void
sigchld_handler(int signal) {
  while(waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

/* Handle term signal */
void
sigterm_handler(int signal) {
  io_close(server_sock);
  exit(0);
}

/* Main server loop */
void
server_loop() {
  char client_addr[16];
  uint16 client_port;
  socklen_t addrlen = sizeof(client_addr);
  int sock;
  connection_t* c;
  socketbuf_t* sb;
  uint64 iteration = 0;
  ssize_t n;

#ifdef USE_SYSTEMD
  sd_notify(0, "READY=1\n");
#endif

  while(TRUE) {
    update_connection_count();

    slist_foreach(connections, c) {
      check_socket(&c->client);
      check_socket(&c->proxy);
    }
    io_waituntil2(1000 * 100);
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Loop #");
    buffer_putulonglong(buffer_2, iteration++);
    buffer_putnlflush(buffer_2);
#endif
#ifdef DEBUG_OUTPUT
    io_dump();
#endif
      iarray_dump(io_getfds());

    while((sock = io_canwrite()) != -1) {
      if((c = find_proxy(sock)) && !c->connected) {
        buffer_puts(&log, "Socket #");
        buffer_putulong(&log, sock);
        buffer_puts(&log, " connected!");
        buffer_putnlflush(&log);
        c->connected = 1;
        io_dontwantwrite(sock);
        io_wantread(sock);
      } else if((sb = find_socket(sock))) {
#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "Flush socket ");
        buffer_putulong(buffer_2, sock);
        buffer_puts(buffer_2, " p=");
        buffer_putulong(buffer_2, sb->buf.p);
        buffer_puts(buffer_2, " n=");
        buffer_putulong(buffer_2, sb->buf.n);
        buffer_putnlflush(buffer_2);
#endif
        if(sb->buf.p > 0) {
          buffer_flush(&sb->buf);
          io_dontwantwrite(sock);
        }
      } else {
        buffer_puts(buffer_2, "No such fd: ");
        buffer_putulong(buffer_2, sock);
        buffer_putnlflush(buffer_2);

        exit(2);
      }
    }

    while((sock = io_canread()) != -1) {
      if(sock == server_sock) {
        fd_t client_sock;

        client_sock = connect_af ? socket_accept4(server_sock, client_addr, &client_port)
                                 : socket_accept6(server_sock, client_addr, &client_port, 0);

        if(client_sock == -1) {
          errmsg_warn("Accept error: ", strerror(errno), 0);
          exit(2);
        }

        handle_client(client_sock, client_addr);

        connections_processed++;

      } else {

        if((c = find_client(sock))) {

          n = forward_data(&c->client, &c->proxy);
          if(n > 0) {
#ifdef DEBUG_OUTPUT_
            buffer_puts(buffer_2, "Client socket #");
            buffer_putulong(buffer_2, sock);
            buffer_puts(buffer_2, " forwarded ");
            buffer_putlonglong(buffer_2, n);
            buffer_puts(buffer_2, " bytes");
            buffer_putnlflush(buffer_2);
#endif
          }
        } else if((c = find_proxy(sock))) {

          n = forward_data(&c->proxy, &c->client);

          if(n > 0) {
#ifdef DEBUG_OUTPUT_
            buffer_puts(buffer_2, "Proxy socket #");
            buffer_putulong(buffer_2, sock);
            buffer_puts(buffer_2, " forwarded ");
            buffer_putlonglong(buffer_2, n);
            buffer_puts(buffer_2, " bytes");
            buffer_putnlflush(buffer_2);
#endif
          }
        } else {
          buffer_puts(buffer_2, "No such fd: ");
          buffer_putulong(buffer_2, sock);
          buffer_putnlflush(buffer_2);

          exit(2);
        }

        if(n == 0 || (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
          connection_t* c = find_connection(sock);
          buffer_puts(buffer_2, "Connection #");
          buffer_putulong(buffer_2, c->client.sock);
          buffer_puts(buffer_2, " <-> ");
          buffer_putulong(buffer_2, c->proxy.sock);
          if(n == 0) {
            buffer_puts(buffer_2, " closed");
          } else {
            buffer_puts(buffer_2, " error: ");
            buffer_puts(buffer_2, strerror(errno));
            buffer_puts(buffer_2, " errno: ");
            buffer_putlong(buffer_2, errno);
          }
          buffer_putnlflush(buffer_2);

          buffer_close(&c->client.buf);
          buffer_close(&c->proxy.buf);

          io_close(c->client.sock);
          io_close(c->proxy.sock);

          delete_connection(c);
        }
      }
    }
  }
}
/* Handle client connection */
void
handle_client(fd_t client_sock, char client_addr[16]) {

  connection_t* c = alloc_connection(client_sock);

  if((c->proxy.sock = create_connection()) < 0)
    // plog(LOG_ERR, "Cannot connect to host: %m");
    goto cleanup;

  buffer_init_free(&c->client.buf, socket_send, c->client.sock, alloc_zero(1024), 1024);
  buffer_init_free(&c->proxy.buf, socket_send, c->proxy.sock, alloc_zero(1024), 1024);

  io_fd(c->client.sock);
  io_nonblock(c->client.sock);
  io_wantread(c->client.sock);

  return;

cleanup:
  io_close(c->proxy.sock);
  io_close(c->client.sock);
}

void
log_data(fd_t sock, bool send, void* buffer, size_t len) {

  buffer_puts(&log, send ? "Sent " : "Received ");
  buffer_putulong(&log, len);
  buffer_puts(&log, send ? " bytes to #" : " bytes from #");
  buffer_putlong(&log, sock);
  buffer_puts(&log, ": '");
  buffer_put_escaped(&log, buffer, len);
  buffer_puts(&log, "'");
  buffer_putnlflush(&log);
}
/* Forward data between sockets */
ssize_t
forward_data(socketbuf_t* source, socketbuf_t* destination) {
  ssize_t n;
  size_t written = 0;
  char buffer[1024];

  while((n = recv(source->sock, buffer, sizeof(buffer), 0)) > 0) { // read data from input socket

    buffer_put(&destination->buf, buffer, n); // send data to output socket
    written += n;

    log_data(source->sock, FALSE, buffer, n);
    //   log_data(destination->sock, TRUE, buffer, n);
  }
  if(written > 0) {
    //  io_wantwrite(destination->sock);

    if(n == -1 && errno == EAGAIN)
      return written;
  }
  return n <= 0 ? n : written;
}

/* Forward data between sockets through external command */
void
forward_data_ext(int source_sock, int destination_sock, char* cmd) {
  char buffer[BUF_SIZE];
  int n, i, pipe_in[2], pipe_out[2];

  if(pipe(pipe_in) < 0 || pipe(pipe_out) < 0) // create command input and output pipes
    // plog(LOG_CRIT, "Cannot create pipe: %m");
    exit(CREATE_PIPE_ERROR);

  if(fork() == 0) {
    dup2(pipe_in[READ], STDIN_FILENO);    // replace standard input with input part of pipe_in
    dup2(pipe_out[WRITE], STDOUT_FILENO); // replace standard output with output part of pipe_out
    close(pipe_in[WRITE]);                // close unused end of pipe_in
    close(pipe_out[READ]);                // close unused end of pipe_out
    n = system(cmd);                      // execute command
    exit(n);
  } else {
    close(pipe_in[READ]);   // no need to read from input pipe here
    close(pipe_out[WRITE]); // no need to write to output pipe here

    while((n = recv(source_sock, buffer, BUF_SIZE, 0)) > 0) { // read data from input socket
      if(write(pipe_in[WRITE], buffer, n) < 0) {              // write data to input pipe of external command
        // plog(LOG_ERR, "Cannot write to pipe: %m");
        exit(BROKEN_PIPE_ERROR);
      }
      if((i = read(pipe_out[READ], buffer, BUF_SIZE)) > 0) { // read command output
        send(destination_sock, buffer, i, 0);                // send data to output socket
      }
    }

    io_close(destination_sock);

    io_close(source_sock);
  }
}

/* Create client connection */
int
create_connection() {
  struct addrinfo /*hints, */* res = NULL;
  int sock;
  int validfamily = 0;
  int ret;
  char portstr[12];
  /*
    memset(&hints, 0x00, sizeof(hints));

    hints.ai_flags = AI_NUMERICSERV;  // numeric service number, not resolve
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;*/

  portstr[fmt_ulong(portstr, remote_port)] = '\0';

  /* check for numeric IP to specify IPv6 or IPv4 socket */
  /* if((validfamily = check_ipversion(remote_host))) {
     hints.ai_family = validfamily;
     hints.ai_flags |= AI_NUMERICHOST; // remote_host is a valid numeric ip, skip resolve
   }*/

  /* Check if specified host is valid. Try to resolve address if remote_host is a hostname */
  /*  if(getaddrinfo(remote_host, portstr, &hints, &res) != 0) {
      errno = EFAULT;
      return CLIENT_RESOLVE_ERROR;
    }*/

  if((sock = connect_af == AF_INET6 ? socket_tcp6() : socket_tcp4()) < 0)
    return CLIENT_SOCKET_ERROR;

  io_fd(sock);
  io_nonblock(sock);

  if(connect_af == AF_INET6)
    ret = socket_connect6(sock, connect_addr, remote_port, 0);
  else
    ret = socket_connect4(sock, connect_addr, remote_port);

  if(ret < 0 && errno != EINPROGRESS)
    return CLIENT_CONNECT_ERROR;

  io_wantwrite(sock);
  // io_wantread(sock);

  return sock;
}
/* vim: set et ts=4 sw=4: */
