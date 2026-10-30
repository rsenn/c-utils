#include "dnsopt.h"
#include "../../lib/buffer.h"
#include "../../lib/byte.h"
#include "../../lib/dns.h"
#include "../../lib/errmsg.h"
#include "../../lib/ip4.h"
#include "../../lib/ip6.h"
#include "../../lib/stralloc.h"

static char seed[128];
static stralloc out;

static int
lookup(const char* arg) {
  char ip[16];

  byte_copy(ip, 12, V4mappedprefix);

  if(!scan_ip4(arg, &ip[12]) && !scan_ip6(arg, ip)) {
    dnsopt_warn("unable to parse IP address ", arg);
    return -1;
  }

  if(dns_name6(&out, ip) == -1) {
    dnsopt_warn("unable to find host name for ", arg);
    return -1;
  }

  dnsopt_prefix(buffer_1);
  buffer_put(buffer_1, out.s, out.len);
  buffer_puts(buffer_1, "\n");
  return 0;
}

int
main(int argc, char** argv) {
  errmsg_iam("dnsname");
  dns_random_init(seed);

  argv += dnsopt_parse(argc, argv, "ip...");
  return dnsopt_run(argv, lookup);
}
