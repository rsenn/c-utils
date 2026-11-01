#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

int
dns_domain_suffix(const char* big, const char* little) {
  unsigned char c;

  for(;;) {
    if(dns_domain_equal(big, little))
      return 1;

    if(!(c = *big++))
      return 0;
    big += c;
  }
}
