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
#include "lib/path.h"
#include "lib/uint16.h"
#include "lib/uint64.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/buffer.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/open.h"
#include "lib/env.h"
#include "lib/wait.h"
#include "lib/str.h"
#include "lib/io.h"
#include "lib/range.h"
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
#include "lib/taia.h"
#include "lib/dns.h"
#include "map.h"
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
#include <sys/stat.h>
#ifdef USE_SYSTEMD
#include <systemd/sd-daemon.h>
#endif

#define DNS_MAX_AGE 30

#define DEFAULT_BUF_SIZE 16384

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
  stralloc host;
  char addr[16];
  uint16 port;
  uint32 scope_id;
  int af;
  fd_t dump;
  int force_write : 1;
} socketbuf_t;

typedef struct dns_result_s {
  range data;
  tai6464 t;
} dns_response_t;

typedef struct connection_s {
  slink link;
  socketbuf_t client;
  socketbuf_t proxy;
  int connected;
} connection_t;

static slink* connections;
dns_response_t* dns_query(stralloc*);
void dns_print(buffer*, dns_response_t* result, size_t num_responses);
dns_response_t* dns_lookup(stralloc*);

void dump_strarray(buffer*, const strarray* a, const char* quote, const char* sep);
size_t dump_fds(array*);
void dump_io(void);

connection_t* connection_new(fd_t, char addr[16], uint16 port);
void connection_delete(connection_t*);
connection_t* connection_find(fd_t, fd_t proxy);
fd_t connection_open_log(connection_t*, const char* prefix, const char* suffix);

socketbuf_t* socket_find(fd_t);
socketbuf_t* socket_other(fd_t);
ssize_t socket_send(fd_t, void* x, size_t n, void* ptr);
int socket_connect(socketbuf_t*);
void socket_accept(fd_t, char addr[16], uint16 port);

void sockbuf_init(socketbuf_t*);
size_t sockbuf_fmt_addr(socketbuf_t*, char* dest, char sep);
void sockbuf_put_addr(buffer*, socketbuf_t* sb);
void sockbuf_close(socketbuf_t*);
void sockbuf_check(socketbuf_t*);
void sockbuf_log_data(socketbuf_t*, bool send, char* x, ssize_t len);
ssize_t sockbuf_forward_data(socketbuf_t*, socketbuf_t* destination);

fd_t server_socket(void);
fd_t server_listen(uint16);

char* search_path(const char*, const char* what, stralloc* out);

void server_finalize(void);
void server_tar_files(const char*, const stralloc* archive, strlist* files);
void server_exit(int);
void server_sigint(int);
void server_sigterm(int);
void server_loop(void);
void server_connection_count(void);

void usage(const char*);

static socketbuf_t server, remote;

fd_t server_sock;
int64 connections_processed = 0, max_length = -1;
char *remote_host, *cmd_in, *cmd_out;
bool foreground = FALSE, use_syslog = FALSE, line_buffer = FALSE, dump = FALSE;
static uint64 buf_size = DEFAULT_BUF_SIZE;
static char logbuf[1024];
static buffer log = BUFFER_INIT(write, STDOUT_FILENO, logbuf, sizeof(logbuf));
static MAP_T dns_cache;
static tai6464 ttl;
static strlist output_files;

#define BACKLOG 20 // how many pending connections queue will hold

dns_response_t*
dns_query(stralloc* h) {
  dns_response_t* res;
  stralloc dns;
  size_t reclen = 0;
  stralloc_init(&dns);
  if(dns_ip4(&dns, h) == -1) {
    stralloc_nul(h);
    errmsg_warnsys("ERROR: resolving ", h->s, ": ", NULL);
    return NULL;
  }
  if(dns.len >= 4)
    reclen = 4;

  if(reclen && dns.len >= reclen) {
    char buf[100];
    size_t i;
    res = alloc_zero(sizeof(dns_response_t));
    taia_now(&res->t);
    res->data.start = dns.s;
    res->data.end = dns.s + dns.len;
    res->data.elem_size = reclen;
    stralloc_init(&dns);

    return res;
  }
  stralloc_free(&dns);
  return NULL;
}

void
dns_print(buffer* b, dns_response_t* result, size_t num_responses) {
  char* x;
  size_t i = 0;
  char buf[128];
  range_foreach(&result->data, x) {
    if(i++ > 0)
      buffer_puts(b, ", ");
    buffer_put(b, buf, (result->data.elem_size == 16 ? fmt_ip6 : fmt_ip4)(buf, x));
    if(i == num_responses)
      break;
  }
}

dns_response_t*
dns_lookup(stralloc* h) {
  size_t i;
  bool cached = FALSE;
  dns_response_t* result;
  tai6464 now, expire, diff;

  stralloc_nul(h);

  if((result = MAP_GET(dns_cache, h->s, h->len + 1)) != NULL) {
    taia_now(&now);
    taia_add(&expire, &result->t, &ttl);

    taia_sub(&diff, &now, &result->t);

    if(!taia_less(&now, &expire)) {
      buffer_puts(buffer_2, "Cache expired ");
      buffer_putsa(buffer_2, h);
      buffer_putnlflush(buffer_2);

      MAP_DELETE(dns_cache, h->s, h->len + 1);
      result = NULL;
    } else {
      cached = TRUE;
    }
  }

  if(result == NULL)
    cached = FALSE;

  if(result == NULL && (result = dns_query(h))) {
    if(result->data.end > result->data.start && result->data.elem_size) {
      MAP_INSERT(dns_cache, h->s, h->len + 1, result, sizeof(dns_response_t));
      alloc_free(result);

      result = (dns_response_t*)MAP_GET(dns_cache, h->s, h->len + 1);
    }
  }

  if(cached)
    range_rotate(&result->data, -1);
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, cached ? "Cache hit " : "Resolved ");
  buffer_putsa(buffer_2, h);
  if(cached) {
    buffer_puts(buffer_2, " age ");
    buffer_putlong(buffer_2, diff.sec.x);
    buffer_puts(buffer_2, "s");
  }
  buffer_puts(buffer_2, " to [");
  buffer_putulong(buffer_2, range_size(&result->data));
  buffer_puts(buffer_2, "] ");
  dns_print(buffer_2, result, cached ? 1 : range_size(&result->data));
  buffer_putnlflush(buffer_2);
#endif
  return result;
}

static int
parse_addr(const char* opt, char ip[16]) {
  size_t n;
  if((n = scan_ip6(opt, ip)))
    return AF_INET6;
  if((n = scan_ip4(opt, ip)))
    return AF_INET;
  return -1;
}

static int
sockbuf_parse_addr(socketbuf_t* sb, const char* opt) {
  int ret = 0;
  stralloc_zero(&sb->host);

  if((ret = parse_addr(opt, sb->addr)) == -1) {
    stralloc_copys(&sb->host, opt);
    byte_zero(sb->addr, sizeof(sb->addr));
  }

  sb->af = ret;

  return ret;
}

static size_t
byte_numlines(char* x, size_t p, size_t* end_ptr) {
  size_t i, n = 0, y;

  for(i = 0; i < p; i++) {
    if(x[i] == '\n') {
      n++;
      y = i + 1;
    }
  }
  if(end_ptr && n > 0)
    *end_ptr = y;
  return n;
}

static bool
byte_is_binary(char* x, size_t n) {
  for(size_t i = 0; i < n; i++)
    if(x[i] < ' ' || x[i] >= 127)
      return TRUE;

  return FALSE;
}

static size_t
buffer_numlines(buffer* b, size_t* end_ptr) {
  return byte_numlines(b->x, b->p, end_ptr);
}

static bool
buffer_is_binary(buffer* b) {
  return byte_is_binary(b->x, b->p);
}

void
dump_strarray(buffer* b, const strarray* a, const char* quote, const char* sep) {
  static char quote_chars[] = {' ', '"', '\'', '`', '(', ')', 0};
  const char* s;
  size_t i, len = strarray_size(a);
  if(!quote)
    quote = "\"";
  if(!sep)
    sep = ", ";
  for(i = 0; i < len; i++) {
    if(i)
      buffer_putm_internal(b, sep, 0);
    s = strarray_at(a, i);
    if(s[str_chrs(s, quote_chars, str_len(quote_chars))])

      buffer_putm_internal(b, quote, s, quote, 0);
    else
      buffer_puts(b, s);
  }
  buffer_flush(b);
}

size_t
dump_fds(array* arr) {
  slink* sl;
  array_catb(arr, &server_sock, sizeof(fd_t));
  for(sl = connections; sl; sl = sl->next) {
    connection_t* c = (connection_t*)sl;
    array_catb(arr, &c->client.sock, sizeof(fd_t));
    array_catb(arr, &c->proxy.sock, sizeof(fd_t));
  }
  return array_length(arr, sizeof(fd_t));
}

/*void
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
*/
void
dump_io() {
  array fds;
  size_t num_fds, i;
  array_init(&fds);

  num_fds = dump_fds(&fds);
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

connection_t*
connection_new(fd_t sock, char addr[16], uint16 port) {
  connection_t* c = alloc_zero(sizeof(connection_t));
  sockbuf_init(&c->client);
  sockbuf_init(&c->proxy);

  slink_insert(&connections, &c->link);

  c->client.sock = sock;
  c->client.port = port;
  c->client.af = server.af;
  byte_copy(c->client.addr, server.af == AF_INET6 ? 16 : 4, addr);

  return c;
}

void
connection_delete(connection_t* c) {
  connection_t** ptr;
  slink_foreach(&connections, ptr) {
    if(*ptr == c)
      break;
  }
  *ptr = (connection_t*)c->link.next;
  alloc_free(c);
}

connection_t*
connection_find(fd_t client, fd_t proxy) {
  slink* sl;
  for(sl = connections; sl; sl = sl->next) {
    connection_t* c = (connection_t*)sl;
    if(c->client.sock == client || c->proxy.sock == proxy)
      return c;
  }
  return NULL;
}

fd_t
connection_open_log(connection_t* c, const char* prefix, const char* suffix) {
  char buf[1024] = {0};
  size_t i, n;
  tai6464 now;

  n = str_copy(buf, prefix);
  buf[n++] = '-';

  n += sockbuf_fmt_addr(&c->client, &buf[n], '_');
  buf[n++] = '-';
  n += sockbuf_fmt_addr(&c->proxy, &buf[n], '_');
  n += str_copy(&buf[n], suffix);
  n += '-';
  taia_now(&now);
  n += fmt_ulonglong(&buf[n], now.sec.x);

  for(i = 0; i < n; i++) {
    char c = buf[i];
    if(c == ':') //!((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9') || c == '.'))
      buf[i] = '+';
  }
  buf[i] = '\0';
  strlist_push_unique(&output_files, buf);

  return open_trunc(buf);
}

socketbuf_t*
socket_find(fd_t sock) {
  connection_t* c;
  if((c = connection_find(sock, -1)))
    return &c->client;
  if((c = connection_find(-1, sock)))
    return &c->proxy;
  return NULL;
}

socketbuf_t*
socket_other(fd_t sock) {
  connection_t* c;
  if((c = connection_find(sock, -1)))
    return &c->proxy;
  if((c = connection_find(-1, sock)))
    return &c->client;
  return NULL;
}

ssize_t
socket_send(fd_t fd, void* x, size_t n, void* ptr) {
  ssize_t r = send(fd, x, n, 0);

  if(r > 0) {
    socketbuf_t* sb = socket_find(fd);
    sockbuf_log_data(sb, TRUE, x, n);
    if(dump) {
      if(sb->dump == -1) {
        connection_t* c = connection_find(fd, fd);
        sb->dump = connection_open_log(c, c->client.sock == fd ? "input" : "output", ".txt");
      }
      write(sb->dump, x, n);
    }
  }
  if(r == -1 && errno == EWOULDBLOCK)
    r = 0;
  return r;
}

/* Create client connection */
int
socket_connect(socketbuf_t* sb) {
  int sock, ret, af;
  char *addr, buf[100];

  af = sb->af;
  addr = sb->addr;

  if(af == -1) {

    dns_response_t* res;

    if((res = dns_lookup(&sb->host)) == NULL) {
      stralloc_nul(&sb->host);
      errmsg_warnsys("ERROR: resolving ", sb->host.s, ": ", NULL);
      return -1;
    } else {
      if(range_size(&res->data) > 0)
        addr = res->data.start;
    }

    if(addr != sb->addr)
      byte_copy(sb->addr, res->data.elem_size, addr);

    af = res->data.elem_size == 16 ? AF_INET6 : AF_INET;
  }

  if((sock = af == AF_INET6 ? socket_tcp6() : socket_tcp4()) < 0)
    return CLIENT_SOCKET_ERROR;

  io_fd(sock);
  io_nonblock(sock);

  if(af == AF_INET6)
    ret = socket_connect6(sock, addr, sb->port, 0);
  else
    ret = socket_connect4(sock, addr, sb->port);

  if(ret < 0 && errno != EINPROGRESS)
    return CLIENT_CONNECT_ERROR;

  io_wantwrite(sock);

  return sock;
}

/* Handle client connection */
void
socket_accept(fd_t sock, char addr[16], uint16 port) {

  connection_t* c = connection_new(sock, addr, port);

  if((c->proxy.sock = socket_connect(&remote)) < 0)
    goto cleanup;

  byte_copy(c->proxy.addr, remote.af == AF_INET6 ? 16 : 4, remote.addr);
  c->proxy.port = remote.port;
  c->proxy.af = remote.af;

  buffer_init_free(&c->client.buf, socket_send, c->client.sock, alloc_zero(1024), 1024);
  buffer_init_free(&c->proxy.buf, socket_send, c->proxy.sock, alloc_zero(1024), 1024);

  io_fd(c->client.sock);
  io_nonblock(c->client.sock);
  io_wantread(c->client.sock);

  buffer_puts(&log, "New connection from ");
  sockbuf_put_addr(&log, &c->client);

  buffer_putnlflush(&log);
  return;

cleanup:
  io_close(c->proxy.sock);
  io_close(c->client.sock);
}

void
sockbuf_init(socketbuf_t* sb) {
  byte_zero(sb, sizeof(socketbuf_t));
  sb->sock = -1;
  sb->dump = -1;
  sb->af = AF_INET;
  stralloc_init(&sb->host);
}

size_t
sockbuf_fmt_addr(socketbuf_t* sb, char* dest, char sep) {
  size_t n = 0;

  if(sb->host.len > 0) {
    byte_copy(dest, sb->host.len, sb->host.s);
    n += sb->host.len;
  } else if(sb->af != -1) {
    n = sb->af == AF_INET6 ? fmt_ip6(dest, sb->addr) : fmt_ip4(dest, sb->addr);

    if(n >= 7 && byte_equal(dest, 6, "::ffff"))
      n = fmt_ip4(dest, &sb->addr[12]);
  }

  dest[n++] = sep ? sep : ':';

  n += fmt_ulong(&dest[n], sb->port);
  dest[n] = '\0';
  return n;
}

void
sockbuf_put_addr(buffer* b, socketbuf_t* sb) {
  char buf[100 + sb->host.len];

  buffer_put(b, buf, sockbuf_fmt_addr(sb, buf, ':'));
}

void
sockbuf_close(socketbuf_t* sb) {

  buffer_flush(&sb->buf);
  buffer_close(&sb->buf);
  io_close(sb->sock);

  if(sb->dump != -1)
    close(sb->dump);
}

void
sockbuf_check(socketbuf_t* sb) {
  int wantwrite = (line_buffer && !buffer_is_binary(&sb->buf) && !sb->force_write) ? buffer_numlines(&sb->buf, NULL) > 0
                                                                                   : sb->buf.p > 0;
  io_entry* e = io_getentry(sb->sock);

  if(wantwrite) {
    buffer_puts(&log, "Socket want write ");
    buffer_putulong(&log, sb->sock);
    buffer_putnlflush(&log);
    io_wantwrite(sb->sock);
  } else {
    io_wantread(sb->sock);
  }
}

void
sockbuf_log_data(socketbuf_t* sb, bool send, char* x, ssize_t len) {
  while(len > 0) {
    size_t maxlen, pos, end, n;
    bool escape = !line_buffer;

    end = n = line_buffer ? byte_chrs(x, len, "\r\n", 2) : len;
    while(n < len && byte_chr("\r\n", 2, x[n]) < 2) {
      n++;
      if(x[n - 1] == '\n')
        break;
    }
    if(!line_buffer)
      end = n;

    buffer_puts(&log, send ? "Sent " : "Received ");
    if(line_buffer) {
      buffer_puts(&log, end == n ? "data" : "line");
      buffer_puts(&log, send ? " to #" : " from #");
    } else {
      buffer_putulong(&log, n);
      buffer_puts(&log, send ? " bytes to #" : " bytes from #");
    }
    buffer_putlong(&log, sb->sock);
    buffer_puts(&log, " ");
    sockbuf_put_addr(&log, sb);
    buffer_puts(&log, " '");

    if(line_buffer) {
      size_t i;
      for(i = 0; i < n; i++) {
        if((unsigned)x[i] < ' ') {
          escape = TRUE;
          end = n;
          break;
        }
      }
    }
    pos = byte_rchr(log.x, log.p, '\n');
    if(pos == log.p)
      pos = 0;

    maxlen = max_length > 0 ? (max_length - (log.p - pos)) : -1;

    pos = log.p;

    (escape ? buffer_put_escaped : buffer_put)(&log, x, maxlen > 0 && maxlen < end ? maxlen : end);

    if(maxlen > 0 && maxlen < end)
      buffer_puts(&log, "<shortened> ...");

    buffer_puts(&log, "'");

    if(max_length > 0) {
      if(log.p > max_length)
        log.p = max_length;
    }
    buffer_putnlflush(&log);

    x += n;
    len -= n;
  }
}

/* Forward data between sockets */
ssize_t
sockbuf_forward_data(socketbuf_t* source, socketbuf_t* destination) {
  ssize_t n;
  size_t written = 0;
  char* buffer = alloca(buf_size);

  if((n = recv(source->sock, buffer, buf_size, 0)) > 0) { // read data from input socket

    buffer_put(&destination->buf, buffer, n); // send data to output socket
    written += n;

    sockbuf_log_data(source, FALSE, buffer, n);
  }
  if(written > 0) {
    if(n == -1 && errno == EAGAIN)
      return written;
  }
  return n <= 0 ? n : written;
}

fd_t
server_socket() {
  fd_t s = server.af == AF_INET6 ? socket_tcp6() : socket_tcp4();
  if(s == -1)
    return s;

  io_fd(s);

  if(server.af == AF_INET6) {
    if(socket_bind6_reuse(s, server.addr, server.port, 0) == 0)
      return s;
  } else {
    if(socket_bind4_reuse(s, server.addr, server.port) == 0)
      return s;
  }

  io_close(s);
  return -1;
}

/* Create server socket */
fd_t
server_listen(uint16 port) {
  fd_t sock;

  if((sock = server_socket()) == -1)
    return SERVER_BIND_ERROR;

  if(socket_listen(sock, BACKLOG) < 0)
    return SERVER_LISTEN_ERROR;

  return sock;
}

char*
search_path(const char* path, const char* what, stralloc* out) {
  const char* x;
  char* ret = 0;
  size_t pos;
  stralloc stra;

  stralloc_init(&stra);

  for(x = path; *x; x += pos) {
    pos = str_chr(x, ':');

    stralloc_copyb(&stra, x, pos);
    stralloc_catc(&stra, '/');
    stralloc_cats(&stra, what);
    stralloc_nul(&stra);

    if(path_exists(stra.s)) {
      stralloc_copy(out, &stra);
      stralloc_nul(out);
      ret = out->s;
      break;
    }

    pos++;
  }
  stralloc_free(&stra);
  return ret;
}

void
server_finalize() {
  char buf[100];
  struct stat st;
  const char *s, *b;
  stralloc base, cmd;
  static const char* const programs[] = {"bsdtar", "star", "gtar", "tar", "7z", "7za", 0};
  strlist syspath;

  size_t i, n;
  time_t t;
  buffer w;
  fd_t in, out;
  struct tm localt;
  stralloc_init(&base);
  strlist_init(&syspath, ':');
  stralloc_copys(&syspath.sa, env_get("PATH"));

  strlist_unshift(&syspath, "/opt/diet/bin-x86_64");
  time(&t);
  localtime_r(&t, &localt);
  stralloc_catb(&base, buf, strftime(buf, sizeof(buf), "%d%m%Y-%H%M%S", &localt));
  stralloc_cats(&base, ".txt");

  stralloc_inserts(&base, "input-", 0);
  stralloc_nul(&base);
  in = open_trunc(base.s);
  stralloc_replace(&base, 0, 5, "output", 6);

  stralloc_nul(&base);
  out = open_trunc(base.s);

  strlist_foreach(&output_files, s, n) {
    size_t filesize;
    ssize_t ret;
    fd_t wr = str_start(s, "in") ? in : out;

    fd_t file = open_read(s);
    if(!(fstat(file, &st) == 0 && (filesize = st.st_size)))
      filesize = 0;

    t = st.st_ctime;

    buffer_init_free(&w, (buffer_op_proto*)&write, wr, alloc(1024), 1024);

    buffer_puts(&w, "\n-- File '");
    buffer_put(&w, s, n);
    buffer_puts(&w, "' -- ");
    localtime_r(&t, &localt);
    buffer_put(&w, buf, strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &localt));
    // buffer_putulong(&w, created);
    buffer_putnlflush(&w);

    buffer_free(&w);

    ret = io_sendfile(wr, file, 0, filesize);

    buffer_puts(buffer_2, "Output file: ");
    buffer_put(buffer_2, s, n);

    buffer_puts(buffer_2, " ret =");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);
  }

  stralloc_replace(&base, 0, 6, "all", 3);
  stralloc_replace(&base, base.len - 4, 4, ".tar", 4);
  stralloc_nul(&base);

  if(stat(base.s, &st) != -1)
    unlink(base.s);

  stralloc_init(&cmd);
  stralloc_nul(&syspath.sa);

  s = NULL;
  for(i = 0; programs[i]; i++) {
    if((s = search_path(syspath.sa.s, programs[i], &cmd)))
      break;
  }

  stralloc_free(&base);

  strlist_foreach_s(&output_files, s) { unlink(s); }
}

void
server_tar_files(const char* cmd, const stralloc* archive, strlist* files) {
  strarray argv;
  const char* base;
  char** v;
  int32 pid, child_pid;
  int status;
  fd_t out = STDOUT_FILENO;
  base = path_basename(cmd);

  strarray_from_argv(strlist_count(files), (const char* const*)strlist_to_argv(files), &argv);
  if(str_start(base, "7z")) {

    strarray_unshiftm(&argv, "a", "-ssc", archive->s, 0);
  } else {
    if(str_equal(base, "star"))
      strarray_unshift(&argv, "artype=pax");
    else if(str_start(base, "g") || str_start(base, "bsd"))
      strarray_unshiftm(&argv, "--format=pax", 0);
    else
      strarray_unshiftm(&argv, "-H", "pax", 0);

    if(str_equal(base, "bsdtar"))
      strarray_unshiftm(&argv, "-f", archive->s, 0);
    else
      out = open_trunc(archive->s);
    strarray_unshiftm(&argv, "-c", 0);
  }
  strarray_unshift(&argv, base);
  buffer_puts(buffer_1, "Exec: ");
  dump_strarray(buffer_1, &argv, "'", " ");
  buffer_putnlflush(buffer_1);
  v = strarray_to_argv(&argv);
  if((child_pid = vfork()) == 0) {
    char* const env[] = {"PATH=/bin:/usr/bin", 0};
    close(2);
    dup2(1, 2);

    if(out != STDOUT_FILENO) {
      close(1);
      dup2(out, 1);
    }
    execve(cmd, v, env);
    exit(127);
  }
  close(out);

  pid = waitpid(child_pid, &status, 0);
  if(pid != -1) {

    buffer_puts(buffer_2, cmd);
    dump_strarray(buffer_2, &argv, "'", " ");
    buffer_puts(buffer_2, " (");
    buffer_putlong(buffer_2, pid);
    buffer_puts(buffer_2, ") exit code = ");
    buffer_putlong(buffer_2, WEXITSTATUS(status));
    buffer_putnlflush(buffer_2);
  }
}
void
server_exit(int code) {
  server_finalize();
  buffer_putnlflush(buffer_2);
  exit(code);
}

void
server_sigint(int sig) {

  buffer_puts(&log, "SIGINT received");
  buffer_putnlflush(&log);
  buffer_close(&log);
  server_exit(1);
}
void
server_sigterm(int sig) {
  buffer_puts(&log, "SIGTERM received");
  buffer_putnlflush(&log);
  buffer_close(&log);
  server_exit(0);
}

/* Main server loop */
void
server_loop() {

  int sock;
  connection_t* c;
  socketbuf_t* sb;
  uint64 iteration = 0;
  ssize_t n;

#ifdef USE_SYSTEMD
  sd_notify(0, "READY=1\n");
#endif

  while(TRUE) {
    server_connection_count();

    slist_foreach(connections, c) {
      sockbuf_check(&c->client);
      sockbuf_check(&c->proxy);
    }
    io_waituntil2(1000 * 100);
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Loop #");
    buffer_putulonglong(buffer_2, iteration++);
    buffer_putnlflush(buffer_2);
#endif
#ifdef DEBUG_OUTPUT_
    dump_io();
#endif
#ifdef DEBUG_OUTPUT_
    iarray_dump(io_getfds());
#endif
    while((sock = io_canwrite()) != -1) {
      if((c = connection_find(-1, sock)) && !c->connected) {
        buffer_puts(&log, "Socket #");
        buffer_putulong(&log, sock);
        buffer_puts(&log, " connected!");
        buffer_putnlflush(&log);
        c->connected = 1;

        c->proxy.dump = -1;

        remote.af == AF_INET6 ? socket_local6(sock, c->proxy.addr, &c->proxy.port, &c->proxy.scope_id)
                              : socket_local4(sock, c->proxy.addr, &c->proxy.port);
        io_dontwantwrite(sock);
        io_wantread(sock);
      } else if((sb = socket_find(sock))) {
#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "Flush socket ");
        buffer_putulong(buffer_2, sock);
        buffer_puts(buffer_2, " p=");
        buffer_putulong(buffer_2, sb->buf.p);
        buffer_puts(buffer_2, " n=");
        buffer_putulong(buffer_2, sb->buf.n);
        buffer_putnlflush(buffer_2);
#endif

        while(sb->buf.p > 0) {
          /*
                    if(line_buffer && !buffer_is_binary(&sb->buf) && !sb->force_write) {
                      size_t num_lines, end_pos;
                      socketbuf_t* other;
                      if((num_lines = buffer_numlines(&sb->buf, &end_pos)) > 0) {
                        ssize_t r = socket_send(sb->sock, sb->buf.x, end_pos, 0);
                        if(r > 0) {
                          buffer* b = &sb->buf;
                          if(r < b->p) {
                            byte_copyr(b->x, b->p - r, &b->x[r]);
                            b->p -= r;

                            if(b->p) {
                              other = socket_other(sb->sock);
                              r = io_tryread(other->sock, &b->x[b->p], b->a - b->p);
                              if(r > 0) {
                                b->p += r;
                                continue;
                              }
                            }
                          }
                        }
                      }
                    } else*/
          {
            buffer_flush(&sb->buf);
            sb->force_write = 0;
            break;
          }
          if(sb->buf.p)
            sb->force_write = 1;
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
        fd_t sock;
        char addr[16];
        uint16 port;
        socklen_t addrlen = sizeof(addr);
        sock = server.af ? socket_accept4(server_sock, addr, &port) : socket_accept6(server_sock, addr, &port, 0);
        if(sock == -1) {
          errmsg_warn("Accept error: ", strerror(errno), 0);
          exit(2);
        }
        socket_accept(sock, addr, port);
        connections_processed++;

      } else {

        if((c = connection_find(sock, -1))) {

          n = sockbuf_forward_data(&c->client, &c->proxy);
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
        } else if((c = connection_find(-1, sock))) {

          n = sockbuf_forward_data(&c->proxy, &c->client);

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
          connection_t* c = connection_find(sock, sock);

          buffer_puts(&log, "Connection #");
          buffer_putulong(&log, c->client.sock);

          buffer_puts(&log, " ");
          sockbuf_put_addr(&log, &c->client);

          buffer_puts(&log, " <-> #");
          buffer_putulong(&log, c->proxy.sock);
          buffer_puts(&log, " ");
          sockbuf_put_addr(&log, &c->proxy);
          if(n == 0) {
            buffer_puts(&log, " closed");
          } else {
            buffer_puts(&log, " error: ");
            buffer_puts(&log, strerror(errno));
            buffer_puts(&log, " errno: ");
            buffer_putlong(&log, errno);
          }
          buffer_putnlflush(&log);

          sockbuf_close(&c->client);
          sockbuf_close(&c->proxy);

          connection_delete(c);
        }
      }
    }
  }
}

/* Update systemd status with connection count */
void
server_connection_count() {
#ifdef USE_SYSTEMD
  sd_notifyf(0, "STATUS=Ready. %d connections processed.\n", connections_processed);
#endif
}

void
usage(const char* argv0) {
  buffer_putm_internal(buffer_2,
                       "Syntax: ",
                       argv0,
                       " [...OPTIONS]\n"
                       "  -b ADDR     local address\n"
                       "  -l PORT     local port\n"
                       "  -r ADDR     remote address\n"
                       "  -p PORT     remote port\n"
                       "  -f          stay in foreground\n"
                       "  -s          use syslog\n"
                       "  -o          output logfile\n"
                       "  -a          append logfile\n"
                       "  -m LENGTH   max line length\n"
                       "  -L          line buffered\n",
                       "  -T          dns TTL\n",
                       0);
  buffer_flush(buffer_2);
}

/* Program start */
int
main(int argc, char* argv[]) {
  int pid;
  int c, index = 0;
  const char* s;
  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"local-port", 0, NULL, 'l'},
                           {"local-addr", 0, NULL, 'b'},
                           {"remote-port", 0, NULL, 'p'},
                           {"remote-addr", 0, NULL, 'r'},
                           {"input-parser", 0, NULL, 'i'},
                           {"output-parser", 0, NULL, 'O'},
                           {"foreground", 0, NULL, 'f'},
                           {"syslog", 0, NULL, 's'},
                           {"logfile", 0, NULL, 'o'},
                           {"append", 0, NULL, 'a'},
                           {"max-length", 0, NULL, 'm'},
                           {"line-buffer", 0, NULL, 'L'},
                           {"dump", 0, NULL, 'd'},
                           {"ttl", 0, NULL, 'T'},
                           {0}};

  errmsg_iam(argv[0]);

  MAP_NEW(dns_cache);

  sockbuf_init(&server);
  sockbuf_init(&remote);

  if((s = env_get("COLUMNS")))
    scan_longlong(s, &max_length);

  taia_uint(&ttl, DNS_MAX_AGE);

  while((c = getopt_long(argc, argv, "hb:l:r:p:i:O:fso:a:m:LdB:T:", opts, &index)) != -1) {
    switch(c) {
      case 'h':
        usage(argv[0]);
        return 0;
        break;
      case 'T':
        scan_ulonglong(optarg, &ttl.sec.x);
        ;
        break;
      case 'l': scan_ushort(optarg, &server.port); break;
      case 'b': sockbuf_parse_addr(&server, optarg); break;
      case 'r': sockbuf_parse_addr(&remote, optarg); break;
      case 'p': scan_ushort(optarg, &remote.port); break;
      case 'i': cmd_in = optarg; break;
      case 'O': cmd_out = optarg; break;
      case 'f': foreground = TRUE; break;
      case 's': use_syslog = TRUE; break;
      case 'm': scan_longlong(optarg, &max_length); break;
      case 'B': scan_ulonglong(optarg, &buf_size); break;
      case 'L': line_buffer = TRUE; break;
      case 'd': dump = TRUE; break;
      case 'a': log.fd = open_append(optarg); break;
      case 'o': log.fd = open_trunc(optarg); break;
    }
  }

  if(!(server.port && remote.port))
    return SYNTAX_ERROR;

  if(server.port < 0) {
    usage(argv[0]);
    return server.port;
  }

  if(use_syslog)
    openlog("proxy", LOG_PID, LOG_DAEMON);

  sig_block(SIGPIPE);
  //    sig_block(SIGINT);
  sig_push(SIGTERM, &server_sigterm);
  sig_push(SIGINT, &server_sigint);

  if((server_sock = server_listen(server.port)) < 0) { // start server
    errmsg_warn("Cannot run server:", 0);

    return server_sock;
  }

  buffer_puts(buffer_2, "Listening to ");
  sockbuf_put_addr(buffer_2, &server);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "Connecting to ");
  sockbuf_put_addr(buffer_2, &remote);
  buffer_putnlflush(buffer_2);

  io_fd(server_sock);
  io_wantread(server_sock);

  if(!foreground)
    daemon(1, 0);

  server_loop();

  if(use_syslog)
    closelog();

  return EXIT_SUCCESS;
}
