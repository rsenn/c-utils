#include "lib/buffer.h"
#include "lib/dns.h"
#include "lib/errmsg.h"
#include <errno.h>
#include <stdlib.h>

#define FATAL "dnstxt: fatal: "

static char seed[128];

static stralloc fqdn;
static stralloc out;

int
main(int argc, char** argv) {
  dns_random_init(seed);

  if(*argv)
    ++argv;

  while(*argv) {
    if(!stralloc_copys(&fqdn, *argv))
      die(111, FATAL, "out of memory", 0);

    if(dns_txt(&out, &fqdn) == -1)
      diesys(111,
             FATAL,
             "unable to find TXT "
             "records for ",
             *argv,
             ": ",
             0);

    buffer_put(buffer_1, out.s, out.len);
    buffer_puts(buffer_1, "\n");

    ++argv;
  }

  buffer_flush(buffer_1);
  exit(0);
}
