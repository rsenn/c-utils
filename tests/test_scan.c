#include "unit_test.h"
#include "../lib/scan.h"

/*
 * size_t scan_ulong(const char* src, unsigned long* dest);
 * size_t scan_ulongn(const char* src, size_t n, unsigned long int* dest);
 */
TEST(test_scan_ulong) {
  unsigned long d;

  ASSERT_EQ(3, scan_ulong("123abc", &d));
  ASSERT_EQ(123, d);

  ASSERT_EQ(2, scan_ulongn("12345", 2, &d));
  ASSERT_EQ(12, d);
}

/*
 * size_t scan_xlong(const char* src, unsigned long* dest);
 */
TEST(test_scan_xlong) {
  unsigned long d;

  ASSERT_EQ(4, scan_xlong("1a2fZZ", &d));
  ASSERT_EQ(0x1a2f, d);
}

/*
 * size_t scan_8long(const char* src, unsigned long* dest);
 */
TEST(test_scan_8long) {
  unsigned long d;

  ASSERT_EQ(3, scan_8long("17189", &d));
  ASSERT_EQ(0171, d);
}

/*
 * size_t scan_long(const char* src, signed long* dest);
 */
TEST(test_scan_long) {
  signed long d;

  ASSERT_EQ(4, scan_long("-123x", &d));
  ASSERT_EQ(-123, d);

  ASSERT_EQ(2, scan_long("42", &d));
  ASSERT_EQ(42, d);
}

/*
 * size_t scan_longlong(const char* src, int64* dest);
 * size_t scan_ulonglong(const char* src, uint64* dest);
 * size_t scan_xlonglong(const char* src, uint64* dest);
 * size_t scan_octal(const char* src, uint64* dest);
 */
TEST(test_scan_longlong_variants) {
  int64 sd;
  uint64 ud;

  ASSERT_EQ(4, scan_longlong("-987x", &sd));
  ASSERT_EQ(-987, sd);

  ASSERT_EQ(3, scan_ulonglong("987x", &ud));
  ASSERT_EQ(987, ud);

  ASSERT_EQ(4, scan_xlonglong("1a2fZ", &ud));
  ASSERT_EQ(0x1a2f, ud);

  ASSERT_EQ(3, scan_octal("017Z", &ud));
  ASSERT_EQ(017, ud);
}

/*
 * size_t scan_uint(const char* src, unsigned int* dest);
 * size_t scan_xint(const char* src, unsigned int* dest);
 * size_t scan_8int(const char* src, unsigned int* dest);
 * size_t scan_int(const char* src, signed int* dest);
 */
TEST(test_scan_int_variants) {
  unsigned int ud;
  signed int sd;

  ASSERT_EQ(3, scan_uint("123x", &ud));
  ASSERT_EQ(123, ud);

  ASSERT_EQ(2, scan_xint("1fx", &ud));
  ASSERT_EQ(0x1f, ud);

  ASSERT_EQ(2, scan_8int("17x", &ud));
  ASSERT_EQ(017, ud);

  ASSERT_EQ(3, scan_int("-42x", &sd));
  ASSERT_EQ(-42, sd);
}

/*
 * size_t scan_ushort(const char* src, unsigned short* dest);
 * size_t scan_xshort(const char* src, unsigned short* dest);
 * size_t scan_8short(const char* src, unsigned short* dest);
 * size_t scan_short(const char* src, signed short* dest);
 */
TEST(test_scan_short_variants) {
  unsigned short ud;
  signed short sd;

  ASSERT_EQ(2, scan_ushort("42x", &ud));
  ASSERT_EQ(42, ud);

  ASSERT_EQ(2, scan_xshort("1fx", &ud));
  ASSERT_EQ(0x1f, ud);

  ASSERT_EQ(2, scan_8short("17x", &ud));
  ASSERT_EQ(017, ud);

  ASSERT_EQ(3, scan_short("-12x", &sd));
  ASSERT_EQ(-12, sd);
}

/*
 * size_t scan_xchar(const char*, unsigned char*);
 */
TEST(test_scan_xchar) {
  unsigned char d;

  ASSERT_EQ(1, scan_xchar("fZ", &d));
  ASSERT_EQ(0xf, d);
}

/*
 * size_t scan_double(const char* in, double* dest);
 */
TEST(test_scan_double) {
  double d;

  ASSERT_EQ(4, scan_double("3.14x", &d));
  ASSERT_GE(d, 3.13);
  ASSERT_LE(d, 3.15);
}

/*
 * size_t scan_plusminus(const char* src, signed int* dest);
 */
TEST(test_scan_plusminus) {
  signed int d;

  ASSERT_EQ(1, scan_plusminus("-5", &d));
  ASSERT_EQ(-1, d);

  ASSERT_EQ(1, scan_plusminus("+5", &d));
  ASSERT_EQ(1, d);

  ASSERT_EQ(0, scan_plusminus("5", &d));
  ASSERT_EQ(1, d);
}

/*
 * size_t scan_whitenskip(const char*, size_t);
 * size_t scan_nonwhitenskip(const char*, size_t);
 */
TEST(test_scan_whitenskip) {
  ASSERT_EQ(2, scan_whitenskip("  abc", 5));
  ASSERT_EQ(3, scan_nonwhitenskip("abc  ", 5));
}

/*
 * size_t scan_charsetnskip(const char* in, const char* charset, size_t limit);
 * size_t scan_noncharsetnskip(const char* in, const char* charset, size_t limit);
 */
TEST(test_scan_charsetnskip) {
  ASSERT_EQ(3, scan_charsetnskip("aaabbb", "a", 6));
  ASSERT_EQ(3, scan_noncharsetnskip("aaabbb", "b", 6));
}

/*
 * size_t scan_httpdate(const char* in, time_t* t);
 */
TEST(test_scan_httpdate) {
  time_t t = 0;

  ASSERT_NE(0, scan_httpdate("Sun, 06 Nov 1994 08:49:37 GMT", &t));
  ASSERT_NE(0, t);
}

/*
 * size_t scan_utf8(const char* in, size_t len, uint32* n);
 */
TEST(test_scan_utf8) {
  uint32 n;

  ASSERT_EQ(1, scan_utf8("A", 1, &n));
  ASSERT_EQ('A', n);

  /* U+00E9 'e' with acute accent, UTF-8: 0xC3 0xA9 */
  ASSERT_EQ(2, scan_utf8("\xc3\xa9", 2, &n));
  ASSERT_EQ(0xe9, n);
}

/*
 * int scan_fromhex(int c);
 */
TEST(test_scan_fromhex) {
  ASSERT_EQ(0, scan_fromhex('0'));
  ASSERT_EQ(9, scan_fromhex('9'));
  ASSERT_EQ(10, scan_fromhex('a'));
  ASSERT_EQ(15, scan_fromhex('F'));
  ASSERT_EQ(-1, scan_fromhex('z'));
}

/*
 * size_t scan_line(const char*, size_t);
 * size_t scan_lineskip(const char*, size_t);
 */
TEST(test_scan_line) {
  ASSERT_EQ(3, scan_line("abc\ndef", 7));
  ASSERT_EQ(4, scan_lineskip("abc\ndef", 7));
}

/*
 * size_t scan_eolskip(const char* s, size_t limit);
 */
TEST(test_scan_eolskip) {
  ASSERT_EQ(1, scan_eolskip("\ndef", 4));
}

/*
 * size_t scan_human(const char*, uint64*);
 */
TEST(test_scan_human) {
  uint64 d;

  ASSERT_EQ(2, scan_human("42", &d));
  ASSERT_EQ(42, d);

  ASSERT_EQ(2, scan_human("1K", &d));
  ASSERT_EQ(1024, d);
}

/*
 * size_t scan_varint(const char* in, size_t len, uint64* n);
 */
TEST(test_scan_varint) {
  uint64 n;
  char buf[2] = {(char)0x96, (char)0x01}; /* 150 in protobuf varint */

  ASSERT_EQ(2, scan_varint(buf, 2, &n));
  ASSERT_EQ(150, n);
}

/*
 * size_t scan_asn1derlength(const char* in, size_t len, uint64* n);
 * size_t scan_asn1dertag(const char* in, size_t len, uint64* n);
 */
TEST(test_scan_asn1) {
  uint64 n;
  /* length byte 0x05 says "5 bytes of content follow"; scan_asn1derlength
   * also verifies that much data is actually present in the buffer. */
  char lenbuf[6] = {0x05, 0, 0, 0, 0, 0};

  ASSERT_EQ(1, scan_asn1derlength(lenbuf, 6, &n));
  ASSERT_EQ(5, n);

  char tagbuf[1] = {0x02};
  ASSERT_EQ(1, scan_asn1dertag(tagbuf, 1, &n));
  ASSERT_EQ(2, n);
}

#define RUN_SCAN_TESTS() \
  RUN(test_scan_ulong); \
  RUN(test_scan_xlong); \
  RUN(test_scan_8long); \
  RUN(test_scan_long); \
  RUN(test_scan_longlong_variants); \
  RUN(test_scan_int_variants); \
  RUN(test_scan_short_variants); \
  RUN(test_scan_xchar); \
  RUN(test_scan_double); \
  RUN(test_scan_plusminus); \
  RUN(test_scan_whitenskip); \
  RUN(test_scan_charsetnskip); \
  RUN(test_scan_httpdate); \
  RUN(test_scan_utf8); \
  RUN(test_scan_fromhex); \
  RUN(test_scan_line); \
  RUN(test_scan_eolskip); \
  RUN(test_scan_human); \
  RUN(test_scan_varint); \
  RUN(test_scan_asn1)

TESTS(scan) { RUN_SCAN_TESTS(); }
