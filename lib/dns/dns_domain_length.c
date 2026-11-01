#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

unsigned int
dns_domain_length(const char* dn) {
  const char* x;
  unsigned char c;

  x = dn;

  while((c = *x++))
    x += (unsigned int)c;
  return x - dn;
}
