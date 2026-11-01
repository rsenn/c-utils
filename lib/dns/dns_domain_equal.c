#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

int
dns_domain_equal(const char* dn1, const char* dn2) {
  unsigned int len;

  len = dns_domain_length(dn1);

  if(len != dns_domain_length(dn2))
    return 0;

  if(case_diffb(dn1, len, dn2))
    return 0; /* safe since 63 < 'A' */
  return 1;
}
