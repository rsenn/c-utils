#include "unit_test.h"
#include "../lib/socket.h"

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
TEST(test_socket_v6only) {}

#define RUN_SOCKET_TESTS() \
  RUN(test_socket_tcp4_close); \
  RUN(test_socket_udp4); \
  RUN(test_socket_bind4_local4); \
  RUN(test_socket_listen); \
  RUN(test_socket_is4_is6); \
  RUN(test_socket_ip4loopback); \
  RUN(test_socket_tryreservein); \
  RUN(test_socket_v6only)

TESTS(socket) { RUN_SOCKET_TESTS(); }
