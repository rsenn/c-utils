#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

int
dns_domain_diff(const char* dn1, const char* dn2) {
  unsigned int len1, len2;

  len1 = dns_domain_length(dn1);
  len2 = dns_domain_length(dn2);

  if(len1 < len2)
    return -(unsigned int)(unsigned char)dn2[len1];

  if(len1 > len2)
    return (unsigned int)(unsigned char)dn1[len2];

  return case_diffb(dn1, len1, dn2);
}
