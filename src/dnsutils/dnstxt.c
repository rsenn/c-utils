#include "dnsopt.h"
#include "../../lib/buffer.h"
#include "../../lib/dns.h"
#include "../../lib/errmsg.h"
#include "../../lib/stralloc.h"

static char seed[128];
static stralloc fqdn;
static stralloc out;

static int
lookup(const char* name) {
  if(!stralloc_copys(&fqdn, name))
    die(111, "out of memory", 0);

  if(dns_txt(&out, &fqdn) == -1) {
    dnsopt_warn("unable to find TXT records for ", name);
    return -1;
  }

  dnsopt_prefix(buffer_1);
  buffer_put(buffer_1, out.s, out.len);
  buffer_puts(buffer_1, "\n");
  return 0;
}

int
main(int argc, char** argv) {
  errmsg_iam("dnstxt");
  dns_random_init(seed);

  argv += dnsopt_parse(argc, argv, "name...");
  return dnsopt_run(argv, lookup);
}
