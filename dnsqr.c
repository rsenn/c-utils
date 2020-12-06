#include <stdlib.h>
#include "lib/errmsg.h"
#include "lib/uint16.h"
#include "lib/buffer.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/iopause.h"
#include "lib/dns.h"
#include <errno.h>

#define FATAL "dnsqr: fatal: "

void
usage(void) {
  die(100, "dnsqr: usage: dnsqr type name", 0);
}
void
oops(void) {
  diesys(111, FATAL, "unable to parse: ", 0);
}

char type[2];
static char* q;

static stralloc out;

static char seed[128];

#define X(s)                                                                                                                                                                                                                                                                                                                                                                                                   \
  if(!stralloc_cats(out, s))                                                                                                                                                                                                                                                                                                                                                                                   \
    return 0;
#define NUM(u)                                                                                                                                                                                                                                                                                                                                                                                                 \
  if(!stralloc_catulong0(out, u, 0))                                                                                                                                                                                                                                                                                                                                                                           \
    return 0;

unsigned int
printpacket_cat(stralloc* out, char* buf, unsigned int len) {
  uint16 numqueries;
  uint16 numanswers;
  uint16 numauthority;
  uint16 numglue;
  unsigned int pos;
  char data[12];
  uint16 type;

  pos = dns_packet_copy(buf, len, 0, data, 12);
  if(!pos)
    return 0;

  uint16_unpack_big(data + 4, &numqueries);
  uint16_unpack_big(data + 6, &numanswers);
  uint16_unpack_big(data + 8, &numauthority);
  uint16_unpack_big(data + 10, &numglue);

  NUM(len)
  X(" bytes, ")
  NUM(numqueries)
  X("+")
  NUM(numanswers)
  X("+")
  NUM(numauthority)
  X("+")
  NUM(numglue)
  X(" records")

  if(data[2] & 128)
    X(", response")
  if(data[2] & 120)
    X(", weird op")
  if(data[2] & 4)
    X(", authoritative")
  if(data[2] & 2)
    X(", truncated")
  if(data[2] & 1)
    X(", weird rd")
  if(data[3] & 128)
    X(", weird ra")
  switch(data[3] & 15) {
    case 0: X(", noerror"); break;
    case 3: X(", nxdomain"); break;
    case 4: X(", notimp"); break;
    case 5: X(", refused"); break;
    default: X(", weird rcode");
  }
  if(data[3] & 112)
    X(", weird z")

  X("\n")

  while(numqueries) {
    --numqueries;
    X("query: ")

    pos = dns_packet_getname(buf, len, pos, &d);
    if(!pos)
      return 0;
    pos = dns_packet_copy(buf, len, pos, data, 4);
    if(!pos)
      return 0;

    if(byte_diff(data + 2, 2, DNS_C_IN)) {
      X("weird class")
    } else {
      uint16_unpack_big(data, &type);
      NUM(type)
      X(" ")
      if(!dns_domain_todot_cat(out, d))
        return 0;
    }
    X("\n")
  }

  for(;;) {
    if(numanswers) {
      --numanswers;
      X("answer: ")
    } else if(numauthority) {
      --numauthority;
      X("authority: ")
    } else if(numglue) {
      --numglue;
      X("additional: ")
    } else
      break;

    pos = printrecord_cat(out, buf, len, pos, 0, 0);
    if(!pos)
      return 0;
  }

  if(pos != len) {
    errno = error_proto;
    return 0;
  }
  return 1;
}

int
parsetype(char* s, char type[2]) {
  unsigned long u;

  if(!s[scan_ulong(s, &u)])
    uint16_pack_big(type, u);
  else if(case_equals(s, "any"))
    byte_copy(type, 2, DNS_T_ANY);
  else if(case_equals(s, "a"))
    byte_copy(type, 2, DNS_T_A);
  else if(case_equals(s, "ns"))
    byte_copy(type, 2, DNS_T_NS);
  else if(case_equals(s, "mx"))
    byte_copy(type, 2, DNS_T_MX);
  else if(case_equals(s, "ptr"))
    byte_copy(type, 2, DNS_T_PTR);
  else if(case_equals(s, "txt"))
    byte_copy(type, 2, DNS_T_TXT);
  else if(case_equals(s, "cname"))
    byte_copy(type, 2, DNS_T_CNAME);
  else if(case_equals(s, "soa"))
    byte_copy(type, 2, DNS_T_SOA);
  else if(case_equals(s, "hinfo"))
    byte_copy(type, 2, DNS_T_HINFO);
  else if(case_equals(s, "rp"))
    byte_copy(type, 2, DNS_T_RP);
  else if(case_equals(s, "sig"))
    byte_copy(type, 2, DNS_T_SIG);
  else if(case_equals(s, "key"))
    byte_copy(type, 2, DNS_T_KEY);
  else if(case_equals(s, "aaaa"))
    byte_copy(type, 2, DNS_T_AAAA);
  else if(case_equals(s, "axfr"))
    byte_copy(type, 2, DNS_T_AXFR);
  else
    return 0;

  return 1;
}

int
main(int argc, char** argv) {
  uint16 u16;

  dns_random_init(seed);

  if(!*argv)
    usage();
  if(!*++argv)
    usage();
  if(!parsetype(*argv, type))
    usage();

  if(!*++argv)
    usage();
  if(!dns_domain_fromdot(&q, *argv, str_len(*argv)))
    oops();

  if(*++argv)
    usage();

  if(!stralloc_copys(&out, ""))
    oops();
  uint16_unpack_big(type, &u16);
  if(!stralloc_catulong0(&out, u16, 0))
    oops();
  if(!stralloc_cats(&out, " "))
    oops();
  if(!dns_domain_todot_cat(&out, q))
    oops();
  if(!stralloc_cats(&out, ":\n"))
    oops();

  if(dns_resolve(q, type) == -1) {
    if(!stralloc_cats(&out, strerror(errno)))
      oops();
    if(!stralloc_cats(&out, "\n"))
      oops();
  } else {
    if(dns_resolve_tx.packetlen < 4)
      oops();
    dns_resolve_tx.packet[2] &= ~1;
    dns_resolve_tx.packet[3] &= ~128;
    if(!printpacket_cat(&out, dns_resolve_tx.packet, dns_resolve_tx.packetlen))
      oops();
  }

  buffer_putflush(buffer_1, out.s, out.len);
  exit(0);
}
