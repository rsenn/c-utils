#include "../buffer.h"
#include "../dns.h"
#include "../stralloc.h"
#include "../uint16.h"

static const char* const rcodes[] = {"NOERROR", "FORMERR", "SERVFAIL", "NXDOMAIN", "NOTIMP", "REFUSED"};

static const struct {
  const char* name;
  const char* type;
} types[] = {{"A", DNS_T_A},
             {"NS", DNS_T_NS},
             {"CNAME", DNS_T_CNAME},
             {"SOA", DNS_T_SOA},
             {"PTR", DNS_T_PTR},
             {"HINFO", DNS_T_HINFO},
             {"MX", DNS_T_MX},
             {"TXT", DNS_T_TXT},
             {"RP", DNS_T_RP},
             {"SIG", DNS_T_SIG},
             {"KEY", DNS_T_KEY},
             {"AAAA", DNS_T_AAAA},
             {"AXFR", DNS_T_AXFR},
             {"ANY", DNS_T_ANY}};

/* dns_trace describes one packet on stderr if dns_verbose is set, and dumps
 * it if dns_debug is. dir is "send" or "recv", tcp selects the transport,
 * ip is the server address of length iplen, pkt is the DNS message. */
void
dns_trace(const char* dir, int tcp, const char* ip, unsigned int iplen, const char* pkt, unsigned int len) {
  char server[DNS_FMT_SERVER], header[12];
  stralloc name = {0, 0, 0};
  char* d = 0;
  uint16 id, v;
  unsigned int pos, i;
  const char* tname = 0;

  if(!dns_verbose && !dns_debug)
    return;

  buffer_puts(buffer_2, "dns: ");
  buffer_puts(buffer_2, dir);
  buffer_puts(buffer_2, tcp ? " tcp " : " udp ");
  buffer_put(buffer_2, server, dns_fmt_server(server, ip, iplen));

  if(dns_packet_copy(pkt, len, 0, header, 12)) {
    uint16_unpack_big(header, &id);
    buffer_puts(buffer_2, " id=0x");
    buffer_putxlong0(buffer_2, id, 4);

    if(header[2] & 0x80) {
      buffer_puts(buffer_2, " reply ");
      buffer_puts(buffer_2, (header[3] & 15) < 6 ? rcodes[header[3] & 15] : "rcode?");
      uint16_unpack_big(header + 6, &v);
      buffer_puts(buffer_2, " an=");
      buffer_putulong(buffer_2, v);
      uint16_unpack_big(header + 8, &v);
      buffer_puts(buffer_2, " ns=");
      buffer_putulong(buffer_2, v);
      uint16_unpack_big(header + 10, &v);
      buffer_puts(buffer_2, " ar=");
      buffer_putulong(buffer_2, v);
      if(header[2] & 2)
        buffer_puts(buffer_2, " truncated");
    } else
      buffer_puts(buffer_2, " query");

    uint16_unpack_big(header + 4, &v);

    if(v && (pos = dns_packet_getname(pkt, len, 12, &d)) && dns_packet_copy(pkt, len, pos, header, 4)) {
      for(i = 0; i < sizeof types / sizeof types[0]; ++i)
        if(header[0] == types[i].type[0] && header[1] == types[i].type[1])
          tname = types[i].name;
      buffer_puts(buffer_2, " ");
      buffer_puts(buffer_2, tname ? tname : "TYPE?");
      buffer_puts(buffer_2, " ");
      if(dns_domain_todot_utf8_cat(&name, d))
        buffer_put(buffer_2, name.s, name.len);
    }
    dns_domain_free(&d);
    stralloc_free(&name);
  }
  buffer_puts(buffer_2, " (");
  buffer_putulong(buffer_2, len);
  buffer_puts(buffer_2, " bytes)\n");
  buffer_flush(buffer_2);

#ifdef DEBUG_OUTPUT
  dns_debug_hex(dir, pkt, len);
#endif
}
