#include "lib/dns.h"
#include "lib/ip4.h"
#include "lib/ip6.h"
#include "lib/taia.h"
#include "lib/io.h"
#include "lib/errmsg.h"
#include "address.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static bool address_initialized = false;
static char address_seed[128];

int
address_init(address_t* addr, char ip[16], bool ip6, uint32 scope_id) {
  if(ip6) {
    byte_copy(addr->ip, 16, ip);
  } else {
    byte_copy(addr->ip, 4, ip);
    byte_zero(&addr->ip[4], 12);
  }
  addr->ip6 = ip6;
  addr->scope_id = ip6 ? scope_id : UINT32_MAX;
  return 1;
}

size_t
address_scan(const char* x, address_t* addr) {
  size_t i;
  byte_zero(addr, sizeof(address_t));

  if((i = scan_ip4(x, addr->ip)) == 0) {
    if((i = scan_ip6if(x, addr->ip, &addr->scope_id)))
      addr->ip6 = true;
  } else {
    addr->ip6 = false;
  }
  return i;
}

size_t
address_fmt(char* x, const address_t* addr) {
  size_t n;

  if(addr->ip6)
    n = fmt_ip6if(x, addr->ip, addr->scope_id);
  else
    n = fmt_ip4(x, addr->ip);
  return n;
}

int
address_lookup(stralloc* name, address_t* addr, bool no_ip6) {
  stralloc ips;
  stralloc_init(&ips);
  stralloc_ready(&ips, 16);
  byte_zero(addr, sizeof(address_t));

  if(!address_initialized)
    dns_random_init(address_seed);

  if(no_ip6 || dns_ip6(&ips, name) == -1) {
    if(dns_ip4(&ips, name) == -1) {
      errmsg_warnsys("unable to find "
                     "IP address for ",
                     name->s,
                     0);
      return 0;
    } else {
      addr->ip6 = false;
    }
  } else {
    addr->ip6 = true;
  }
  byte_copy(addr->ip, MIN(addr->ip6 ? 16 : 4, ips.len), ips.s);
  stralloc_free(&ips);
  return 1;
}
