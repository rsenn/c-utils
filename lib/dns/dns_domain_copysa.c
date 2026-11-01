#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

int
dns_domain_copysa(stralloc* sa, const char* in) {
  unsigned int len;

  len = dns_domain_length(in);
  stralloc_copyb(sa, in, len);
  return len;
}
