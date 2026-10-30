#include "dnsopt.h"
#include "../../lib/buffer.h"
#include "../../lib/dns.h"
#include "../../lib/errmsg.h"
#include "../../lib/ip6.h"
#include "../../lib/stralloc.h"

static char seed[128];
static stralloc fqdn;
static stralloc out;

static int
lookup(const char* name) {
  char str[IP6_FMT];
  size_t i;

  if(!stralloc_copys(&fqdn, name))
    die(111, "out of memory", 0);

  if(dns_ip6(&out, &fqdn) == -1) {
    dnsopt_warn("unable to find IPv6 address for ", name);
    return -1;
  }

  dnsopt_prefix(buffer_1);

  for(i = 0; i + 16 <= out.len; i += 16) {
    buffer_put(buffer_1, str, fmt_ip6(str, out.s + i));
    buffer_puts(buffer_1, " ");
  }
  buffer_puts(buffer_1, "\n");
  return 0;
}

int
main(int argc, char** argv) {
  errmsg_iam("dnsip6");
  dns_random_init(seed);

  argv += dnsopt_parse(argc, argv, "name...");
  return dnsopt_run(argv, lookup);
}
