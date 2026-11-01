#include "../byte.h"
#include "../dns.h"
#include "../stralloc.h"
#include "../uint16.h"

static char* q = 0;

int
dns_mx(stralloc* out, const stralloc* fqdn) {
  if(!dns_domain_fromdot(&q, fqdn->s, fqdn->len))
    return -1;

  if(dns_resolve(q, DNS_T_MX) == -1)
    return -1;

  if(dns_mx_packet(out, dns_resolve_tx.packet, dns_resolve_tx.packetlen) == -1)
    return -1;
  dns_transmit_free(&dns_resolve_tx);
  dns_domain_free(&q);
  return 0;
}
