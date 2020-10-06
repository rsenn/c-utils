#include "lib/dns.h"
#include "lib/byte.h"
#include "lib/uint16.h"

#include <assert.h>

#include "response.h"

/*char response[65535];
unsigned int response_len = 0;  */

#define NAMES 100

struct names {
 char str[NAMES][128];
 unsigned int ptr[NAMES];
 unsigned int num;
};

static struct names name; 
int response_hidettl = 0;

int
response_addbytes(stralloc* resp, const char* buf, unsigned int len) {
  if(len > 65535 - resp->len)
    return 0;
  stralloc_catb(resp, buf, len);
  return 1;
}

int
response_addname(stralloc* resp, const char* d) {
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
        name.ptr[name.num] = resp->len;
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
response_query(stralloc* resp, const char* q, const char qtype[2], const char qclass[2]) {
  stralloc_zero(resp);
  name.num = 0;
  if(!response_addbytes(resp, "\0\0\201\200\0\1\0\0\0\0\0\0", 12))
    return 0;
  if(!response_addname(resp, q))
    return 0;
  if(!response_addbytes(resp, qtype, 2))
    return 0;
  if(!response_addbytes(resp, qclass, 2))
    return 0;
  // tctarget =;
  return resp->len;
}


int
response_rstart(stralloc* resp, const char* d, const char type[2], uint32 ttl) {
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
  // dpos = resp->len;
  return resp->len;
}

void
response_rfinish(stralloc* resp, int x, unsigned int dpos) {
  char* response = resp->s;
  size_t response_len = resp->len;
  uint16_pack_big(response + dpos - 2, response_len - dpos);
  if(!++response[x + 1])
    ++response[x];
}

int
response_cname(stralloc* resp, const char* c, const char* d, uint32 ttl) {
  int dpos;
  if(!(dpos = response_rstart(resp, c, DNS_T_CNAME, ttl)))
    return 0;
  if(!response_addname(resp, d))
    return 0;
  response_rfinish(resp, RESPONSE_ANSWER, dpos);
  return dpos;
}

void
response_nxdomain(stralloc* resp) {
  char* response = resp->s;
  assert(resp->len > 3);

  response[3] |= 3;
  response[2] |= 4;
}

void
response_servfail(stralloc* resp) {
  char* response = resp->s;
  assert(resp->len > 3);
  response[3] |= 2;
}

void
response_id(stralloc* resp, const char id[2]) {
  if(resp->len < 2)
    stralloc_ready(resp, 2);
  byte_copy(resp->s, 2, id);
}

void
response_tc(stralloc* resp, int tctarget) {
  assert(resp->len > 2);
  resp->s[2] |= 2;
  resp->len = tctarget;
}
