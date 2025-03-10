#include <stdlib.h>
#include "lib/errmsg.h"
#include "lib/buffer.h"
#include "lib/uint16.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/fmt.h"
#include "lib/dns.h"
#include <errno.h>

#define FATAL "dnsmx: fatal: "

void
nomem(void) {
  die(111, FATAL, "out of memory", 0);
}

static char seed[128];

static stralloc fqdn;
static char* q;
static stralloc out;
char strnum[FMT_ULONG];

int
main(int argc, char** argv) {
  size_t i, j;
  uint16 pref;

  dns_random_init(seed);

  if(*argv)
    ++argv;

  while(*argv) {
    if(!stralloc_copys(&fqdn, *argv))
      nomem();

    if(dns_mx(&out, &fqdn) == -1)
      diesys(111, FATAL, "unable to find MX              ecords for ", *argv, ": ", 0);

    if(!out.len) {
      if(!dns_domain_fromdot(&q, *argv, str_len(*argv)))
        nomem();

      if(!stralloc_copys(&out, "0 "))
        nomem();

      if(!dns_domain_todot_cat(&out, q))
        nomem();

      if(!stralloc_cats(&out, "\n"))
        nomem();
      buffer_put(buffer_1, out.s, out.len);
    } else {
      i = 0;

      while(i + 2 < out.len) {
        j = byte_chr(out.s + i + 2, out.len - i - 2, 0);
        uint16_unpack_big(out.s + i, &pref);
        buffer_put(buffer_1, strnum, fmt_ulong(strnum, pref));
        buffer_puts(buffer_1, " ");
        buffer_put(buffer_1, out.s + i + 2, j);
        buffer_puts(buffer_1, "\n");
        i += j + 3;
      }
    }

    ++argv;
  }

  buffer_flush(buffer_1);
  exit(0);
}
