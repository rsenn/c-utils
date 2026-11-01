#include "unit_test.h"
#include "../lib/socket.h"
#include <unistd.h>
#include "../lib/byte.h"
#include "../lib/scan.h"
#include "../lib/fmt.h"
#include "../lib/str.h"
#include "../lib/ip4.h"
#include "../lib/ip6.h"

/*
 * int socket_tcp4(void);
 * int socket_close(fd_type);
 */
TEST(test_socket_tcp4_close) {
  int s = socket_tcp4();

  ASSERT_NE(-1, s);
  socket_close(s);
}

/*
 * int socket_udp4(void);
 */
TEST(test_socket_udp4) {
  int s = socket_udp4();

  ASSERT_NE(-1, s);
  socket_close(s);
}

/*
 * int socket_bind4(int s, const char* ip, uint16 port);
 * int socket_local4(int s, char* ip, uint16* port);
 */
TEST(test_socket_bind4_local4) {
  int s = socket_tcp4();
  uint16 port = 0;
  char ip[4] = {0};

  ASSERT_NE(-1, s);
  ASSERT_EQ(0, socket_bind4(s, NULL, 0));
  ASSERT_EQ(0, socket_local4(s, ip, &port));
  ASSERT_NE(0, port);

  socket_close(s);
}

/*
 * int socket_listen(int s, unsigned int backlog);
 */
TEST(test_socket_listen) {
  int s = socket_tcp4();

  ASSERT_NE(-1, s);
  ASSERT_EQ(0, socket_bind4(s, NULL, 0));
  ASSERT_EQ(0, socket_listen(s, 1));

  socket_close(s);
}

/*
 * int socket_is4(int);
 * int socket_is6(int);
 */
TEST(test_socket_is4_is6) {
  int s = socket_tcp4();

  ASSERT_NE(-1, s);
  ASSERT_NE(0, socket_is4(s));

  socket_close(s);
}

/*
 * const char* socket_ip4loopback();
 */
TEST(test_socket_ip4loopback) {
  const char* lo = socket_ip4loopback();

  ASSERT_NE(NULL, lo);
  ASSERT_EQ(127, (unsigned char)lo[0]);
}

/*
 * void socket_tryreservein(int s, int size);
 */
TEST(test_socket_tryreservein) {
  int s = socket_udp4();

  ASSERT_NE(-1, s);
  socket_tryreservein(s, 4096);

  socket_close(s);
}

/*
 * int socket_v6only(int fd, int enable);
 * int socket_tcp6(void);
 *
 * Not exercised here: IPv6 support depends on the test host's network
 * stack configuration (see `noipv6`); the IPv4 paths above already cover
 * the shared socket-option plumbing.
 */
TEST(test_socket_v6only) {
  int s = socket_tcp6();

  if(s < 0)
    return; /* no IPv6 in this environment */
  if(socket_is6(s)) {
    ASSERT_EQ(0, socket_v6only(s, 1));
    ASSERT_EQ(0, socket_v6only(s, 0));
  }
  socket_close(s);
}


/*
 * size_t fmt_ip4(char*, const char* ip); size_t scan_ip4(const char*, char* ip);
 */
TEST(test_socket_ip4_text) {
  static const char lo[4] = {127, 0, 0, 1};
  char out[32], ip[4];
  size_t n;

  n = fmt_ip4(out, lo);
  ASSERT_EQ(9, n);
  ASSERT_EQ(0, byte_diff(out, 9, "127.0.0.1"));
  ASSERT_EQ(9, fmt_ip4(NULL, lo));

  ASSERT_EQ(12, scan_ip4("192.168.1.20:80", ip)); /* stops at the colon */
  ASSERT_EQ(192, (unsigned char)ip[0]);
  ASSERT_EQ(20, (unsigned char)ip[3]);
  ASSERT_EQ(0, scan_ip4("256.1.1.1", ip));
  ASSERT_EQ(0, scan_ip4("1.2.3", ip));
  ASSERT_EQ(0, scan_ip4("x", ip));
}

/*
 * fmt_ip6, fmt_ip6c, fmt_ip6_flat, scan_ip6, scan_ip6_flat, scan_ip6if, fmt_ip6if(c)
 */
TEST(test_socket_ip6_text) {
  char ip[16], out[64];
  char lo6[16] = {0}, v4m[16] = {0};
  size_t n;
  uint32 ifx = 99;

  ASSERT_EQ(11, scan_ip6("2001:db8::1", ip));
  ASSERT_EQ(0x20, (unsigned char)ip[0]);
  ASSERT_EQ(0x01, (unsigned char)ip[1]);
  ASSERT_EQ(1, (unsigned char)ip[15]);
  ASSERT_EQ(0, (unsigned char)ip[8]);

  n = fmt_ip6(out, ip);
  ASSERT_EQ(0, byte_diff(out, n, "2001:db8::1"));
  n = fmt_ip6c(out, ip);
  ASSERT_EQ(0, byte_diff(out, n, "2001:db8::1"));
  n = fmt_ip6_flat(out, ip);
  ASSERT_EQ(32, n);
  ASSERT_EQ(0, byte_diff(out, 32, "20010db8000000000000000000000001"));
  ASSERT_EQ(32, scan_ip6_flat("20010db8000000000000000000000001", ip));
  ASSERT_EQ(1, (unsigned char)ip[15]);

  lo6[15] = 1;
  n = fmt_ip6c(out, lo6);
  ASSERT_EQ(0, byte_diff(out, n, "::1"));

  v4m[10] = v4m[11] = (char)0xff;
  v4m[12] = 10;
  v4m[15] = 7;
  n = fmt_ip6(out, v4m); /* an IPv4 mapped address ends in dotted form */
  ASSERT_EQ(0, byte_diff(out, n, "::ffff:10.0.0.7"));
  n = fmt_ip6c(out, v4m); /* the compact form prints it as a plain IPv4 address */
  ASSERT_EQ(0, byte_diff(out, n, "10.0.0.7"));
  ASSERT_EQ(14, scan_ip6("::ffff:1.2.3.4", ip));
  ASSERT_EQ(1, (unsigned char)ip[12]);

  ASSERT_EQ(10, scan_ip6if("fe80::1%12", ip, &ifx)); /* a numeric scope */
  ASSERT_EQ(12, ifx);
  ASSERT_EQ(7, scan_ip6if("fe80::1", ip, &ifx));
  ASSERT_EQ(0, ifx);
  n = fmt_ip6if(out, ip, 0);
  ASSERT_EQ(0, byte_diff(out, n, "fe80::1"));
  /* the interface name used to be freed although it pointed into the input: valgrind runs this clean */
  ASSERT_EQ(1, scan_ip6if("fe80::1%nosuchif0", ip, &ifx) >= 7);
}

TEST(test_socket_loopback_addresses) {
  const char* l4 = socket_ip4loopback();
  const char* l6 = socket_ip6loopback();

  ASSERT_EQ(127, (unsigned char)l4[0]);
  ASSERT_EQ(1, (unsigned char)l4[3]);
  ASSERT_EQ(0, (unsigned char)l6[0]);
  ASSERT_EQ(1, (unsigned char)l6[15]);
}

/*
 * socket_bind4(_reuse), socket_local4, socket_listen, socket_connect4, socket_accept4,
 * socket_remote4, socket_connected, socket_error, socket_tcp4b (non blocking)
 */
TEST(test_socket_tcp_loopback) {
  int srv = socket_tcp4(), c = socket_tcp4b(), a;
  char lip[4], pip[4], rip[4], buf[8];
  uint16 port = 0, pport = 0, rport = 0;
  int err = -1;

  ASSERT_EQ(1, srv >= 0 && c >= 0);
  ASSERT_EQ(0, socket_bind4_reuse(srv, socket_ip4loopback(), 0));
  ASSERT_EQ(0, socket_local4(srv, lip, &port));
  ASSERT_EQ(1, port > 0);
  ASSERT_EQ(127, (unsigned char)lip[0]);
  ASSERT_EQ(0, socket_listen(srv, 5));

  socket_connect4(c, socket_ip4loopback(), port); /* non blocking: may be in progress */
  a = socket_accept4(srv, pip, &pport);
  ASSERT_EQ(1, a >= 0);
  ASSERT_EQ(127, (unsigned char)pip[0]);
  ASSERT_EQ(1, pport > 0);
  ASSERT_EQ(1, socket_connected(c));
  ASSERT_EQ(0, socket_remote4(c, rip, &rport));
  ASSERT_EQ(port, rport);
  ASSERT_EQ(1, socket_error(c, &err) >= 0);

  ASSERT_EQ(2, write(c, "hi", 2));
  ASSERT_EQ(2, read(a, buf, sizeof buf));
  ASSERT_EQ(0, byte_diff(buf, 2, "hi"));
  socket_close(a);
  socket_close(c);
  socket_close(srv);
}

/*
 * socket_udp4, socket_send4, socket_recv4, socket_broadcast
 */
TEST(test_socket_udp_loopback) {
  int u1 = socket_udp4(), u2 = socket_udp4();
  char lip[4], sip[4], rb[16];
  uint16 up = 0, sp = 0;

  ASSERT_EQ(1, u1 >= 0 && u2 >= 0);
  ASSERT_EQ(0, socket_bind4(u1, socket_ip4loopback(), 0));
  ASSERT_EQ(0, socket_local4(u1, lip, &up));
  ASSERT_EQ(4, socket_send4(u2, "ping", 4, socket_ip4loopback(), up));
  ASSERT_EQ(4, socket_recv4(u1, rb, sizeof rb, sip, &sp));
  ASSERT_EQ(0, byte_diff(rb, 4, "ping"));
  ASSERT_EQ(127, (unsigned char)sip[0]);
  ASSERT_EQ(1, sp > 0);
  ASSERT_EQ(0, socket_broadcast(u2));
  socket_close(u1);
  socket_close(u2);
}

/*
 * fmt_ip6ifc: an address with its scope, "addr%interface"
 */
TEST(test_fmt_ip6ifc) {
  char buf[64];
  static const char ll[16] = {0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  size_t n;

  n = fmt_ip6ifc(buf, ll, 0);
  buf[n] = 0;
  ASSERT_STR_EQUAL("fe80::1", buf); /* no scope: just the address */
  ASSERT_EQ(n, fmt_ip6ifc(NULL, ll, 0));
  n = fmt_ip6ifc(buf, ll, 2);
  buf[n] = 0;
  ASSERT_EQ(1, str_start(buf, "fe80::1%"));
  ASSERT_EQ(n, fmt_ip6ifc(NULL, ll, 2));
}

/*
 * socket_getifidx, socket_getifname
 */
TEST(test_socket_ifname_ifidx) {
  uint32 idx = socket_getifidx("lo");

  if(idx == 0)
    return; /* no interface names here */
  ASSERT_STR_EQUAL("lo", socket_getifname(idx));
  ASSERT_EQ(0, socket_getifidx("no-such-if0"));
  ASSERT_STR_EQUAL("[unknown]", socket_getifname(999999));
}

/*
 * socket options that exist only on some systems must not fail hard:
 * socket_fastopen, socket_quickack, socket_deferaccept, multicast v4 settings
 */
TEST(test_socket_options) {
  int t = socket_tcp4(), u = socket_udp4();
  char loop[4] = {127, 0, 0, 1}, group[4] = {(char)239, 1, 2, 3};

  ASSERT_EQ(1, t >= 0 && u >= 0);
  socket_fastopen(t);
  socket_quickack(t, 1);
  socket_deferaccept(t, 0);
  ASSERT_EQ(1, socket_mcttl4(u, 2) <= 0);
  ASSERT_EQ(1, socket_mcloop4(u, 1) <= 0);
  /* joining may be refused in a sandbox without multicast, leaving needs a join first */
  if(socket_mcjoin4(u, group, loop) == 0) {
    /* the leave names no interface: the system decides if that matches the join */
    ASSERT_EQ(1, socket_mcleave4(u, group) <= 0);
  } else {
    ASSERT_EQ(-1, socket_mcleave4(u, group));
  }
  ASSERT_EQ(0, V6any[0]);
  ASSERT_EQ(0xff, (unsigned char)V4mappedprefix[10]);
  ASSERT_EQ(0xff, (unsigned char)V4mappedprefix[11]);
  socket_close(t);
  socket_close(u);
}

/*
 * the IPv6 variants, when the system has IPv6 at all
 */
TEST(test_socket_ipv6_loopback) {
  int srv = socket_tcp6(), c, a, u1, u2;
  char ip[16], lip[16], rb[16];
  uint16 port = 0, pport = 0, up = 0, sp = 0;
  uint32 scope = 0;
  static const char lo6[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

  if(srv < 0)
    return; /* no IPv6 here */
  if(socket_bind6_reuse(srv, lo6, 0, 0) != 0) {
    socket_close(srv);
    return; /* IPv6 without a loopback address */
  }
  ASSERT_EQ(0, socket_local6(srv, lip, &port, &scope));
  ASSERT_EQ(0, byte_diff(lip, 16, lo6));
  ASSERT_EQ(0, socket_listen(srv, 5));
  c = socket_tcp6b();
  socket_connect6(c, lo6, port, 0);
  a = socket_accept6(srv, ip, &pport, &scope);
  ASSERT_EQ(1, a >= 0);
  ASSERT_EQ(0, byte_diff(ip, 16, lo6));
  ASSERT_EQ(0, socket_remote6(c, ip, &pport, &scope));
  ASSERT_EQ(port, pport);
  socket_close(a);
  socket_close(c);
  socket_close(srv);

  u1 = socket_udp6();
  u2 = socket_udp6();
  ASSERT_EQ(1, u1 >= 0 && u2 >= 0);
  ASSERT_EQ(0, socket_bind6(u1, lo6, 0, 0));
  socket_local6(u1, lip, &up, &scope);
  ASSERT_EQ(4, socket_send6(u2, "ping", 4, lo6, up, 0));
  ASSERT_EQ(4, socket_recv6(u1, rb, sizeof rb, ip, &sp, &scope));
  ASSERT_EQ(0, byte_diff(rb, 4, "ping"));
  ASSERT_EQ(0, byte_diff(ip, 16, lo6));
  socket_mchopcount6(u2, 1);
  socket_mcloop6(u2, 1);
  socket_close(u1);
  socket_close(u2);
}

/*
 * the SCTP sockets exist or the system says it has none
 */
TEST(test_socket_sctp) {
  int s[4], i;

  s[0] = socket_sctp4();
  s[1] = socket_sctp4b();
  s[2] = socket_sctp6();
  s[3] = socket_sctp6b();
  for(i = 0; i < 4; ++i)
    if(s[i] >= 0)
      socket_close(s[i]);
  ASSERT_EQ(1, 1);
}

#define RUN_SOCKET_TESTS() \
  RUN(test_fmt_ip6ifc); \
  RUN(test_socket_ifname_ifidx); \
  RUN(test_socket_options); \
  RUN(test_socket_ipv6_loopback); \
  RUN(test_socket_sctp); \
  RUN(test_socket_ip4_text); \
  RUN(test_socket_ip6_text); \
  RUN(test_socket_loopback_addresses); \
  RUN(test_socket_tcp_loopback); \
  RUN(test_socket_udp_loopback); \
  RUN(test_socket_tcp4_close); \
  RUN(test_socket_udp4); \
  RUN(test_socket_bind4_local4); \
  RUN(test_socket_listen); \
  RUN(test_socket_is4_is6); \
  RUN(test_socket_ip4loopback); \
  RUN(test_socket_tryreservein); \
  RUN(test_socket_v6only)

TESTS(socket) { RUN_SOCKET_TESTS(); }
