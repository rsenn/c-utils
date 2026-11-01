#include "../byte.h"
#include "../case.h"
#include "../dns.h"
#include "../str.h"
#include "../stralloc.h"

static int
doit(stralloc* work, const char* rule) {
  char ch;
  unsigned int colon;
  unsigned int prefixlen;

  ch = *rule++;

  if((ch != '?') && (ch != '=') && (ch != '*') && (ch != '-'))
    return 1;
  colon = str_chr(rule, ':');

  if(!rule[colon])
    return 1;

  if(work->len < colon)
    return 1;
  prefixlen = work->len - colon;

  if((ch == '=') && prefixlen)
    return 1;

  if(case_diffb(rule, colon, work->s + prefixlen))
    return 1;

  if(ch == '?') {
    if(byte_chr(work->s, prefixlen, '.') < prefixlen)
      return 1;

    if(byte_chr(work->s, prefixlen, '[') < prefixlen)
      return 1;

    if(byte_chr(work->s, prefixlen, ']') < prefixlen)
      return 1;
  }

  work->len = prefixlen;

  if(ch == '-')
    work->len = 0;
  return stralloc_cats(work, rule + colon + 1);
}

int
dns_ip4_qualify(stralloc* out, stralloc* fqdn, const stralloc* in) {
  static stralloc rules;

  if(dns_resolvconfrewrite(&rules) == -1)
    return -1;
  return dns_ip4_qualify_rules(out, fqdn, in, &rules);
}
