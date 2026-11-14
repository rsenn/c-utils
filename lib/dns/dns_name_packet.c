#include "../byte.h"
#include "../dns.h"
#include "../ip6.h"
#include "../stralloc.h"
#include "../uint16.h"

static char* q = 0;

int
dns_name_packet(stralloc* out, const char* buf, unsigned int len) {
  unsigned int pos;
  char header[12];
  uint16 numanswers, datalen;

  if(!stralloc_copys(out, ""))
    return -1;

  if(!(pos = dns_packet_copy(buf, len, 0, header, 12)))
    return -1;

  uint16_unpack_big(header + 6, &numanswers);

  if(!(pos = dns_packet_skipname(buf, len, pos)))
    return -1;

  pos += 4;

  while(numanswers--) {
    if(!(pos = dns_packet_skipname(buf, len, pos)))
      return -1;

    if(!(pos = dns_packet_copy(buf, len, pos, header, 10)))
      return -1;

    uint16_unpack_big(header + 8, &datalen);

    if(byte_equal(header, 2, DNS_T_PTR))
      if(byte_equal(header + 2, 2, DNS_C_IN)) {
        if(!dns_packet_getname(buf, len, pos, &q))
          return -1;

        if(!dns_domain_todot_cat(out, q))
          return -1;

        return 0;
      }

    pos += datalen;
  }

  return 0;
}
