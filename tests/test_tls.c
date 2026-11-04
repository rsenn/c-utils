#include "unit_test.h"
#include "../lib/tls.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/wait.h>

#ifdef HAVE_OPENSSL
#include "../lib/tls_internal.h"

#define KEYFILE "/tmp/c-utils-tls-key.pem"
#define CERTFILE "/tmp/c-utils-tls-cert.pem"

/* a self signed certificate for the server of the test; 0 if openssl cannot make one */
static int
make_certificate(void) {
  return system("openssl req -x509 -newkey rsa:2048 -nodes -keyout " KEYFILE " -out " CERTFILE
                " -subj /CN=localhost -days 2 >/dev/null 2>&1") == 0;
}

/*
 * tls_init, tls_server, tls_accept, tls_client, tls_servername, tls_connect, tls_established,
 * tls_write, tls_read, tls_by_fd, tls_get_fd, tls_error, tls_errno, tls_strerror, tls_close, tls_free
 */
TEST(test_tls_loopback) {
  int sv[2], pid, st;
  char got[64];
  ssize_t n;
  tls_t* ssl;

  if(!make_certificate())
    return; /* no openssl command: nothing to serve with */
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, sv));

  pid = fork();
  if(pid == 0) {
    char buf[64];
    int fd = sv[1], rc = 1;

    prctl(PR_SET_PDEATHSIG, SIGKILL);
    alarm(60);
    close(sv[0]);
    tls_init(KEYFILE, CERTFILE);
    if(tls_server(fd) && tls_accept(fd) == 1) {
      ssize_t r = tls_read(fd, buf, sizeof buf);

      if(r == 4 && !memcmp(buf, "ping", 4) && tls_write(fd, "pong!", 5) == 5)
        rc = 0;
      tls_close(fd);
    }
    _exit(rc);
  }
  close(sv[1]);

  ssl = tls_client(sv[0]);
  ASSERT_EQ(1, ssl != NULL);
  ASSERT_EQ(ssl, tls_by_fd(sv[0]));
  ASSERT_EQ(sv[0], tls_get_fd(ssl));
  ASSERT_EQ(1, tls_servername(sv[0], "localhost"));
  ASSERT_EQ(0, tls_servername(sv[0], NULL));
  ASSERT_EQ(0, tls_established(sv[0]));

  ASSERT_EQ(1, tls_connect(sv[0]));
  ASSERT_EQ(1, tls_established(sv[0]));
  ASSERT_EQ(TLS_ERR_NONE, tls_error(sv[0]));
  ASSERT_STR_EQUAL("none", tls_strerror(sv[0]));

  ASSERT_EQ(4, tls_write(sv[0], "ping", 4));
  n = tls_read(sv[0], got, sizeof got);
  ASSERT_EQ(5, n);
  ASSERT_EQ(0, byte_diff(got, 5, "pong!"));

  tls_close(sv[0]);
  tls_free(sv[0]);
  close(sv[0]);
  waitpid(pid, &st, 0);
  ASSERT_EQ(1, WIFEXITED(st));
  ASSERT_EQ(0, WEXITSTATUS(st));
  unlink(KEYFILE);
  unlink(CERTFILE);
}

/*
 * tls_bio_socket, tls_instance_new/get, tls_want, tls_io,
 * tls_instance_strerror, tls_instance_errno, tls_instance_debug
 */
TEST(test_tls_instances) {
  int sv[2];
  tls_t* ssl;
  tls_instance_t *i, *j;

  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, sv));

  ssl = tls_client(sv[0]);
  ASSERT_EQ(1, ssl != NULL);
  i = tls_instance_get(sv[0]);
  ASSERT_EQ(1, i != NULL);
  ASSERT_EQ(ssl, i->ssl);
  j = tls_instance_get(sv[0]);
  ASSERT_EQ(i, j); /* the same descriptor, the same instance */

  tls_io(sv[0]); /* the instance asks io for the events it wants */
  ASSERT_EQ(1, i->wantread != NULL && i->wantwrite != NULL);

  /* nothing was read yet: a want with nothing pending calls nothing */
  ASSERT_EQ(0, tls_want(sv[0], NULL, NULL) < 0 ? 1 : 0);

  ASSERT_EQ(1, tls_instance_strerror(i) != NULL);
  tls_instance_errno(i);
  tls_instance_debug(i);

  tls_free(sv[0]);
  close(sv[0]);
  close(sv[1]);
}
#else
TEST(test_tls_not_built) {}
#endif

#ifdef HAVE_OPENSSL
#define RUN_TLS_TESTS() \
  RUN(test_tls_loopback); \
  RUN(test_tls_instances)
#else
#define RUN_TLS_TESTS() RUN(test_tls_not_built)
#endif

TESTS(tls) { RUN_TLS_TESTS(); }
