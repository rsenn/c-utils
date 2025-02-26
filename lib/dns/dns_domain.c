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

void
dns_domain_free(char** out) {
  if(*out) {
    free(*out);
    *out = 0;
  }
}

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

int
dns_domain_copysa(stralloc* sa, const char* in) {
  unsigned int len;

  len = dns_domain_length(in);
  stralloc_copyb(sa, in, len);
  return len;
}

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
