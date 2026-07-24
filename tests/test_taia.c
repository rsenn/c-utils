#include "unit_test.h"
#include "../lib/taia.h"

/*
 * void taia_now(struct taia*);
 */
TEST(test_taia_now) {
  tai6464 t;

  taia_now(&t);
  ASSERT_NE(0, t.sec.x);
}

/*
 * void taia_tai(const tai6464* source, tai64* dest);
 */
TEST(test_taia_tai) {
  tai6464 t;
  tai64 sec;

  t.sec.x = 12345;
  t.nano = 0;
  t.atto = 0;

  taia_tai(&t, &sec);
  ASSERT_EQ(12345, sec.x);
}

/*
 * double taia_approx(const tai6464*);
 * double taia_frac(const tai6464*);
 */
TEST(test_taia_approx_frac) {
  tai6464 t;

  t.sec.x = 5;
  t.nano = 500000000;
  t.atto = 0;

  ASSERT_GE(taia_approx(&t), 5.0);
  ASSERT_GE(taia_frac(&t), 0.0);
}

/*
 * void taia_add(tai6464* dest, const tai6464* source1, const tai6464* source2);
 * void taia_addsec(tai6464* dest, const tai6464* source, long secs);
 * void taia_sub(tai6464* dest, const tai6464* source1, const tai6464* source2);
 */
TEST(test_taia_add_sub) {
  tai6464 a, b, sum, diff, addsec;

  a.sec.x = 10;
  a.nano = 0;
  a.atto = 0;
  b.sec.x = 5;
  b.nano = 0;
  b.atto = 0;

  taia_add(&sum, &a, &b);
  ASSERT_EQ(15, sum.sec.x);

  taia_sub(&diff, &sum, &b);
  ASSERT_EQ(10, diff.sec.x);

  taia_addsec(&addsec, &a, 3);
  ASSERT_EQ(13, addsec.sec.x);
}

/*
 * void taia_half(tai6464* dest, const tai6464* source);
 */
TEST(test_taia_half) {
  tai6464 a, half;

  a.sec.x = 10;
  a.nano = 0;
  a.atto = 0;

  taia_half(&half, &a);
  ASSERT_EQ(5, half.sec.x);
}

/*
 * int taia_less(const tai6464* a, const tai6464* b);
 */
TEST(test_taia_less) {
  tai6464 a, b;

  a.sec.x = 1;
  a.nano = 0;
  a.atto = 0;
  b.sec.x = 2;
  b.nano = 0;
  b.atto = 0;

  ASSERT_NE(0, taia_less(&a, &b));
  ASSERT_EQ(0, taia_less(&b, &a));
}

/*
 * void taia_pack(char* buf, const tai6464* src);
 * void taia_unpack(const char* buf, tai6464* dest);
 */
TEST(test_taia_pack_unpack) {
  tai6464 a, b;
  char buf[TAIA_PACK];

  a.sec.x = 0x1122334455667788ULL;
  a.nano = 123456789;
  a.atto = 987654321;

  taia_pack(buf, &a);
  taia_unpack(buf, &b);

  ASSERT_EQ(a.sec.x, b.sec.x);
  ASSERT_EQ(a.nano, b.nano);
  ASSERT_EQ(a.atto, b.atto);
}

/*
 * unsigned int taia_fmtfrac(char* s, const tai6464* t);
 *
 * Not tested: declared in taia.h but has no implementation anywhere in
 * the tree (would fail to link). See BUGS.
 */
TEST(test_taia_fmtfrac) {}

/*
 * void taia_uint(tai6464* t, unsigned int secs);
 */
TEST(test_taia_uint) {
  tai6464 t;

  taia_uint(&t, 42);
  ASSERT_EQ(42, t.sec.x);
}

#define RUN_TAIA_TESTS() \
  RUN(test_taia_now); \
  RUN(test_taia_tai); \
  RUN(test_taia_approx_frac); \
  RUN(test_taia_add_sub); \
  RUN(test_taia_half); \
  RUN(test_taia_less); \
  RUN(test_taia_pack_unpack); \
  RUN(test_taia_fmtfrac); \
  RUN(test_taia_uint)

TESTS(taia) { RUN_TAIA_TESTS(); }
