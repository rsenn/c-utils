#include <stdlib.h>
#include "lib/errmsg.h"
#include "lib/buffer.h"
#include "lib/ip4.h"
#include "lib/dns.h"
#include <errno.h>

#define FATAL "dnsipq: fatal: "

static char seed[128];

static stralloc in;
static stralloc fqdn;
static stralloc out;
char str[IP4_FMT];

int
main(int argc, char** argv) {
  size_t i;

  dns_random_init(seed);

  if(*argv)
    ++argv;

  while(*argv) {
    if(!stralloc_copys(&in, *argv))
      die(111, FATAL, "out of memory", 0);

    if(dns_ip4_qualify(&out, &fqdn, &in) == -1)
      diesys(111, FATAL, "unable to find IP              ddress for ", *argv, ": ", 0);

    buffer_put(buffer_1, fqdn.s, fqdn.len);
    buffer_puts(buffer_1, " ");

    for(i = 0; i + 4 <= out.len; i += 4) {
      buffer_put(buffer_1, str, ip4_fmt(str, out.s + i));
      buffer_puts(buffer_1, " ");
    }
    buffer_puts(buffer_1, "\n");

    ++argv;
  }

  buffer_flush(buffer_1);
  exit(0);
}
