#include "lib/uint64.h"
#include "lib/uint32.h"
#include "lib/uint16.h"
#include "lib/buffer.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/byte.h"
#include "lib/errmsg.h"
#include "lib/fmt.h"
#include "lib/dns.h"
#include "lib/socket.h"

#include "log.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>

static int iphex = 0;

static const char*
errstr() {
  const char* s;
  int error = errno;
  errno = 0;
  // assert(error);
  s = strerror(error);
  return s;
}

void
log_number(uint64 x) {
  buffer_putulonglong(buffer_2, x);
}

void
log_hex(unsigned char c) {
  static const char hexdigits[] = "0123456789abcdef";

  buffer_put(buffer_2, &hexdigits[(c >> 4)], 1);
  buffer_put(buffer_2, &hexdigits[(c & 15)], 1);
}

void
log_port(uint16 port) {
  log_string(":");

  /*  log_number(port);
    return;*/
  log_hex(port >> 8);
  log_hex(port & 0xff);
}

void
log_octal(long o) {
  char buf[FMT_8LONG];
  // buffer_putc(buffer_2, '0');
  buffer_put(buffer_2, buf, fmt_8long(buf, o));
}

void
log_string(const char* s) {
  buffer_puts(buffer_2, s);
}

void
log_line(void) {
  buffer_putnlflush(buffer_2);
}

void
log_space(void) {
  log_string(" ");
}

void
log_separator(void) {
  log_string("|");
}

void
log_ip(const char* x, size_t iplen) {
  if(!iphex) {
    char buf[FMT_IP6];

    if(iplen == 16 && ip6_isv4mapped(x)) {
      log_ip(&x[12], 4);
      return;
    }

    buffer_put(buffer_2, buf, (iplen == 4 ? fmt_ip4 : fmt_ip6)(buf, x));
  } else {
    int j;
    for(j = ip6_isv4mapped(x) ? 12 : 0; j < 16; j++) log_hex(x[j]);
  }
}

void
log_iplist(const char* x, size_t n, size_t iplen) {
  size_t i;
  char buf[FMT_IP4];
  for(i = 0; i + iplen <= n; i += iplen) {
    if(i > 0)
      log_string(",");
    log_ip(&x[i], iplen);
  }
}

void
log_fd(int fd) {
  int type = 0;
  socklen_t length = sizeof(type);
  getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &length);
  log_string(type == SOCK_STREAM ? "tcp" : type == SOCK_DGRAM ? "udp" : "sock");
  // log_string(socket_is6(fd) ? "6" : "4");
  log_string("/");
  log_number(fd);
}

void
log_peer(const char ip[16], uint16 port, int fd) {
  log_ip(ip, 16);
  log_port(port);
  log_space();
  log_fd(fd);
}

void
log_logid(const char id[2]) {
  log_hex(id[0]);
  log_hex(id[1]);
}

void
log_logtype(const char type[2]) {
  uint16 u;
  if(byte_equal(type, 2, DNS_T_A))
    log_string("a");
  else if(byte_equal(type, 2, DNS_T_NS))
    log_string("ns");
  else if(byte_equal(type, 2, DNS_T_CNAME))
    log_string("cname");
  else if(byte_equal(type, 2, DNS_T_SOA))
    log_string("soa");
  else if(byte_equal(type, 2, DNS_T_PTR))
    log_string("ptr");
  else if(byte_equal(type, 2, DNS_T_HINFO))
    log_string("hinfo");
  else if(byte_equal(type, 2, DNS_T_MX))
    log_string("mx");
  else if(byte_equal(type, 2, DNS_T_TXT))
    log_string("txt");
  else if(byte_equal(type, 2, DNS_T_RP))
    log_string("rp");
  else if(byte_equal(type, 2, DNS_T_SIG))
    log_string("sig");
  else if(byte_equal(type, 2, DNS_T_KEY))
    log_string("key");
  else if(byte_equal(type, 2, DNS_T_AAAA))
    log_string("aaaa");
  else if(byte_equal(type, 2, DNS_T_AXFR))
    log_string("axfr");
  else if(byte_equal(type, 2, DNS_T_ANY))
    log_string("any");
  else {
    uint16_unpack_big(type, &u);

    if(u <= 255) {
      log_string("0o");
      log_octal(u);
    } else {
      log_string("0x");
      log_hex(u >> 8);
      log_hex(u & 0xff);
    }
  }
}

size_t
log_name(const char* name) {
  char ch;
  const char* q = name;
  int state;

  if(!*q) {
    log_string(".");
    return 1;
  }
  while(state = *q++) {
    while(state) {
      ch = *q++;
      --state;
      if((ch <= 32) || (ch > 126))
        ch = '?';
      if((ch >= 'A') && (ch <= 'Z'))
        ch += 32;
      buffer_put(buffer_2, &ch, 1);
    }
    log_string(".");
  }
  return q - name;
}

void
log_startup(void) {
  log_string("starting");
  log_line();
}

void
log_query(uint64* qnum, const char client[], unsigned int port, int fd, const char id[2], const char* q, const char qtype[2]) {
  log_string("query ");
  log_number(*qnum);
  log_space();
  log_peer(client, port, fd);
  log_space();
  log_logid(id);
  log_space();
  log_logtype(qtype);
  log_space();
  log_name(q);
  log_line();
}

void
log_querydone(uint64* qnum, const char client[], uint16 port, int fd, const char id[2], unsigned int len) {
  log_string("sent ");
  log_number(*qnum);
  log_space();
  log_peer(client, port, fd);
  log_space();
  log_logid(id);
  log_space();
  log_number(len);
  log_line();
}

void
log_querydrop(uint64* qnum) {
  const char* x = errstr();

  log_string("drop ");
  log_number(*qnum);
  log_space();
  log_string(x);
  log_line();
}

void
log_tcpopen(const char client[], unsigned int port, int fd) {
  log_string("tcpopen ");
  log_peer(client, port, fd);
  log_line();
}

void
log_tcpclose(const char client[], unsigned int port, int fd) {
  const char* x = errno ? errstr() : "Success";
  log_string("tcpclose ");
  log_peer(client, port, fd);

  log_space();
  log_string(x);
  log_line();
}

void
log_tx(const char* q, const char qtype[2], const char* control, const char servers[64], unsigned int gluelessness) {
  int i;

  log_string("tx ");
  log_number(gluelessness);
  log_space();
  log_logtype(qtype);
  log_space();
  log_name(q);
  log_space();
  log_name(control);
  for(i = 0; i < 64; i += 4)
    if(byte_diff(servers + i, 4, "\0\0\0\0")) {
      log_space();
      log_ip(servers + i, 4);
    }
  log_line();
}

void
log_cachedanswer(const char* q, const char type[2], const char* cached, size_t cachedlen) {
  int i;
  log_string("cached ");
  log_logtype(type);
  log_space();
  log_name(q);
  log_space();
  log_string("[");

  if(byte_equal(type, 2, DNS_T_A)) {
    log_iplist(cached, cachedlen, 4);
  } else if(byte_equal(type, 2, DNS_T_AAAA)) {
    log_iplist(cached, cachedlen, 16);
  } else if(byte_equal(type, 2, DNS_T_MX)) {
    uint16 prio;
    for(i = 0; i < cachedlen;) {
      uint16_unpack_big(&cached[i], &prio);

      if(i > 0)
        log_separator();

      log_number(prio);
      log_space();

      i += 2 + log_name(&cached[i + 2]);
    }

  } else if(byte_equal(type, 2, DNS_T_NS)) {
    for(i = 0; i < cachedlen;) {
      if(i > 0)
        log_separator();
      i += log_name(&cached[i]);
    }
  } else {
    for(i = 0; i < cachedlen; i++) {
      if(isalnum(cached[i])) {
        buffer_putc(buffer_2, cached[i]);
      } else if(cached[i] == 0) {
        log_string("\\0");
      } else {
        log_string("\\x");
        log_hex(cached[i]);
      }
    }
    log_string("] @ ");
    log_number(cachedlen);
    log_line();
    return;
  }
  log_string("]");
  log_line();
}

void
log_cachedcname(const char* dn, const char* dn2) {
  log_string("cached cname ");
  log_name(dn);
  log_space();
  log_name(dn2);
  log_line();
}

void
log_cachedns(const char* control, const char* ns) {
  log_string("cached ns ");
  log_name(control);
  log_space();
  log_name(ns);
  log_line();
}

void
log_cachednxdomain(const char* dn) {
  log_string("cached nxdomain ");
  log_name(dn);
  log_line();
}

void
log_nxdomain(const char server[16], const char* q, unsigned int ttl) {
  log_string("nxdomain ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_space();
  log_name(q);
  log_line();
}

void
log_nodata(const char server[16], const char* q, const char qtype[2], unsigned int ttl) {
  log_string("nodata ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_space();
  log_logtype(qtype);
  log_space();
  log_name(q);
  log_line();
}

void
log_lame(const char server[16], const char* control, const char* referral) {
  log_string("lame ");
  log_ip(server, 4);
  log_space();
  log_name(control);
  log_space();
  log_name(referral);
  log_line();
}

void
log_servfail(const char* dn) {
  errmsg_warnsys("servfail ", dn, 0);
  /*  const char* x = errstr();

    log_string("servfail ");
    log_name(dn);
    log_space();
    log_string(x);
    log_line();*/
}

void
log_rr(const char server[16], const char* q, const char type[2], const char* buf, unsigned int len, unsigned int ttl) {
  int i;

  log_string("rr ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_space();
  log_logtype(type);
  log_space();
  log_name(q);
  log_space();

  if(len == 4 || len == 16) {
    log_ip(buf, len);
  } else {
    for(i = 0; i < len; ++i) {
      log_hex(buf[i]);
      if(i > 30) {
        log_string("...");
        break;
      }
    }
  }
  log_line();
}

void
log_rrns(const char server[16], const char* q, const char* data, unsigned int ttl) {
  log_string("rr ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_string(" ns ");
  log_name(q);
  log_space();
  log_name(data);
  log_line();
}

void
log_rrcname(const char server[16], const char* q, const char* data, unsigned int ttl) {
  log_string("rr ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_string(" cname ");
  log_name(q);
  log_space();
  log_name(data);
  log_line();
}

void
log_rrptr(const char server[16], const char* q, const char* data, unsigned int ttl) {
  log_string("rr ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_string(" ptr ");
  log_name(q);
  log_space();
  log_name(data);
  log_line();
}

void
log_rrmx(const char server[16], const char* q, const char* mx, const char pref[2], unsigned int ttl) {
  uint16 u;

  log_string("rr ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_string(" mx ");
  log_name(q);
  log_space();
  uint16_unpack_big(pref, &u);
  log_number(u);
  log_space();
  log_name(mx);
  log_line();
}

void
log_rrsoa(const char server[16], const char* q, const char* n1, const char* n2, const char misc[20], unsigned int ttl) {
  uint32 u;
  int i;

  log_string("rr ");
  log_ip(server, 4);
  log_space();
  log_number(ttl);
  log_string(" soa ");
  log_name(q);
  log_space();
  log_name(n1);
  log_space();
  log_name(n2);
  for(i = 0; i < 20; i += 4) {
    uint32_unpack_big(misc + i, &u);
    log_space();
    log_number(u);
  }
  log_line();
}

void
log_stats(void) {
  extern uint64 numqueries;
  extern uint64 cache_motion;
  extern int uactive;
  extern int tactive;

  log_string("stats ");
  log_number(numqueries);
  log_space();
  log_number(cache_motion);
  log_space();
  log_number(uactive);
  log_space();
  log_number(tactive);
  log_line();
}
