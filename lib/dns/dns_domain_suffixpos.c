#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

unsigned int
dns_domain_suffixpos(const char* big, const char* little) {
  const char* orig = big;
  unsigned char c;

  for(;;) {
    if(dns_domain_equal(big, little))
      return big - orig;

    if(!(c = *big++))
      return 0;
    big += c;
  }
}
