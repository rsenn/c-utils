#include "lib/uint64.h"
#include "lib/uint32.h"
#include "lib/uint16.h"
#include "lib/buffer.h"
#include "lib/ip6.h"
#include "lib/byte.h"

#include "log.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

static const char*
errstr() {
  const char* s;
  assert(errno);
  s = strerror(errno);
  errno = 0;
  return s;
}

void
log_number(uint64 x) {
  buffer_putulonglong(buffer_2, x);
}

void
log_hex(unsigned char c) {
  buffer_put(buffer_2, "0123456789abcdef" + (c >> 4), 1);
  buffer_put(buffer_2, "0123456789abcdef" + (c & 15), 1);
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
log_ip(const char i[16]) {
  int j;
  for(j = ip6_isv4mapped(i) ? 12 : 0; j < 16; j++) log_hex(i[j]);
}

void
log_logid(const char id[2]) {
  log_hex(id[0]);
  log_hex(id[1]);
}

void
log_logtype(const char type[2]) {
  uint16 u;

  uint16_unpack_big(type, &u);
  log_number(u);
}

void
log_name(const char* q) {
  char ch;
  int state;

  if(!*q) {
    log_string(".");
    return;
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
}

void
log_startup(void) {
  log_string("starting");
  log_line();
}

void
log_query(uint64* qnum, const char client[], unsigned int port, const char id[2], const char* q, const char qtype[2]) {
  log_string("query ");
  log_number(*qnum);
  log_space();
  log_ip(client);
  log_string(":");
  log_hex(port >> 8);
  log_hex(port & 255);
  log_string(":");
  log_logid(id);
  log_space();
  log_logtype(qtype);
  log_space();
  log_name(q);
  log_line();
}

void
log_querydone(uint64* qnum, unsigned int len) {
  log_string("sent ");
  log_number(*qnum);
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
log_tcpopen(const char client[], unsigned int port) {
  log_string("tcpopen ");
  log_ip(client);
  log_string(":");
  log_hex(port >> 8);
  log_hex(port & 255);
  log_line();
}

void
log_tcpclose(const char client[], unsigned int port) {
  const char* x = errstr();
  log_string("tcpclose ");
  log_ip(client);
  log_string(":");
  log_hex(port >> 8);
  log_hex(port & 255);
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
      log_ip(servers + i);
    }
  log_line();
}

void
log_cachedanswer(const char* q, const char type[2]) {
  log_string("cached ");
  log_logtype(type);
  log_space();
  log_name(q);
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
log_nxdomain(const char server[], const char* q, unsigned int ttl) {
  log_string("nxdomain ");
  log_ip(server);
  log_space();
  log_number(ttl);
  log_space();
  log_name(q);
  log_line();
}

void
log_nodata(const char server[], const char* q, const char qtype[2], unsigned int ttl) {
  log_string("nodata ");
  log_ip(server);
  log_space();
  log_number(ttl);
  log_space();
  log_logtype(qtype);
  log_space();
  log_name(q);
  log_line();
}

void
log_lame(const char server[], const char* control, const char* referral) {
  log_string("lame ");
  log_ip(server);
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
log_rr(const char server[], const char* q, const char type[2], const char* buf, unsigned int len, unsigned int ttl) {
  int i;

  log_string("rr ");
  log_ip(server);
  log_space();
  log_number(ttl);
  log_space();
  log_logtype(type);
  log_space();
  log_name(q);
  log_space();

  for(i = 0; i < len; ++i) {
    log_hex(buf[i]);
    if(i > 30) {
      log_string("...");
      break;
    }
  }
  log_line();
}

void
log_rrns(const char server[], const char* q, const char* data, unsigned int ttl) {
  log_string("rr ");
  log_ip(server);
  log_space();
  log_number(ttl);
  log_string(" ns ");
  log_name(q);
  log_space();
  log_name(data);
  log_line();
}

void
log_rrcname(const char server[], const char* q, const char* data, unsigned int ttl) {
  log_string("rr ");
  log_ip(server);
  log_space();
  log_number(ttl);
  log_string(" cname ");
  log_name(q);
  log_space();
  log_name(data);
  log_line();
}

void
log_rrptr(const char server[], const char* q, const char* data, unsigned int ttl) {
  log_string("rr ");
  log_ip(server);
  log_space();
  log_number(ttl);
  log_string(" ptr ");
  log_name(q);
  log_space();
  log_name(data);
  log_line();
}

void
log_rrmx(const char server[], const char* q, const char* mx, const char pref[2], unsigned int ttl) {
  uint16 u;

  log_string("rr ");
  log_ip(server);
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
log_rrsoa(const char server[], const char* q, const char* n1, const char* n2, const char misc[20], unsigned int ttl) {
  uint32 u;
  int i;

  log_string("rr ");
  log_ip(server);
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
