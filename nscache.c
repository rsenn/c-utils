#include "lib/env.h"
#include "lib/scan.h"
#include "lib/errmsg.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/uint16.h"
#include "lib/uint64.h"
#include "lib/socket.h"
#include "lib/dns.h"
#include "lib/taia.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/iopause.h"
#include "lib/alloc.h"
#include "lib/ndelay.h"
#include "lib/str.h"

#include "lib/bool.h"
#include <unistd.h>
#include <sys/stat.h>

#include "response.h"
#include "roots.h"
#include "query.h"
#include "cache.h"
#include "log.h"

#define FATAL "nscache: fatal: "
#define WARN "nscache: warning: "

static char fn[3 + IP6_FMT];

static char sendaddr[16], bindaddr[16];
static uint32 bindscope;
static char buf[1024];
uint64 numqueries = 0;

static int udp53, tcp53;

#define MAXUDP 200
#define MAXTCP 20

iopause_fd io[3 + MAXUDP + MAXTCP];
iopause_fd *udp53io, *tcp53io;

struct tcpclient {
  uint64 active; /* query number or 1, if
                    active; otherwise 0 */
  iopause_fd* io;
  char* buf; /* 0, or dynamically
                allocated of length len */
  struct taia start;
  struct taia timeout;
  struct query q;
  uint32 scope_id;
  int tcp; /* open TCP socket, if active
            */
  int state;
  unsigned int len;
  unsigned int pos;
  uint16 port; /* send response to this
                  port */
  char id[2];
  char ip[16]; /* send response to this
                  address */
};

struct udpclient {
  uint64 active; /* query number, if
                    active; otherwise 0 */
  iopause_fd* io;
  struct taia start;
  struct query q;
  uint32 scope_id;
  uint16 port;
  char id[2];
  char ip[16];
};

static struct udpclient u[MAXUDP];
int uactive = 0;

static struct tcpclient t[MAXTCP];
int tactive = 0;

union {
  char u8[128];
  uint32 u32[32];
} seed;

int
nscache_okclient(char ip[16], bool ip6) {
  struct stat st;
  int i;

  if(ip6 && ip6_isv4mapped(ip)) {
    if(nscache_okclient(&ip[12], false))
      return 1;
  }

  fn[0] = 'i';
  fn[1] = 'p';
  fn[2] = '/';
  fn[3 + (ip6 ? fmt_ip6 : fmt_ip4)(fn + 3, ip)] = 0;

  for(;;) {
    if(stat(fn, &st) == 0)
      return 1;
    /* treat temporary error as
     * rejection */
    i = str_rchr(fn, '.');
    if(!fn[i])
      return 0;
    fn[i] = 0;
  }
}

void
nscache_droproot(const char* fatal) {
  const char* x;
  unsigned long id;

  if((x = env_get("ROOT"))) {
    if(chdir(x) == -1)
      diesys(111, fatal, "unable to chdir to ", x, ": ");
    if(getuid() == 0 && chroot(".") == -1)
      carpsys(WARN, "unable to chroot to ", x, ": ");
  } else {
    msg(WARN, "$ROOT not set");
  }

  if((x = env_get("GID"))) {
    scan_ulong(x, &id);
    if(setgid((int)id) == -1)
      diesys(111, fatal, "unable to setgid: ");
  } else {
    msg(WARN, "$GID not set");
  }

  if((x = env_get("UID"))) {
    scan_ulong(x, &id);
    if(setuid((int)id) == -1)
      diesys(111, fatal, "unable to setuid: ");
  } else {
    msg(WARN, "$UID not set");
  }
}

static int
packetquery(char* buf, unsigned int len, char** q, char qtype[2], char qclass[2], char id[2]) {
  unsigned int pos;
  char header[12];

  errno = EPROTO;
  pos = dns_packet_copy(buf, len, 0, header, 12);
  if(!pos)
    return 0;
  if(header[2] & 128)
    return 0; /* must not respond to
                 responses */
  if(!(header[2] & 1))
    return 0; /* do not respond to
                 non-recursive queries
               */
  if(header[2] & 120)
    return 0;
  if(header[2] & 2)
    return 0;
  if(byte_diff(header + 4, 2, "\0\1"))
    return 0;

  pos = dns_packet_getname(buf, len, pos, q);
  if(!pos)
    return 0;
  pos = dns_packet_copy(buf, len, pos, qtype, 2);
  if(!pos)
    return 0;
  pos = dns_packet_copy(buf, len, pos, qclass, 2);
  if(!pos)
    return 0;
  if(byte_diff(qclass, 2, DNS_C_IN) && byte_diff(qclass, 2, DNS_C_ANY))
    return 0;

  byte_copy(id, 2, header);
  errno = 0;
  return 1;
}

void
udp_drop(int j) {
  if(!u[j].active)
    return;
  log_querydrop(&u[j].active);
  u[j].active = 0;
  --uactive;
}

void
udp_respond(int j) {
  response* resp;
  if(!u[j].active)
    return;
  resp = &u[j].q.response;
  response_id(resp, u[j].id);
  if(resp->pos > 512)
    response_tc(resp);

  response_send(resp, udp53, u[j].ip, u[j].port);
  log_querydone(&u[j].active, u[j].ip, u[j].port, udp53, u[j].id, resp->pos);

  u[j].active = 0;
  --uactive;
}

void
udp_new(void) {
  size_t j, i, len;
  ssize_t ret;
  struct udpclient* x;
  char *q = 0, qtype[2], qclass[2];

  for(j = 0; j < MAXUDP; ++j)
    if(!u[j].active)
      break;

  if(j >= MAXUDP) {
    j = 0;
    for(i = 1; i < MAXUDP; ++i)
      if(taia_less(&u[i].start, &u[j].start))
        j = i;
    errno = ETIMEDOUT;
    udp_drop(j);
  }

  x = u + j;
  taia_now(&x->start);

  if((ret = socket_recv6(udp53, buf, sizeof buf, x->ip, &x->port, &x->scope_id)) == -1)
    return;
  len = (size_t)ret;
  if(len >= sizeof buf)
    return;
  if(x->port < 1024)
    if(x->port != 53)
      return;
  if(!nscache_okclient(x->ip, true))
    return;

  if(!packetquery(buf, len, &q, qtype, qclass, x->id))
    return;

  x->active = ++numqueries;
  ++uactive;
  log_query(&x->active, x->ip, x->port, udp53, x->id, q, qtype);

  switch(query_start(&x->q, q, qtype, qclass, sendaddr)) {
    case -1: {
      udp_drop(j);
      return;
    }
    case 1: udp_respond(j);
  }
}

int
udp_num(const void* ptr) {
  size_t n = ((size_t)ptr - (size_t)u) / sizeof(struct udpclient);
  if(n >= MAXUDP)
    return -1;
  return n;
}

/*
state 1: buf 0; normal state at
beginning of TCP connection state 2: buf
0; have read 1 byte of query packet
length into len state 3: buf allocated;
have read pos bytes of buf state 0: buf
0; handling query in q state -1: buf
allocated; have written pos bytes
*/

void
tcp_free(int j) {
  if(!t[j].buf)
    return;
  alloc_free(t[j].buf);
  t[j].buf = 0;
}

void
tcp_timeout(int j) {
  struct taia now;
  if(!t[j].active)
    return;
  taia_now(&now);
  taia_uint(&t[j].timeout, 10);
  taia_add(&t[j].timeout, &t[j].timeout, &now);
}

void
tcp_close(int j) {
  if(!t[j].active)
    return;
  tcp_free(j);
  errno = 0;
  log_tcpclose(t[j].ip, t[j].port, t[j].tcp);
  close(t[j].tcp);

  t[j].active = 0;
  --tactive;
}

void
tcp_drop(int j) {
  log_querydrop(&t[j].active);
  errno = EPIPE;
  tcp_close(j);
}

void
tcp_respond(int j) {
  response* resp;
  if(!t[j].active)
    return;
  resp = &t[j].q.response;
  log_querydone(&t[j].active, t[j].ip, t[j].port, t[j].tcp, t[j].id, resp->pos);
  response_id(resp, t[j].id);
  t[j].len = resp->pos + 2;
  tcp_free(j);
  t[j].buf = alloc(resp->pos + 2);
  if(!t[j].buf) {
    tcp_close(j);
    return;
  }
  uint16_pack_big(t[j].buf, resp->pos);
  byte_copy(t[j].buf + 2, resp->pos, resp->buf);
  t[j].pos = 0;
  t[j].state = -1;
}

void
tcp_rw(int j) {
  struct tcpclient* x;
  char ch;
  static char* q = 0;
  char qtype[2];
  char qclass[2];
  int r;

  x = t + j;
  if(x->state == -1) {
    r = write(x->tcp, x->buf + x->pos, x->len - x->pos);
    if(r <= 0) {
      tcp_close(j);
      return;
    }
    x->pos += r;
    if(x->pos == x->len) {
      tcp_free(j);
      x->state = 1; /* could drop connection
                       immediately */
    }
    return;
  }

  r = read(x->tcp, &ch, 1);
  if(r == 0) {
    errno = EPIPE;
    tcp_close(j);
    return;
  }
  if(r < 0) {
    tcp_close(j);
    return;
  }

  if(x->state == 1) {
    x->len = (unsigned char)ch;
    x->len <<= 8;
    x->state = 2;
    return;
  }
  if(x->state == 2) {
    x->len += (unsigned char)ch;
    if(!x->len) {
      errno = EPROTO;
      tcp_close(j);
      return;
    }
    x->buf = alloc(x->len);
    if(!x->buf) {
      tcp_close(j);
      return;
    }
    x->pos = 0;
    x->state = 3;
    return;
  }

  if(x->state != 3)
    return; /* impossible */

  x->buf[x->pos++] = ch;
  if(x->pos < x->len)
    return;

  if(!packetquery(x->buf, x->len, &q, qtype, qclass, x->id)) {
    tcp_close(j);
    return;
  }

  x->active = ++numqueries;
  log_query(&x->active, x->ip, x->port, x->tcp, x->id, q, qtype);
  log_query(&x->active, x->ip, x->port, x->tcp, x->id, q, qtype);
  switch(query_start(&x->q, q, qtype, qclass, sendaddr)) {
    case -1: tcp_drop(j); return;
    case 1: tcp_respond(j); return;
  }
  tcp_free(j);
  x->state = 0;
}

void
tcp_new(void) {
  int i;
  int j;
  struct tcpclient* x;

  for(j = 0; j < MAXTCP; ++j)
    if(!t[j].active)
      break;

  if(j >= MAXTCP) {
    j = 0;
    for(i = 1; i < MAXTCP; ++i)
      if(taia_less(&t[i].start, &t[j].start))
        j = i;
    errno = ETIMEDOUT;
    if(t[j].state == 0)
      tcp_drop(j);
    else
      tcp_close(j);
  }

  x = t + j;
  taia_now(&x->start);

  x->tcp = socket_accept6(tcp53, x->ip, &x->port, &x->scope_id);
  if(x->tcp == -1)
    return;
  if(x->port < 1024)
    if(x->port != 53) {
      close(x->tcp);
      return;
    }
  if(!nscache_okclient(x->ip, true)) {
    close(x->tcp);
    return;
  }
  if(ndelay_on(x->tcp) == -1) {
    close(x->tcp);
    return;
  } /* Linux bug */

  x->active = 1;
  ++tactive;
  x->state = 1;
  tcp_timeout(j);

  log_tcpopen(x->ip, x->port, x->tcp);
}

int
tcp_num(const void* ptr) {
  size_t n = ((size_t)ptr - (size_t)t) / sizeof(struct tcpclient);
  if(n >= MAXTCP)
    return -1;
  return n;
}

void
nscache_run(void) {
  int j;
  struct taia deadline;
  struct taia stamp;
  int iolen;
  int r;

  for(;;) {
    taia_now(&stamp);
    taia_uint(&deadline, 120);
    taia_add(&deadline, &deadline, &stamp);

    iolen = 0;

    udp53io = io + iolen++;
    udp53io->fd = udp53;
    udp53io->events = IOPAUSE_READ;

    tcp53io = io + iolen++;
    tcp53io->fd = tcp53;
    tcp53io->events = IOPAUSE_READ;

    for(j = 0; j < MAXUDP; ++j)
      if(u[j].active) {
        u[j].io = io + iolen++;
        query_io(&u[j].q, u[j].io, &deadline);
      }

    for(j = 0; j < MAXTCP; ++j)
      if(t[j].active) {
        t[j].io = io + iolen++;
        if(t[j].state == 0)
          query_io(&t[j].q, t[j].io, &deadline);
        else {
          if(taia_less(&t[j].timeout, &deadline))
            deadline = t[j].timeout;
          t[j].io->fd = t[j].tcp;
          t[j].io->events = (t[j].state > 0) ? IOPAUSE_READ : IOPAUSE_WRITE;
        }
      }

    iopause(io, iolen, &deadline, &stamp);

    for(j = 0; j < MAXUDP; ++j)
      if(u[j].active) {

        r = query_get(&u[j].q, u[j].io, &stamp);
        if(r == -1)
          udp_drop(j);
        if(r == 1)
          udp_respond(j);
        query_dump(&u[j].q);
      }

    for(j = 0; j < MAXTCP; ++j)
      if(t[j].active) {
        if(t[j].io->revents)
          tcp_timeout(j);
        if(t[j].state == 0) {
          r = query_get(&t[j].q, t[j].io, &stamp);
          if(r == -1)
            tcp_drop(j);
          if(r == 1)
            tcp_respond(j);
        } else if(t[j].io->revents || taia_less(&t[j].timeout, &stamp))
          tcp_rw(j);
        query_dump(&t[j].q);
      }

    if(udp53io)
      if(udp53io->revents)
        udp_new();

    if(tcp53io)
      if(tcp53io->revents)
        tcp_new();
  }
}

int
main(int argc, char* argv[]) {
  const char *x, *y;
  unsigned long cachesize;
  int i;
  int v6only_prev;

  uint32_seed(NULL, 0);

  for(i = 1; i < argc; i++) env_put(argv[i]);

  x = env_get("IP");
  if(!x)
    die(111, FATAL, "$IP not set");
  if(!scan_ip6if(x, bindaddr, &bindscope))
    die(111, FATAL, "unable to parse IP address ", x);

  udp53 = socket_udp6();
  /// v6only_prev = socket_v6only(udp53,
  /// 0);

  if(udp53 == -1)
    diesys(111,
           FATAL,
           "unable to create UDP "
           "socket: ");
  if(socket_bind6_reuse(udp53, bindaddr, 53, bindscope) == -1)
    diesys(111, FATAL, "unable to bind UDP socket: ");

  tcp53 = socket_tcp6();
  // v6only_prev = socket_v6only(udp53,
  // 0);

  if(tcp53 == -1)
    diesys(111,
           FATAL,
           "unable to create TCP "
           "socket: ");
  if(socket_bind6_reuse(tcp53, bindaddr, 53, bindscope) == -1)
    diesys(111, FATAL, "unable to bind TCP socket: ");

  nscache_droproot(FATAL);

  socket_tryreservein(udp53, 131072);

  for(i = 0; i < 32; i++) seed.u32[i] = uint32_random();

  dns_random_init(seed.u8);

  x = env_get("IPSEND");
  if(!x)
    die(111, FATAL, "$IPSEND not set");
  if(!scan_ip6(x, sendaddr))
    die(111, FATAL, "unable to parse IP address ", x);

  x = env_get("CACHESIZE");
  if(!x)
    die(111, FATAL, "$CACHESIZE not set");
  scan_ulong(x, &cachesize);

  y = env_get("CACHEFILE");
  if(y) {
    if(!cache_open(y, cachesize))
      diesys(111, FATAL, "unable to open cache file ", y);

  } else {
    if(!cache_init(cachesize))
      die(111,
          FATAL,
          "not enough memory for cache "
          "of size ",
          x);
  }

  if(env_get("HIDETTL"))
    response_hidettl = 1;

  if(env_get("FORWARDONLY"))
    query_forwardonly();

  if(!roots_init())
    diesys(111, FATAL, "unable to read servers: ");

  if(socket_listen(tcp53, 20) == -1)
    diesys(111,
           FATAL,
           "unable to listen on TCP "
           "socket: ");

  log_startup();
  nscache_run();
}
