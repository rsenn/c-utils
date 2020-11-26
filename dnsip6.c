#include <stdlib.h>
#include "lib/buffer.h"
#include "lib/ip6.h"
#include "lib/dns.h"
#include "lib/errmsg.h"
#include <errno.h>

#define FATAL "dnsip: fatal: "

static char seed[128];

static stralloc fqdn;
static stralloc out;
char str[IP6_FMT];

int
main(int argc, char** argv) {
  size_t i;

  dns_random_init(seed);

  if(*argv)
    ++argv;

  while(*argv) {
    if(!stralloc_copys(&fqdn, *argv))
      die(111, FATAL, "out of memory", 0);
    if(dns_ip6(&out, &fqdn) == -1)
      diesys(111, FATAL, "unable to find IPv6 address for ", *argv, ": ", 0);

    for(i = 0; i + 16 <= out.len; i += 16) {
      buffer_put(buffer_1, str, fmt_ip6(str, out.s + i));
      buffer_puts(buffer_1, " ");
    }
    buffer_puts(buffer_1, "\n");

    ++argv;
  }

  buffer_flush(buffer_1);
  exit(0);
}
