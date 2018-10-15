#include "lib/buffer.h"
#include "lib/dns.h"
#include "lib/errmsg.h"
#include "lib/ip4.h"

#define FATAL "dnsname: fatal: "

static char seed[128];

char ip[4];
static stralloc out;

int
main(int argc, char** argv) {
  dns_random_init(seed);

  if(*argv) ++argv;

  while(*argv) {
    if(!ip4_scan(*argv, ip)) {
      errmsg_warnsys("unable to parse IP address ", *argv, 0);
      return 111;
    }
    if(dns_name4(&out, ip) == -1) {
      errmsg_warnsys("unable to find host name for ", *argv, ": ", 0);
      return 111;
    }

    buffer_put(buffer_1, out.s, out.len);
    buffer_puts(buffer_1, "\n");

    ++argv;
  }

  buffer_flush(buffer_1);

  return 0;
}
