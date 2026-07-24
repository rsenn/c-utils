#include "unit_test.h"
#include "../lib/ndelay.h"

#include <fcntl.h>
#include <unistd.h>

/*
 * int ndelay_on(fd_type);
 * int ndelay_off(fd_type);
 */
TEST(test_ndelay_on_off) {
  int fds[2];

  ASSERT_EQ(0, pipe(fds));

  ASSERT_NE(-1, ndelay_on(fds[0]));
  ASSERT_NE(0, fcntl(fds[0], F_GETFL) & O_NONBLOCK);

  ASSERT_NE(-1, ndelay_off(fds[0]));
  ASSERT_EQ(0, fcntl(fds[0], F_GETFL) & O_NONBLOCK);

  close(fds[0]);
  close(fds[1]);
}

#define RUN_NDELAY_TESTS() RUN(test_ndelay_on_off)

TESTS(ndelay) { RUN_NDELAY_TESTS(); }
