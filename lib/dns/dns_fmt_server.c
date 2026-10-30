#include "../dns.h"
#include "../fmt.h"
#include "../ip4.h"
#include "../ip6.h"

/* dns_fmt_server writes ip:port, with brackets around an IPv6 address, and
 * returns the length. iplen is 4 or 16, dest has room for DNS_FMT_SERVER. */
size_t
dns_fmt_server(char* dest, const char* ip, unsigned int iplen) {
  size_t n = 0;
  int v6 = iplen == 16 && !ip6_isv4mapped(ip);

  if(v6)
    dest[n++] = '[';
  n += v6 ? fmt_ip6(dest + n, ip) : fmt_ip4(dest + n, iplen == 16 ? ip + 12 : ip);
  if(v6)
    dest[n++] = ']';
  dest[n++] = ':';
  return n + fmt_ulong(dest + n, dns_port);
}
