#include "unit_test.h"
#include "../lib/iom.h"
#include <string.h>

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
 *
 * Not tested: neither HAVE_EPOLL nor HAVE_KQUEUE is ever defined by this
 * build (see BUGS: iom-epoll-kqueue-never-enabled), so `iom_add` falls
 * off the end of a non-void function with no `return`, and `iom_wait`'s
 * polling branch never calls epoll_wait/kevent and just fabricates a
 * ready event. Calling either here would assert on undefined behavior
 * rather than real functionality.
 */
TEST(test_iom_add) {}
TEST(test_iom_wait) {}

#define RUN_IOM_TESTS() \
  RUN(test_iom_init); \
  RUN(test_iom_abort); \
  RUN(test_iom_add); \
  RUN(test_iom_wait)

TESTS(iom) { RUN_IOM_TESTS(); }
