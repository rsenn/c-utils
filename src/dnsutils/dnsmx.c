#include "dnsopt.h"
#include "../../lib/buffer.h"
#include "../../lib/byte.h"
#include "../../lib/dns.h"
#include "../../lib/errmsg.h"
#include "../../lib/fmt.h"
#include "../../lib/str.h"
#include "../../lib/uint16.h"

static char seed[128];
static stralloc fqdn;
static char* q;
static stralloc out;

static int
lookup(const char* name) {
  char strnum[FMT_ULONG];
  size_t i, j;
  uint16 pref;

  if(!stralloc_copys(&fqdn, name))
    die(111, "out of memory", 0);

  if(dns_mx(&out, &fqdn) == -1) {
    dnsopt_warn("unable to find MX records for ", name);
    return -1;
  }

  if(!out.len) {
    if(!dns_domain_fromdot(&q, name, str_len(name)))
      die(111, "out of memory", 0);

    if(!stralloc_copys(&out, "0 ") || !dns_domain_todot_utf8_cat(&out, q))
      die(111, "out of memory", 0);

    dnsopt_prefix(buffer_1);
    buffer_put(buffer_1, out.s, out.len);
    buffer_puts(buffer_1, "\n");
    return 0;
  }

  for(i = 0; i + 2 < out.len; i += j + 3) {
    j = byte_chr(out.s + i + 2, out.len - i - 2, 0);
    uint16_unpack_big(out.s + i, &pref);
    dnsopt_prefix(buffer_1);
    buffer_put(buffer_1, strnum, fmt_ulong(strnum, pref));
    buffer_puts(buffer_1, " ");
    buffer_put(buffer_1, out.s + i + 2, j);
    buffer_puts(buffer_1, "\n");
  }
  return 0;
}

int
main(int argc, char** argv) {
  errmsg_iam("dnsmx");
  dns_random_init(seed);

  argv += dnsopt_parse(argc, argv, "name...");
  return dnsopt_run(argv, lookup);
}
