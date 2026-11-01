#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include <stdlib.h>

void
dns_domain_free(char** out) {
  if(*out) {
    free(*out);
    *out = 0;
  }
}
