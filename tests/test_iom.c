#include "unit_test.h"
#include "../lib/iom.h"
#include <string.h>
#include <unistd.h>

/*
 * int iom_init(iomux_t* c);
 */
TEST(test_iom_init) {
  iomux_t c;

  memset(&c, 0, sizeof(c));
  iom_init(&c);

  ASSERT_EQ(0, c.h);
  ASSERT_EQ(0, c.l);
  ASSERT_EQ(0, c.working);
}

/*
 * int iom_abort(iomux_t* c);
 */
TEST(test_iom_abort) {
  iomux_t c;

  memset(&c, 0, sizeof(c));
  iom_init(&c);
  iom_abort(&c);

  ASSERT_EQ(-2, c.working);
}

/*
 * int iom_add(iomux_t* c, int64 s, unsigned int events);
 * int iom_wait(iomux_t* c, int64* s, unsigned int* revents, unsigned long timeout);
 */
TEST(test_iom_add) {
#ifdef HAVE_EPOLL
  iomux_t c;
  int p[2];

  memset(&c, 0, sizeof(c));
  ASSERT_EQ(1, iom_init(&c)); /* 1 when the epoll descriptor was made */
  ASSERT_EQ(0, pipe(p));
  ASSERT_EQ(0, iom_add(&c, p[0], IOM_READ));
  ASSERT_EQ(0, iom_add(&c, p[1], IOM_WRITE));
  ASSERT_EQ(-1, iom_add(&c, -1, IOM_READ));
  close(p[0]);
  close(p[1]);
#endif
}
TEST(test_iom_wait) {
#ifdef HAVE_EPOLL
  iomux_t c;
  int p[2];
  int64 fd = -1;
  unsigned int ev = 0;

  memset(&c, 0, sizeof(c));
  ASSERT_EQ(1, iom_init(&c)); /* 1 when the epoll descriptor was made */
  ASSERT_EQ(0, pipe(p));
  ASSERT_EQ(0, iom_add(&c, p[0], IOM_READ));

  /* nothing to read: a timeout */
  ASSERT_EQ(0, iom_wait(&c, &fd, &ev, 20));

  write(p[1], "x", 1);
  ASSERT_EQ(1, iom_wait(&c, &fd, &ev, 1000));
  ASSERT_EQ(p[0], fd);
  ASSERT_EQ(IOM_READ, ev & IOM_READ);

  /* one-shot: the descriptor must be added again */
  ASSERT_EQ(0, iom_wait(&c, &fd, &ev, 20));
  ASSERT_EQ(0, iom_add(&c, p[0], IOM_READ));
  ASSERT_EQ(1, iom_wait(&c, &fd, &ev, 1000));

  iom_abort(&c);
  ASSERT_EQ(-2, iom_wait(&c, &fd, &ev, 20));
  close(p[0]);
  close(p[1]);
#endif
}

#define RUN_IOM_TESTS() \
  RUN(test_iom_init); \
  RUN(test_iom_abort); \
  RUN(test_iom_add); \
  RUN(test_iom_wait)

TESTS(iom) { RUN_IOM_TESTS(); }
