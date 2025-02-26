
#include "lib/dns.h"

#include "lib/ip6.h"
#include "lib/ip4.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/case.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/fmt.h"
#include "lib/scan.h"
#include "lib/socket.h"
#include "lib/alloc.h"
#include "lib/stralloc.h"
#include "lib/uint16.h"

#include "libc-resolv-override.h"
#include <errno.h>
#include <netdb.h>

static char ip[256];

static int
dns_domain_todot(char* out, const char* d, size_t n) {
  char ch;
  char ch2;
  /*  char ch3;*/
  int idx = 0;

  if(!n)
    return 0;

  if(!*d) {
    *out = '\0';
    return 0;
  }

  for(; idx < n;) {
    ch = *d++;

    while(ch--) {
      ch2 = *d++;

      if((ch2 >= 'A') && (ch2 <= 'Z'))
        ch2 += 0x20;

      if(((ch2 >= 'a') && (ch2 <= 'z')) || ((ch2 >= '0') && (ch2 <= '9')) || (ch2 == '-') || (ch2 == '_')) {
        out[idx++] = ch2;

        if(idx == n)
          break;
      }
      /*          else
                  {
                    ch3 = ch2;
                    buf[3] = '0' + (ch3
         & 7); ch3 >>= 3; buf[2] = '0' +
         (ch3 & 7); ch3 >>= 3; buf[1] =
         '0' + (ch3 & 7); buf[0] = '\\';
                  }*/
    }

    if(!*d || idx == n)
      break;

    out[idx++] = '.';
  }

  out[idx] = '\0';

  return idx;
}
#if 0
static int
dns_event(struct dns_resolver* d) {
  char udpbuf[513];
  unsigned char ch;
  int r;
  int fd = d->s1 - 1;

  if(d->tcpstate == 0) {
    /* have attempted to send UDP query to each server udploop times
       have sent query to curserver on UDP socket s
     */
    r = recv(fd, udpbuf, sizeof(udpbuf), 0);

    if(r <= 0) {
      if(errno == ECONNREFUSED)

        if(d->udploop == 2)
          return 0;

      return nextudp(d);
    }

    if(r + 1 > sizeof(udpbuf))
      return 0;

    if(irrelevant(d, udpbuf, r))
      return 0;

    if(serverwantstcp(udpbuf, r))
      return firsttcp(d);

    if(serverfailed(udpbuf, r)) {
      if(d->udploop == 2)
        return 0;
      return nextudp(d);
    }

    socketfree(d);

    d->packetlen = r;
    d->packet = malloc(d->packetlen);

    if(!d->packet) {
      dns_free(d);
      return -1;
    }
    memcpy(d->packet, udpbuf, d->packetlen);
    queryfree(d);
    return 1;
  }

  if(d->tcpstate == 1) {
    /* have sent connection attempt to curserver on TCP socket s
       pos not defined
     */

    if(!socket_connected(fd))
      return nexttcp(d);
    d->pos = 0;
    d->tcpstate = 2;
    return 0;
  }

  if(d->tcpstate == 2) {
    /* have connection to curserver on TCP socket s
       have sent pos bytes of query
     */
    r = write(fd, d->query + d->pos, d->querylen - d->pos);

    if(r <= 0)
      return nexttcp(d);
    d->pos += r;

    if(d->pos == d->querylen) {
      d->deadline = time(0) + 10;
      d->tcpstate = 3;
    }

    return 0;
  }

  if(d->tcpstate == 3) {
    /* have sent entire query to curserver on TCP socket s
       pos not defined
     */
    r = read(fd, &ch, 1);

    if(r <= 0)
      return nexttcp(d);
    d->packetlen = ch;
    d->tcpstate = 4;
    return 0;
  }

  if(d->tcpstate == 4) {
    /* have sent entire query to curserver on TCP socket s
       pos not defined have received one byte of packet
       length into packetlen
     */
    r = read(fd, &ch, 1);

    if(r <= 0)
      return nexttcp(d);
    d->packetlen <<= 8;
    d->packetlen += ch;
    d->tcpstate = 5;
    d->pos = 0;
    d->packet = malloc(d->packetlen);

    if(!d->packet) {
      dns_free(d);
      return -1;
    }
    return 0;
  }

  if(d->tcpstate == 5) {
    /* have sent entire query to curserver on TCP socket s
       have received entire packet length into packetlen
       packet is allocated have received pos bytes of packet
     */
    r = read(fd, d->packet + d->pos, d->packetlen - d->pos);

    if(r <= 0)
      return nexttcp(d);
    d->pos += r;

    if(d->pos < d->packetlen)
      return 0;

    socketfree(d);

    if(irrelevant(d, d->packet, d->packetlen))
      return nexttcp(d);

    if(serverwantstcp(d->packet, d->packetlen))
      return nexttcp(d);

    if(serverfailed(d->packet, d->packetlen))
      return nexttcp(d);

    queryfree(d);
    return 1;
  }

  return 0;
}
#endif
/*
static int
dns_get_addr(struct dns_resolver* dns,
struct in_addr* addr) { int ret;
  stralloc_zero(&dns->result);
  ret = dns_ip4_packet(&dns->result,
dns->t.packet, dns->t.packetlen);
  dns->ans = dns->result.len / 4;
  return ret;
}

static int
dns_get_name(struct dns_resolver* dns,
char* out, size_t len) { unsigned int
pos; char header[12]; unsigned short
numanswers; unsigned short datalen;
  char* name;

  stralloc_zero(&dns->result);

  pos = dns_packet_copy(dns->t.packet,
dns->t.packetlen, 0, header, 12);

  if(!pos)
    return -1;

  uint16_unpack_big(header + 6,
&numanswers);

  pos =
dns_packet_skipname(dns->t.packet,
dns->t.packetlen, pos);

  if(!pos)
    return -1;

  pos += 4;

  while(numanswers--) {
    pos =
dns_packet_skipname(dns->t.packet,
dns->t.packetlen, pos);

    if(!pos)
      return -1;

    pos = dns_packet_copy(dns->t.packet,
dns->t.packetlen, pos, header, 10);

    if(!pos)
      return -1;

    uint16_unpack_big(header + 8,
&datalen);

    if(byte_equal(header, 2, DNS_T_PTR))

      if(byte_equal(header + 2, 2,
DNS_C_IN)) {
        if(!dns_packet_getname(dns->t.packet,
dns->t.packetlen, pos, &name)) return
-1;

        if(!dns_domain_todot(out, name,
len)) return -1;

        return 1;
      }

    pos += datalen;
  }

  return 0;
}

static int
dns_name_lookup(struct dns_resolver*
dns, const char* name) { char* domain =
0;

  dns_domain_fromdot(&domain, name, 64);

  if(dns_resolvconfip(ip) == -1)
    return -1;

  if(dns_transmit_start(&dns->t, ip, 1,
domain, DNS_T_A, (char*)&in6addr_any) ==
-1) return -1;

  return 0;
}
*/
/*
static int
dns_post_poll(struct dns_resolver* d,
const struct pollfd* x) { int fd;

  if(!d->s1)
    return 0;

  fd = d->s1 - 1;

  if(!x->revents) {
    if(time(0) < d->deadline)
      return 0;
    errno = ETIMEDOUT;

    if(d->tcpstate == 0)
      return nextudp(d);

    return nexttcp(d);
  }

  return dns_event(d);
}

static int
dns_post_select(struct dns_resolver* d,
const fd_set* readset, const fd_set*
writeset) { int fd;

  if(!d->s1)
    return 0;

  fd = d->s1 - 1;

  if(!FD_ISSET(fd, readset) &&
!FD_ISSET(fd, writeset)) { if(time(0) <
d->deadline) return 0; errno =
ETIMEDOUT; if(d->tcpstate == 0) return
nextudp(d);

    return nexttcp(d);
  }

  return dns_event(d);
}

static int
dns_pre_poll(struct dns_resolver* dns,
struct pollfd* pfd) { if(dns->s1) {
    pfd->fd = dns->s1 - 1;

    switch(dns->tcpstate) {
      case 0:
      case 3:
      case 4:
      case 5: pfd->events = POLLIN;
break; case 1: case 2: pfd->events =
POLLOUT; break;
    }

    return 1;
  }

  return 0;
}

static int
dns_pre_select(struct dns_resolver* dns,
fd_set* readset, fd_set* writeset) { int
fd;

  if(dns->s1) {
    fd = dns->s1 - 1;

    switch(dns->tcpstate) {
      case 0:
      case 3:
      case 4:
      case 5: FD_SET(fd, readset);
break; case 1: case 2: FD_SET(fd,
writeset); break;
    }

    return 1;
  }

  return 0;
} */

/* converts unsigned int to string.
 * returns length of the string. */
static unsigned int
utoa(char* buf, unsigned int i) {
  register char* p = buf;
  register unsigned int n = 0; /* buffer index */
  register unsigned int v;     /* digit value */

  if((v = i / 1000000000) || n)
    p[n++] = v + '0';
  i %= 1000000000;

  if((v = i / 100000000) || n)
    p[n++] = v + '0';
  i %= 100000000;

  if((v = i / 10000000) || n)
    p[n++] = v + '0';
  i %= 10000000;

  if((v = i / 1000000) || n)
    p[n++] = v + '0';
  i %= 1000000;

  if((v = i / 100000) || n)
    p[n++] = v + '0';
  i %= 100000;

  if((v = i / 10000) || n)
    p[n++] = v + '0';
  i %= 10000;

  if((v = i / 1000) || n)
    p[n++] = v + '0';
  i %= 1000;

  if((v = i / 100) || n)
    p[n++] = v + '0';
  i %= 100;

  if((v = i / 10) || n)
    p[n++] = v + '0';
  i %= 10;

  /* last remainder is last char */
  p[n++] = i + '0';

  /* null terminate the stuff */
  p[n] = '\0';

  return n;
}

static char ns[256];
static struct dns_resolver dns;
static char seed[128];
static stralloc fqdn;
static stralloc out;

static void
dns_init() {
  char b[64];
  size_t n, i;
  dns_zero(&dns);
  /*   const char* s;

    if((s = env_get("DNSCACHE"))) {
      stralloc_copys(&ns, s);
    }

    if(!ns.len) {
      size_t n, p = 0;
      const char* rcf;
      stralloc tmp;
      stralloc_init(&tmp);

      if((rcf = env_get("RESOLVCONF"))
    == NULL) rcf = "/etc/resolv.conf";
      openreadclose(rcf, &tmp, 1024);
      stralloc_nul(&tmp);

      do {
        p += case_finds(&tmp.s[p],
    tmp.len - p, "nameserver"); if(p +
    10 <= tmp.len) p += 10; } while(p <
    tmp.len && !(tmp.s[p] == ' ' ||
    tmp.s[p] == '\t')); p +=
    scan_whitenskip(&tmp.s[p], tmp.len -
    p); n =
    scan_nonwhitenskip(&tmp.s[p],
    tmp.len - p); stralloc_copyb(&ns,
    &tmp.s[p], n);
    } */
  dns_random_init(seed);

  n = dns_resolvconfip(ns);

  for(i = 0; byte_diff(ns + i,
                       16,
                       "\0\0\0\0\0\0\0\0\0\0\0"
                       "\0\0\0\0\0");
      i += 16) {
    b[fmt_ip6(b, ns + i)] = '\0';
    errmsg_info("nameserver: ", b, 0);
  }
}

#ifndef NO_MAIN
int
main(int argc, char* argv[]) {
  char str[IP4_FMT];
  unsigned int i;
  dns_init();

  if(*argv)
    ++argv;

  while(*argv) {
    if(!stralloc_copys(&fqdn, *argv)) {
      errmsg_warnsys("out of memory", 0);
      return 111;
    }

    if(dns_ip4(&out, &fqdn) == -1) {
      errmsg_warnsys("unable to find "
                     "IP address for ",
                     *argv,
                     0);
      return 111;
    }

    for(i = 0; i + 4 <= out.len; i += 4) {
      if(i)
        buffer_puts(buffer_1, " ");
      buffer_put(buffer_1, str, fmt_ip4(str, out.s + i));
    }
    buffer_puts(buffer_1, "\n");
    ++argv;
  }
  buffer_flush(buffer_1);
  return 0;
}
#endif

static char**
resolve_ip6(const char* name, int* nptr) {
  size_t i, n;
  char b[64];
  static char* addrlist[(256 / 16 + 1) * sizeof(char*)];
  byte_zero(&addrlist, sizeof(addrlist));

  errmsg_info("resolving: ", name, 0);

  if(!stralloc_copys(&fqdn, name)) {
    errmsg_warnsys("out of memory", 0);
    return 0;
  }

  if(dns_ip6(&out, &fqdn) == -1) {
    errmsg_warnsys("unable to find IP "
                   "address for ",
                   name,
                   0);
    return 0;
  }

  for(i = 0, n = 0; i < out.len && n < (sizeof(addrlist) / sizeof(addrlist[0]) - 1); i += 16, n++) {
    addrlist[n] = &out.s[i];

    b[fmt_ip6(b, addrlist[n])] = '\0';
    errmsg_info("result: ", b, 0);
  }

  if(nptr)
    *nptr = n;
  return addrlist;
}

struct hostent*
gethostbyname(const char* name) {
  size_t i;
  static struct hostent ret;
  static char* a[2] = {NULL, NULL};

  if(ret.h_addr_list)
    alloc_free(ret.h_addr_list);

  ret.h_name = name;
  ret.h_aliases = a;
  ret.h_addrtype = AF_INET6;
  ret.h_addr_list = resolve_ip6(name, &i);
  ret.h_length = i * 16;

  return &ret;
}

int
getaddrinfo(const char* node, const char* service, const struct addrinfo* hints, struct addrinfo** res) {

  struct hostent* h = gethostbyname(node);
  static struct addrinfo r;
  struct addrinfo* info;
  // struct sockaddr_in6* sin;
  /*  if(r.ai_addr)
     alloc_free(r.ai_addr); */
  byte_zero(&r, sizeof(r));

  r.ai_family = h->h_addrtype;
  // r.ai_protocol = PF_INET6;
  // r.ai_addr = sin =
  // alloc_zero(r.ai_addrlen =
  // sizeof(struct sockaddr_in6));
  r.ai_addrlen = sizeof(struct sockaddr_in6);

  r.ai_addr = h->h_addr_list[0];

  //  byte_copy(&sin->sin6_addr,
  //  sizeof(sin->sin6_addr),
  //  h->h_addr_list[0]);
  info = malloc(sizeof(struct addrinfo));
  byte_copy(info, (sizeof(struct addrinfo)), &r);
  *res = info;
  return 0;
}
