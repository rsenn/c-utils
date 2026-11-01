#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

int
dns_domain_copy(char** out, const char* in) {
  unsigned int len;
  char* x;

  len = dns_domain_length(in);

  if(!(x = malloc(len)))
    return 0;
  byte_copy(x, len, in);

  if(*out)
    free(*out);
  *out = x;
  return len;
}
