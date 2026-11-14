#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include "../str.h"
#include "../stralloc.h"

int
dns_ip4_qualify(stralloc* out, stralloc* fqdn, const stralloc* in) {
  static stralloc rules;

  if(dns_resolvconfrewrite(&rules) == -1)
    return -1;

  return dns_ip4_qualify_rules(out, fqdn, in, &rules);
}
