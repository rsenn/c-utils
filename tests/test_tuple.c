#include "unit_test.h"
#include "../lib/tuple.h"


/*
 * mktuple
 */
TEST(test_mktuple) {
}

/*
 * tuple_consume
 */
TEST(test_tuple_consume) {
}

/*
 * tuple_left
 */
TEST(test_tuple_left) {
}

/*
 * tuple_produce
 */
TEST(test_tuple_produce) {
}

#define RUN_TUPLE_TESTS()                                                                                               \
  RUN(test_mktuple);                                                                                             \
  RUN(test_tuple_consume);                                                                                         \
  RUN(test_tuple_produce);                                                                                         \
  RUN(test_tuple_left)

TESTS(tuple) { RUN_TUPLE_TESTS(); }
