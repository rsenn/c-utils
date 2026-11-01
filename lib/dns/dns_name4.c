#include "../byte.h"
#include "../dns.h"
#include "../ip6.h"
#include "../stralloc.h"
#include "../uint16.h"

static char* q = 0;

static int
dns_name6_inner(stralloc* out, const char ip[16]) {
  char name[DNS_NAME6_DOMAIN];

  dns_name6_domain(name, ip);

  if(dns_resolve(name, DNS_T_PTR) == -1)
    return -1;

  if(dns_name_packet(out, dns_resolve_tx.packet, dns_resolve_tx.packetlen) == -1)
    return -1;
  dns_transmit_free(&dns_resolve_tx);
  dns_domain_free(&q);
  return 0;
}

int
dns_name4(stralloc* out, const char ip[4]) {
  char name[DNS_NAME4_DOMAIN];

  dns_name4_domain(name, ip);

  if(dns_resolve(name, DNS_T_PTR) == -1)
    return -1;

  if(dns_name_packet(out, dns_resolve_tx.packet, dns_resolve_tx.packetlen) == -1)
    return -1;
  dns_transmit_free(&dns_resolve_tx);
  dns_domain_free(&q);
  return 0;
}
