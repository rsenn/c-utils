#include "../lib/stralloc.h"
#include "unit_test.h"
#include "../lib/dns.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include "../lib/env.h"
#include "../lib/fmt.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 * dns_domain_fromdot, dns_domain_todot_cat, dns_domain_todot, dns_domain_length, dns_domain_copy,
 * dns_domain_copysa, dns_domain_free, dns_domain_equal, dns_domain_diff, dns_domain_suffix,
 * dns_domain_suffixpos
 */
TEST(test_dns_domain) {
  char *a = NULL, *b = NULL, *c = NULL, *copy = NULL;
  stralloc sa;
  char text[256];

  stralloc_init(&sa);
  ASSERT_EQ(1, dns_domain_fromdot(&a, "www.Example.org", 15) != 0);
  ASSERT_EQ(0, byte_diff(a, 17, "\3www\7Example\3org\0"));
  ASSERT_EQ(17, dns_domain_length(a));
  ASSERT_EQ(1, dns_domain_fromdot(&b, "WWW.EXAMPLE.ORG.", 16) != 0); /* trailing dot is the same */
  ASSERT_EQ(1, dns_domain_equal(a, b));                              /* case does not matter */
  ASSERT_EQ(0, dns_domain_diff(a, b));
  dns_domain_fromdot(&c, "example.org", 11);
  ASSERT_EQ(0, dns_domain_equal(a, c));
  ASSERT_EQ(1, dns_domain_diff(a, c) != 0);
  ASSERT_EQ(1, dns_domain_suffix(a, c));
  ASSERT_EQ(0, dns_domain_suffix(c, a));
  ASSERT_EQ(4, dns_domain_suffixpos(a, c));
  ASSERT_EQ(0, dns_domain_suffixpos(a, a));

  ASSERT_EQ(17, dns_domain_copy(&copy, a));
  ASSERT_EQ(1, copy != a);
  ASSERT_EQ(1, dns_domain_equal(copy, a));
  ASSERT_EQ(17, dns_domain_copysa(&sa, a));
  ASSERT_EQ(17, sa.len);

  stralloc_zero(&sa);
  ASSERT_EQ(1, dns_domain_todot_cat(&sa, a));
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "www.example.org."));
  ASSERT_EQ(15, dns_domain_todot(text, a));             /* no dot at the end */
  ASSERT_EQ(0, byte_diff(text, 15, "www.Example.org")); /* keeps the case, unlike todot_cat */

  stralloc_zero(&sa);
  ASSERT_EQ(1, dns_domain_todot_cat(&sa, "\0")); /* the root */
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "."));
  stralloc_zero(&sa);
  dns_domain_todot_cat(&sa, "\3a b\0"); /* a character that is not a name character */
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "a\\040b."));

  dns_domain_free(&a);
  dns_domain_free(&b);
  dns_domain_free(&c);
  dns_domain_free(&copy);
  ASSERT_EQ(NULL, a);
  stralloc_free(&sa);
}

/*
 * dns_packet_copy, dns_packet_skipname, dns_packet_getname
 */
TEST(test_dns_packet) {
  /* a name at 0, a compressed name pointing back to it at 17 */
  static const char pkt[] = "\3www\7example\3org\0\3ftp\300\4end";
  char out[8], *name = NULL;
  unsigned int pos;

  ASSERT_EQ(4, dns_packet_copy(pkt, sizeof(pkt) - 1, 0, out, 4));
  ASSERT_EQ(0, byte_diff(out, 4, "\3www"));
  ASSERT_EQ(0, dns_packet_copy(pkt, 6, 4, out, 4)); /* beyond the end */

  ASSERT_EQ(17, dns_packet_skipname(pkt, sizeof(pkt) - 1, 0));
  ASSERT_EQ(23, dns_packet_skipname(pkt, sizeof(pkt) - 1, 17)); /* a pointer ends the name */
  ASSERT_EQ(0, dns_packet_skipname(pkt, 5, 0));                 /* runs off the packet */

  pos = dns_packet_getname(pkt, sizeof(pkt) - 1, 17, &name);
  ASSERT_EQ(23, pos);
  ASSERT_EQ(1, name != NULL);
  ASSERT_EQ(0, byte_diff(name, 17, "\3ftp\7example\3org\0")); /* the pointer jumps into the first name */
  dns_domain_free(&name);

  /* a pointer loop is refused */
  {
    static const char loop[] = "\300\0";
    ASSERT_EQ(0, dns_packet_getname(loop, 2, 0, &name));
  }
}

/*
 * dns_name4_domain, dns_name6_domain, dns_fmt_server, dns_timeout, dns_random, dns_sortip, dns_sortip6
 */
TEST(test_dns_small_helpers) {
  char d4[DNS_NAME4_DOMAIN], d6[DNS_NAME6_DOMAIN], srv[DNS_FMT_SERVER];
  char ip6[16];
  struct taia t;
  char list[16], sorted[16];
  unsigned int i, seen[4] = {0, 0, 0, 0}, any = 0;

  dns_name4_domain(d4, "\1\2\3\4");
  ASSERT_EQ(1, dns_domain_length(d4) <= DNS_NAME4_DOMAIN);
  ASSERT_EQ(0, byte_diff(d4, 8, "\1\x34\1\x33\1\x32\1\x31"));

  byte_zero(ip6, 16);
  ip6[0] = 0x20;
  ip6[1] = 0x01;
  ip6[15] = 1;
  dns_name6_domain(d6, ip6);
  ASSERT_EQ(1, dns_domain_length(d6) <= DNS_NAME6_DOMAIN);
  ASSERT_EQ(0, byte_diff(d6, 4, "\1\x31\1\x30")); /* nibbles, last first */

  i = dns_fmt_server(srv, "\0\0\0\0\0\0\0\0\0\0\377\377\177\0\0\1", 16);
  srv[i] = 0;
  ASSERT_EQ(1, str_contains(srv, "127.0.0.1"));

  dns_timeout_ms = 0;
  dns_timeout(&t, 3);
  {
    struct taia three, one;

    taia_uint(&three, 3);
    taia_uint(&one, 1);
    ASSERT_EQ(three.sec.x, t.sec.x);
    ASSERT_EQ(0, t.nano);
    dns_timeout_ms = 1500;
    dns_timeout(&t, 3);
    ASSERT_EQ(one.sec.x, t.sec.x);
    ASSERT_EQ(500000000, t.nano);
  }
  dns_timeout_ms = 0;

  for(i = 0; i < 200; ++i) {
    unsigned int r = dns_random(4);
    ASSERT_EQ(1, r < 4);
    seen[r] = 1;
  }
  ASSERT_EQ(1, seen[0] + seen[1] + seen[2] + seen[3] >= 3); /* spread over the range */
  ASSERT_EQ(0, dns_random(0));

  /* sorting keeps the addresses, only the order changes */
  for(i = 0; i < 4; ++i)
    memset(list + 4 * i, (int)i + 1, 4);
  for(i = 0; i < 20; ++i) {
    unsigned int k, bits = 0;
    byte_copy(sorted, 16, list);
    dns_sortip(sorted, 16);
    for(k = 0; k < 4; ++k)
      bits |= 1u << (sorted[4 * k] - 1);
    ASSERT_EQ(15, bits);
    if(byte_diff(sorted, 16, list))
      any = 1;
  }
  ASSERT_EQ(1, any);
  {
    char l6[32];

    memset(l6, 1, 16);
    memset(l6 + 16, 2, 16);
    dns_sortip6(l6, 32);
    ASSERT_EQ(3, l6[0] + l6[16]);
  }
}

/* ------------------------------------------------------------------------ */

/* a resolver on the loopback interface that answers A, AAAA, TXT, MX and PTR */
static unsigned short
dns_server(int* pid_out) {
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in sa;
  socklen_t sl = sizeof sa;
  int pid;

  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if(s < 0 || bind(s, (struct sockaddr*)&sa, sizeof sa) < 0 || getsockname(s, (struct sockaddr*)&sa, &sl) < 0)
    return 0;
  pid = fork();
  if(pid == 0) {
    prctl(PR_SET_PDEATHSIG, SIGKILL); /* never outlive the test */
    alarm(60);
    for(;;) {
      unsigned char q[512], r[512];
      struct sockaddr_in from;
      socklen_t fl = sizeof from;
      ssize_t n = recvfrom(s, q, sizeof q, 0, (struct sockaddr*)&from, &fl);
      size_t qend = 12, rdlen = 0, i;
      const unsigned char* rdata = (const unsigned char*)"";
      unsigned type;

      if(n < 17)
        continue;
      while(qend < (size_t)n && q[qend])
        qend += q[qend] + 1u;
      qend += 5;
      type = q[qend - 4] << 8 | q[qend - 3];
      memcpy(r, q, qend);
      r[2] = 0x81;
      r[3] = 0x80;
      r[6] = 0;
      r[7] = 1;
      r[8] = r[9] = r[10] = r[11] = 0;
      switch(type) {
        case 1: rdata = (const unsigned char*)"\300\0\2\1", rdlen = 4; break;
        case 28: rdata = (const unsigned char*)"\x20\x01\x0d\xb8\0\0\0\0\0\0\0\0\0\0\0\1", rdlen = 16; break;
        case 16: rdata = (const unsigned char*)"\5hello", rdlen = 6; break;
        case 15: rdata = (const unsigned char*)"\0\12\2mx\4test\0", rdlen = 11; break;
        case 12: rdata = (const unsigned char*)"\3ptr\4test", rdlen = 10; break;
      }
      i = qend;
      r[i++] = 0xc0;
      r[i++] = 0x0c;
      r[i++] = (unsigned char)(type >> 8);
      r[i++] = (unsigned char)type;
      r[i++] = 0;
      r[i++] = 1;
      r[i++] = 0; /* ttl 60 */
      r[i++] = 0;
      r[i++] = 0;
      r[i++] = 60;
      r[i++] = (unsigned char)(rdlen >> 8);
      r[i++] = (unsigned char)rdlen;
      memcpy(r + i, rdata, rdlen);
      i += rdlen;
      sendto(s, r, i, 0, (struct sockaddr*)&from, fl);
    }
  }
  close(s);
  *pid_out = pid;
  return ntohs(sa.sin_port);
}

static void
dns_point_at(unsigned short port) {
  char num[FMT_ULONG];

  num[fmt_ulong(num, port)] = 0;
  setenv("DNSCACHEIP", "127.0.0.1", 1);
  setenv("DNSCACHEPORT", num, 1);
  dns_resolvconfip_reset();
  dns_family = 4;
  dns_timeout_ms = 1000;
}

TEST(test_dns_queries_loopback) {
  int pid = 0, st;
  unsigned short port = dns_server(&pid);
  stralloc out, name;
  char ip[16];

  ASSERT_EQ(1, port != 0);
  dns_point_at(port);
  stralloc_init(&out);
  stralloc_init(&name);

  stralloc_copys(&name, "host.test");
  ASSERT_EQ(0, dns_ip4(&out, &name));
  ASSERT_EQ(4, out.len);
  ASSERT_EQ(0, byte_diff(out.s, 4, "\300\0\2\1"));

  ASSERT_EQ(0, dns_ip6(&out, &name));
  ASSERT_EQ(1, out.len >= 16 && out.len % 16 == 0);
  {
    size_t k;
    int found = 0;

    for(k = 0; k + 16 <= out.len; k += 16)
      if(!byte_diff(out.s + k, 16, "\x20\x01\x0d\xb8\0\0\0\0\0\0\0\0\0\0\0\1"))
        found = 1;
    ASSERT_EQ(1, found);
  }

  ASSERT_EQ(0, dns_txt(&out, &name));
  ASSERT_EQ(1, str_contains(out.s, "hello") || byte_finds(out.s, out.len, "hello") < out.len);

  ASSERT_EQ(0, dns_mx(&out, &name));
  ASSERT_EQ(1, out.len > 2);
  ASSERT_EQ(1, byte_finds(out.s, out.len, "mx") < out.len);

  ip[0] = 10;
  ip[1] = 0;
  ip[2] = 0;
  ip[3] = 1;
  ASSERT_EQ(0, dns_name4(&out, ip));
  ASSERT_EQ(1, byte_finds(out.s, out.len, "ptr") < out.len);

  stralloc_free(&out);
  stralloc_free(&name);
  kill(pid, SIGKILL);
  waitpid(pid, &st, 0);
  unsetenv("DNSCACHEIP");
  unsetenv("DNSCACHEPORT");
  dns_resolvconfip_reset();
  dns_family = 0;
  dns_timeout_ms = 0;
}

/*
 * dns_ip4_packet, dns_mx_packet, dns_txt_packet, dns_name_packet on a hand made answer
 */
TEST(test_dns_packet_parsers) {
  /* header: id, flags, 1 question, 2 answers; question: a.test A IN; answers: two A records */
  static const unsigned char pkt[] = {0x12, 0x34, 0x81, 0x80, 0,   1,  0, 2,  0, 0, 0,    0,  1,    'a',
                                      4,    't',  'e',  's',  't', 0,  0, 1,  0, 1, 0xc0, 12, 0,    1,
                                      0,    1,    0,    0,    0,   60, 0, 4,  1, 2, 3,    4,  0xc0, 12,
                                      0,    1,    0,    1,    0,   0,  0, 60, 0, 4, 5,    6,  7,    8};
  stralloc out;

  stralloc_init(&out);
  ASSERT_EQ(0, dns_ip4_packet(&out, (const char*)pkt, sizeof pkt));
  ASSERT_EQ(8, out.len);
  ASSERT_EQ(1,
            byte_finds(out.s, 8, "\1\2\3\4") < 8 || byte_diff(out.s, 4, "\1\2\3\4") == 0 ||
                byte_diff(out.s + 4, 4, "\1\2\3\4") == 0);

  ASSERT_EQ(-1, dns_ip4_packet(&out, (const char*)pkt, 20));        /* truncated */
  ASSERT_EQ(0, dns_txt_packet(&out, (const char*)pkt, sizeof pkt)); /* no TXT records in it */
  ASSERT_EQ(0, out.len);
  ASSERT_EQ(0, dns_mx_packet(&out, (const char*)pkt, sizeof pkt));
  ASSERT_EQ(0, out.len);
  ASSERT_EQ(0, dns_name_packet(&out, (const char*)pkt, sizeof pkt));
  ASSERT_EQ(0, out.len);
  stralloc_free(&out);
}

/*
 * dns_ip6_packet, dns_domain_todot_inline, dns_domain_todot_utf8_cat, dns_random_init
 */
TEST(test_dns_ip6_todot_variants) {
  static const unsigned char pkt[] = {0x12, 0x34, 0x81, 0x80, 0, 1, 0,    1,  0, 0,  0, 0, 1, 'a', 4, 't', 'e', 's',
                                      't',  0,    0,    28,   0, 1, 0xc0, 12, 0, 28, 0, 1, 0, 0,   0, 60,  0,   16,
                                      0x20, 0x01, 0x0d, 0xb8, 0, 0, 0,    0,  0, 0,  0, 0, 0, 0,   0, 1};
  stralloc out;
  char name[64];
  char seed[128];

  stralloc_init(&out);
  ASSERT_EQ(0, dns_ip6_packet(&out, (const char*)pkt, sizeof pkt));
  ASSERT_EQ(16, out.len);
  ASSERT_EQ(0, byte_diff(out.s, 16, "\x20\x01\x0d\xb8\0\0\0\0\0\0\0\0\0\0\0\1"));
  ASSERT_EQ(-1, dns_ip6_packet(&out, (const char*)pkt, 30)); /* truncated */

  /* the in place conversion turns the wire format of a name into text at the same address */
  byte_copy(name, 17, "\3www\7example\3org\0");
  ASSERT_EQ(16, dns_domain_todot_inline(name)); /* every length byte becomes a dot: ".www.example.org" */
  ASSERT_EQ(0, byte_diff(name, 16, ".www.example.org"));

  stralloc_zero(&out);
  ASSERT_EQ(1, dns_domain_todot_utf8_cat(&out, "\3www\7example\3org\0"));
  ASSERT_EQ(1, str_contains(out.s ? (stralloc_nul(&out), out.s) : "", "www.example.org"));
  stralloc_free(&out);

  byte_zero(seed, sizeof seed);
  dns_random_init(seed);
  ASSERT_EQ(1, dns_random(1000) < 1000);
}

/*
 * dns_resolvconfip, dns_resolvconfrewrite, dns_ip4_qualify(_rules), dns_ip6_qualify, dns_name6,
 * dns_resolve, dns_transmit_start/io/get/free, against the loopback resolver
 */
TEST(test_dns_lowlevel_loopback) {
  int pid = 0, st;
  unsigned short port = dns_server(&pid);
  stralloc out, fqdn, in, rules;
  char servers[256], ip6[16];
  char* q = NULL;
  const char* rw = "/tmp/c-utils-dnsrewrite.tmp";
  int fd, k;

  ASSERT_EQ(1, port != 0);
  dns_point_at(port);
  stralloc_init(&out);
  stralloc_init(&fqdn);
  stralloc_init(&in);
  stralloc_init(&rules);

  /* the server list comes from DNSCACHEIP: one IPv4 address, in the v4-mapped form */
  ASSERT_EQ(0, dns_resolvconfip(servers));
  ASSERT_EQ(0, byte_diff(servers, 16, "\0\0\0\0\0\0\0\0\0\0\377\377\177\0\0\1"));
  ASSERT_EQ(0, servers[16]);
  ASSERT_EQ(port, dns_port);

  /* rewrite rules from a file */
  fd = open(rw, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  write(fd, "?:+.test\n", 9);
  close(fd);
  setenv("DNSREWRITEFILE", rw, 1);
  ASSERT_EQ(0, dns_resolvconfrewrite(&rules));
  ASSERT_EQ(1, rules.len > 0);

  stralloc_copys(&in, "host");
  ASSERT_EQ(0, dns_ip4_qualify(&out, &fqdn, &in));
  ASSERT_EQ(4, out.len);
  ASSERT_EQ(1, fqdn.len >= 4);
  ASSERT_EQ(0, dns_ip4_qualify_rules(&out, &fqdn, &in, &rules));
  ASSERT_EQ(4, out.len);
  ASSERT_EQ(0, dns_ip6_qualify(&out, &fqdn, &in));
  ASSERT_EQ(1, out.len >= 16);
  ASSERT_EQ(0, dns_ip6_qualify_rules(&out, &fqdn, &in, &rules));
  ASSERT_EQ(1, out.len >= 16);

  byte_zero(ip6, 16);
  ip6[15] = 1;
  ASSERT_EQ(0, dns_name6(&out, ip6));
  ASSERT_EQ(1, byte_finds(out.s, out.len, "ptr") < out.len);

  /* the single steps of a query */
  dns_domain_fromdot(&q, "host.test", 9);
  ASSERT_EQ(0, dns_resolve(q, DNS_T_A));
  ASSERT_EQ(1, dns_resolve_tx.packetlen > 12);
  dns_transmit_free(&dns_resolve_tx);
  ASSERT_EQ(NULL, dns_resolve_tx.packet);

  {
    struct dns_transmit tx;
    struct taia deadline, stamp;
    iopause_fd x[1];
    int r = 0;

    byte_zero(&tx, sizeof tx);
    ASSERT_EQ(0, dns_transmit_start(&tx, servers, 1, q, DNS_T_A, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"));
    for(k = 0; k < 200 && r == 0; ++k) {
      taia_now(&stamp);
      taia_uint(&deadline, 5);
      taia_add(&deadline, &deadline, &stamp);
      dns_transmit_io(&tx, x, &deadline);
      iopause(x, 1, &deadline, &stamp);
      r = dns_transmit_get(&tx, x, &stamp);
      ASSERT_EQ(1, r != -1);
    }
    ASSERT_EQ(1, r);
    ASSERT_EQ(0, dns_ip4_packet(&out, tx.packet, tx.packetlen));
    ASSERT_EQ(4, out.len);
    dns_transmit_free(&tx);
  }

  dns_domain_free(&q);
  unlink(rw);
  unsetenv("DNSREWRITEFILE");
  stralloc_free(&out);
  stralloc_free(&fqdn);
  stralloc_free(&in);
  stralloc_free(&rules);
  kill(pid, SIGKILL);
  waitpid(pid, &st, 0);
  unsetenv("DNSCACHEIP");
  unsetenv("DNSCACHEPORT");
  dns_resolvconfip_reset();
  dns_family = 0;
  dns_timeout_ms = 0;
}

#define RUN_DNS_TESTS() \
  RUN(test_dns_domain); \
  RUN(test_dns_packet); \
  RUN(test_dns_small_helpers); \
  RUN(test_dns_packet_parsers); \
  RUN(test_dns_queries_loopback); \
  RUN(test_dns_ip6_todot_variants); \
  RUN(test_dns_lowlevel_loopback)

TESTS(dns) { RUN_DNS_TESTS(); }
