#include "../byte.h"
#include "../dns.h"
#include "../uint16.h"
#include "../stralloc.h"

static char* q = 0;

int
dns_ip4(stralloc* out, const stralloc* fqdn) {
  unsigned int i;
  char code;
  char ch;

  if(!stralloc_copys(out, ""))
    return -1;
  code = 0;

  for(i = 0; i <= fqdn->len; ++i) {
    if(i < fqdn->len)
      ch = fqdn->s[i];
    else
      ch = '.';

    if((ch == '[') || (ch == ']'))
      continue;

    if(ch == '.') {
      if(!stralloc_append(out, &code))
        return -1;
      code = 0;
      continue;
    }

    if((ch >= '0') && (ch <= '9')) {
      code *= 10;
      code += ch - '0';
      continue;
    }

    if(!dns_domain_fromdot(&q, fqdn->s, fqdn->len))
      return -1;

    if(dns_resolve(q, DNS_T_A) == -1)
      return -1;

    if(dns_ip4_packet(out, dns_resolve_tx.packet, dns_resolve_tx.packetlen) == -1)
      return -1;
    dns_transmit_free(&dns_resolve_tx);
    dns_domain_free(&q);
    return 0;
  }

  out->len &= ~3;
  return 0;
}
