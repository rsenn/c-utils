#include "lib/dns.h"
#include "lib/byte.h"
#include "lib/uint8.h"
#include "lib/uint16.h"
#include "lib/socket.h"
#include "lib/buffer.h"

#include "response.h"

#include <assert.h>

#define NAMES 100

struct names {
  char str[NAMES][128];
  uint32 ptr[NAMES];
  uint32 num;
};

static struct names name;
int response_hidettl = 0;

int
response_addbytes(response* resp, const char* buf, size_t len) {
  if(len > 65535 - resp->pos)
    return 0;
  stralloc_catb((stralloc*)resp, buf, len);
  return 1;
}

int
response_addname(response* resp, const char* d) {
  unsigned int dlen, i;
  char buf[2];

  dlen = dns_domain_length(d);

  while(*d) {
    for(i = 0; i < name.num; ++i)
      if(dns_domain_equal(d, name.str[i])) {
        uint16_pack_big(buf, 49152 + name.ptr[i]);
        return response_addbytes(resp, buf, 2);
      }
    if(dlen <= 128)
      if(name.num < NAMES) {
        byte_copy(name.str[name.num], dlen, d);
        name.ptr[name.num] = resp->pos;
        ++name.num;
      }
    i = (unsigned char)*d;
    ++i;
    if(!response_addbytes(resp, d, i))
      return 0;
    d += i;
    dlen -= i;
  }
  return response_addbytes(resp, d, 1);
}

int
response_query(response* resp, const char* q, const char qtype[2], const char qclass[2]) {
  stralloc_zero((stralloc*)resp);
  name.num = 0;
  if(!response_addbytes(resp, "\0\0\201\200\0\1\0\0\0\0\0\0", 12))
    return 0;
  if(!response_addname(resp, q))
    return 0;
  if(!response_addbytes(resp, qtype, 2))
    return 0;
  if(!response_addbytes(resp, qclass, 2))
    return 0;
  resp->tctarget = resp->pos;
  return 1;
}

int
response_rstart(response* resp, const char* d, const char type[2], uint32 ttl) {
  char ttlstr[4];
  if(!response_addname(resp, d))
    return 0;
  if(!response_addbytes(resp, type, 2))
    return 0;
  if(!response_addbytes(resp, DNS_C_IN, 2))
    return 0;
  uint32_pack_big(ttlstr, response_hidettl ? 0 : ttl);
  if(!response_addbytes(resp, ttlstr, 4))
    return 0;
  if(!response_addbytes(resp, "\0\0", 2))
    return 0;
  resp->dpos = resp->pos;
  return 1;
}

void
response_rfinish(response* resp, int x) {
  assert(resp->pos > x + 1);

  uint16_pack_big(&resp->buf[resp->dpos - 2], resp->pos - resp->dpos);
  if(!++resp->buf[x + 1])
    ++resp->buf[x];
}

char*
response_getdata(response* resp, size_t* size_ptr) {
  uint32 pos;
  char* x;

  x = &resp->buf[resp->dpos];
  *size_ptr = resp->pos - resp->dpos;
  return x;
}

int
response_send(response* resp, int fd, char ip[], uint16 port) {
  return socket_send6(fd, resp->buf, resp->pos, ip, port, 0);
}

int
response_cname(response* resp, const char* c, const char* d, uint32 ttl) {
  if(!response_rstart(resp, c, DNS_T_CNAME, ttl))
    return 0;
  if(!response_addname(resp, d))
    return 0;
  response_rfinish(resp, RESPONSE_ANSWER);
  return 1;
}

void
response_nxdomain(response* resp) {
  char* response = resp->buf;
  assert(resp->pos > 3);

  response[3] |= 3;
  response[2] |= 4;
}

void
response_servfail(response* resp) {
  char* response = resp->buf;
  assert(resp->pos > 3);
  response[3] |= 2;
}

void
response_id(response* resp, const char id[2]) {
  if(resp->pos < 2)
    stralloc_ready((stralloc*)resp, 2);
  byte_copy(resp->buf, 2, id);
}

void
response_tc(response* resp) {
  assert(resp->pos > 2);
  resp->buf[2] |= 2;
  resp->pos = resp->tctarget;
}

void
response_dump(response const* resp) {
  size_t i;
  buffer_puts(buffer_2, "len = ");
  buffer_putulong(buffer_2, resp->stra.len);

  buffer_puts(buffer_2, " = ");
  buffer_putulong(buffer_2, resp->stra.len);

  for(i = 0; i < resp->stra.len; i++) {
    uint8 ch = resp->stra.s[i];

    if(ch >= 'a' && ch <= 'z')
      buffer_putc(buffer_2, ch);
    else {
      buffer_putc(buffer_2, '\\');
      buffer_put8long(buffer_2, ch);
    }
  }
}
