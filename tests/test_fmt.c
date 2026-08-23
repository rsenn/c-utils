#include "unit_test.h"
#include "../lib/fmt.h"
#include "../lib/str.h"

/* fmt_escapecharhtml is defined in lib/fmt/fmt_escapecharxml.c (GNU alias)
 * or lib/fmt/fmt_escapecharhtml.c, but never declared in fmt.h. See BUGS. */
size_t fmt_escapecharhtml(char* dest, unsigned int ch);

/*
 * size_t fmt_long(char* dest, signed long src);
 */
TEST(test_fmt_long) {
  char buf[FMT_LONG];

  buf[fmt_long(buf, 123)] = 0;
  ASSERT_STR_EQUAL("123", buf);

  buf[fmt_long(buf, -123)] = 0;
  ASSERT_STR_EQUAL("-123", buf);

  buf[fmt_long(buf, 0)] = 0;
  ASSERT_STR_EQUAL("0", buf);

  ASSERT_EQ(3, fmt_long(NULL, 123));
}

/*
 * size_t fmt_ulong(char* dest, unsigned long src);
 */
TEST(test_fmt_ulong) {
  char buf[FMT_ULONG];

  buf[fmt_ulong(buf, 456)] = 0;
  ASSERT_STR_EQUAL("456", buf);

  buf[fmt_ulong(buf, 0)] = 0;
  ASSERT_STR_EQUAL("0", buf);

  ASSERT_EQ(3, fmt_ulong(NULL, 456));
}

/*
 * size_t fmt_xlong(char* dest, unsigned long src);
 */
TEST(test_fmt_xlong) {
  char buf[FMT_XLONG];

  buf[fmt_xlong(buf, 0xabcd)] = 0;
  ASSERT_STR_EQUAL("abcd", buf);

  buf[fmt_xlong(buf, 0)] = 0;
  ASSERT_STR_EQUAL("0", buf);
}

/*
 * size_t fmt_xlongu(char* dest, unsigned long i);
 */
TEST(test_fmt_xlongu) {
  char buf[FMT_XLONG];

  buf[fmt_xlongu(buf, 0xabcd)] = 0;
  ASSERT_STR_EQUAL("ABCD", buf);
}

/*
 * size_t fmt_8long(char* dest, unsigned long src);
 */
TEST(test_fmt_8long) {
  char buf[FMT_8LONG];

  buf[fmt_8long(buf, 8)] = 0;
  ASSERT_STR_EQUAL("10", buf);

  buf[fmt_8long(buf, 0)] = 0;
  ASSERT_STR_EQUAL("0", buf);
}

/*
 * size_t fmt_ulong0(char*, unsigned long src, size_t padto);
 */
TEST(test_fmt_ulong0) {
  char buf[FMT_ULONG];

  buf[fmt_ulong0(buf, 7, 4)] = 0;
  ASSERT_STR_EQUAL("0007", buf);

  buf[fmt_ulong0(buf, 12345, 3)] = 0;
  ASSERT_STR_EQUAL("12345", buf);

  ASSERT_EQ(4, fmt_ulong0(NULL, 7, 4));
}

/*
 * size_t fmt_double(char* dest, double d, int max, int prec);
 */
TEST(test_fmt_double) {
  char buf[64];

  buf[fmt_double(buf, 1.5, 64, 6)] = 0;
  ASSERT_STR_EQUAL("1.5", buf);

  buf[fmt_double(buf, 0.0, 64, 6)] = 0;
  ASSERT_STR_EQUAL("0", buf);

  buf[fmt_double(buf, -2.25, 64, 6)] = 0;
  ASSERT_STR_EQUAL("-2.25", buf);
}

/*
 * size_t fmt_plusminus(char* dest, int src);
 */
TEST(test_fmt_plusminus) {
  char buf[2];

  ASSERT_EQ(1, fmt_plusminus(buf, 5));
  ASSERT_EQ('+', buf[0]);

  ASSERT_EQ(1, fmt_plusminus(buf, -5));
  ASSERT_EQ('-', buf[0]);

  ASSERT_EQ(0, fmt_plusminus(buf, 0));
}

/*
 * size_t fmt_minus(char* dest, int src);
 */
TEST(test_fmt_minus) {
  char buf[2];

  ASSERT_EQ(1, fmt_minus(buf, -5));
  ASSERT_EQ('-', buf[0]);

  ASSERT_EQ(0, fmt_minus(buf, 5));
  ASSERT_EQ(0, fmt_minus(buf, 0));
}

/*
 * size_t fmt_str(char* dest, const char* src);
 */
TEST(test_fmt_str) {
  char buf[16];

  buf[fmt_str(buf, "hello")] = 0;
  ASSERT_STR_EQUAL("hello", buf);

  ASSERT_EQ(5, fmt_str(NULL, "hello"));
}

/*
 * size_t fmt_strn(char* dest, const char* src, size_t limit);
 */
TEST(test_fmt_strn) {
  char buf[16];

  buf[fmt_strn(buf, "hello world", 5)] = 0;
  ASSERT_STR_EQUAL("hello", buf);

  buf[fmt_strn(buf, "hi", 5)] = 0;
  ASSERT_STR_EQUAL("hi", buf);
}

/*
 * size_t fmt_pad(char* dest, const char* src, size_t srclen, size_t padlen, size_t maxlen);
 */
TEST(test_fmt_pad) {
  char buf[16];
  size_t n;

  n = fmt_pad(buf, "foo", 3, 5, 16);
  buf[n] = 0;
  ASSERT_STR_EQUAL("  foo", buf);

  n = fmt_pad(buf, "foo", 3, 2, 16);
  buf[n] = 0;
  ASSERT_STR_EQUAL("foo", buf);
}

/*
 * size_t fmt_fill(char* dest, size_t srclen, size_t padlen, size_t maxlen);
 */
TEST(test_fmt_fill) {
  char buf[16];
  size_t n;

  str_copy(buf, "foo");
  n = fmt_fill(buf, 3, 5, 16);
  buf[n] = 0;
  ASSERT_STR_EQUAL("foo  ", buf);
}

/*
 * size_t fmt_human(char* dest, uint64 l);
 */
TEST(test_fmt_human) {
  char buf[16];

  buf[fmt_human(buf, 42)] = 0;
  ASSERT_STR_EQUAL("42", buf);

  buf[fmt_human(buf, 4900)] = 0;
  ASSERT_STR_EQUAL("4.9k", buf);

  buf[fmt_human(buf, 2300000)] = 0;
  ASSERT_STR_EQUAL("2.3M", buf);
}

/*
 * size_t fmt_humank(char* dest, uint64 l);
 */
TEST(test_fmt_humank) {
  char buf[16];

  buf[fmt_humank(buf, 42)] = 0;
  ASSERT_STR_EQUAL("42", buf);

  buf[fmt_humank(buf, 4900)] = 0;
  ASSERT_STR_EQUAL("4.8k", buf);
}

/*
 * size_t fmt_httpdate(char* dest, time_t t);
 */
TEST(test_fmt_httpdate) {
  char buf[32];
  /* 1994-11-06 08:49:37 UTC */
  time_t t = 784111777;

  buf[fmt_httpdate(buf, t)] = 0;
  ASSERT_STR_EQUAL("Sun, 06 Nov 1994 08:49:37 GMT", buf);
  ASSERT_EQ(29, fmt_httpdate(NULL, t));
}

/*
 * size_t fmt_iso8601(char* dest, time_t t);
 */
TEST(test_fmt_iso8601) {
  char buf[32];
  time_t t = 784111777;

  buf[fmt_iso8601(buf, t)] = 0;
  ASSERT_STR_EQUAL("1994-11-06T08:49:37Z", buf);
}

/*
 * size_t fmt_utf8(char* dest, uint32 n);
 */
TEST(test_fmt_utf8) {
  unsigned char buf[8];

  ASSERT_EQ(1, fmt_utf8((char*)buf, 'A'));
  ASSERT_EQ('A', buf[0]);

  ASSERT_EQ(2, fmt_utf8((char*)buf, 0x80));
  ASSERT_EQ(0xc2, buf[0]);
  ASSERT_EQ(0x80, buf[1]);

  ASSERT_EQ(3, fmt_utf8((char*)buf, 0x20ac));
  ASSERT_EQ(0xe2, buf[0]);
  ASSERT_EQ(0x82, buf[1]);
  ASSERT_EQ(0xac, buf[2]);
}

/*
 * size_t fmt_asn1derlength(char* dest, uint64 l);
 */
TEST(test_fmt_asn1derlength) {
  unsigned char buf[16];

  ASSERT_EQ(1, fmt_asn1derlength((char*)buf, 100));
  ASSERT_EQ(100, buf[0]);

  ASSERT_EQ(2, fmt_asn1derlength((char*)buf, 200));
  ASSERT_EQ(0x81, buf[0]);
  ASSERT_EQ(200, buf[1]);
}

/*
 * size_t fmt_asn1dertag(char* dest, uint64 l);
 */
TEST(test_fmt_asn1dertag) {
  unsigned char buf[16];

  ASSERT_EQ(1, fmt_asn1dertag((char*)buf, 0x7f));
  ASSERT_EQ(0x7f, buf[0]);

  ASSERT_EQ(2, fmt_asn1dertag((char*)buf, 0x80));
  ASSERT_EQ(0x81, buf[0]);
  ASSERT_EQ(0x00, buf[1]);
}

/*
 * char fmt_tohex(char c);
 */
TEST(test_fmt_tohex) {
  ASSERT_EQ('0', fmt_tohex(0));
  ASSERT_EQ('9', fmt_tohex(9));
  ASSERT_EQ('a', fmt_tohex(10));
  ASSERT_EQ('f', fmt_tohex(15));
}

/*
 * size_t fmt_ulonglong(char* dest, uint64 i);
 * size_t fmt_longlong(char* dest, int64 i);
 * size_t fmt_xlonglong(char* dest, uint64 x);
 * size_t fmt_octal(char* dest, uint64 o);
 */
TEST(test_fmt_longlong_family) {
  char buf[FMT_LONG];

  buf[fmt_ulonglong(buf, 1234567890123ULL)] = 0;
  ASSERT_STR_EQUAL("1234567890123", buf);

  buf[fmt_longlong(buf, -1234567890123LL)] = 0;
  ASSERT_STR_EQUAL("-1234567890123", buf);

  buf[fmt_xlonglong(buf, 0xdeadbeefULL)] = 0;
  ASSERT_STR_EQUAL("deadbeef", buf);

  buf[fmt_octal(buf, 8)] = 0;
  ASSERT_STR_EQUAL("10", buf);
}

/*
 * size_t fmt_xlong0(char*, size_t num, size_t n);
 * size_t fmt_xlong0u(char*, size_t num, size_t n);
 */
TEST(test_fmt_xlong0_family) {
  char buf[32];

  buf[fmt_xlong0(buf, 0xab, 4)] = 0;
  ASSERT_STR_EQUAL("00ab", buf);

  buf[fmt_xlong0u(buf, 0xab, 4)] = 0;
  ASSERT_STR_EQUAL("00AB", buf);

  buf[fmt_xlong0(buf, 0xabcdef, 2)] = 0;
  ASSERT_STR_EQUAL("abcdef", buf);
}

/*
 * size_t fmt_hexb(void* out, const void* d, size_t len);
 * size_t fmt_hexbs(void* out, const void* d, size_t len);
 */
TEST(test_fmt_hexb) {
  char buf[32];
  size_t n;

  n = fmt_hexb(buf, "\x01\xab\xff", 3);
  buf[n] = 0;
  ASSERT_EQ(6, n);
  ASSERT_STR_EQUAL("01abff", buf);

  n = fmt_hexbs(buf, "\x01\xab\xff", 3);
  buf[n] = 0;
  ASSERT_EQ(8, n);
  ASSERT_STR_EQUAL("01 ab ff", buf);
}

/*
 * size_t fmt_repeat(char* dest, const char* src, int n);
 */
TEST(test_fmt_repeat) {
  char buf[16];
  size_t n = fmt_repeat(buf, "ab", 3);

  buf[n] = 0;
  ASSERT_STR_EQUAL("ababab", buf);
}

/*
 * size_t fmt_escapecharc(char* dest, int ch);
 */
TEST(test_fmt_escapecharc) {
  char buf[8];

  buf[fmt_escapecharc(buf, '\n')] = 0;
  ASSERT_STR_EQUAL("\\n", buf);

  buf[fmt_escapecharc(buf, '\\')] = 0;
  ASSERT_STR_EQUAL("\\\\", buf);

  ASSERT_EQ(4, fmt_escapecharc(buf, 1));
}

/*
 * size_t fmt_escapecharcx(char* dest, int chr);
 */
TEST(test_fmt_escapecharcx) {
  char buf[8];

  buf[fmt_escapecharcx(buf, '\n')] = 0;
  ASSERT_STR_EQUAL("\\n", buf);

  buf[fmt_escapecharcx(buf, 1)] = 0;
  ASSERT_STR_EQUAL("\\x01", buf);
}

/*
 * size_t fmt_escapecharcontrol(char* dest, int c);
 */
TEST(test_fmt_escapecharcontrol) {
  char buf[8];

  buf[fmt_escapecharcontrol(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  buf[fmt_escapecharcontrol(buf, '\n')] = 0;
  ASSERT_STR_EQUAL("\\n", buf);

  ASSERT_EQ(2, fmt_escapecharcontrol(buf, '\0'));
}

/*
 * size_t fmt_escapecharnonprintable(char* dest, int c);
 */
TEST(test_fmt_escapecharnonprintable) {
  char buf[8];

  buf[fmt_escapecharnonprintable(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  buf[fmt_escapecharnonprintable(buf, '\n')] = 0;
  ASSERT_STR_EQUAL("\\n", buf);
}

/*
 * size_t fmt_escapecharshell(char* dest, int ch);
 */
TEST(test_fmt_escapecharshell) {
  char buf[8];

  buf[fmt_escapecharshell(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  buf[fmt_escapecharshell(buf, '"')] = 0;
  ASSERT_STR_EQUAL("\\\"", buf);

  buf[fmt_escapecharshell(buf, 0)] = 0;
  ASSERT_STR_EQUAL("\\x00", buf);
}

/*
 * size_t fmt_escapecharquotedshell(char*, int ch);
 */
TEST(test_fmt_escapecharquotedshell) {
  char buf[8];

  buf[fmt_escapecharquotedshell(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  ASSERT_EQ(4, fmt_escapecharquotedshell(buf, '\''));
}

/*
 * size_t fmt_escapechardoublequotedshell(char* dest, int ch);
 */
TEST(test_fmt_escapechardoublequotedshell) {
  char buf[8];

  buf[fmt_escapechardoublequotedshell(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  buf[fmt_escapechardoublequotedshell(buf, '"')] = 0;
  ASSERT_STR_EQUAL("\\\"", buf);
}

/*
 * size_t fmt_escapecharjson(char* dest, int ch, int quote);
 */
TEST(test_fmt_escapecharjson) {
  char buf[8];

  buf[fmt_escapecharjson(buf, 'A', '"')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  buf[fmt_escapecharjson(buf, '\n', '"')] = 0;
  ASSERT_STR_EQUAL("\\n", buf);

  buf[fmt_escapecharjson(buf, '"', '"')] = 0;
  ASSERT_STR_EQUAL("\\\"", buf);
}

/*
 * size_t fmt_xmlescape(char* dest, int ch);
 * size_t fmt_escapecharxml(char*, unsigned int ch);
 */
TEST(test_fmt_xmlescape) {
  char buf[16];

  buf[fmt_xmlescape(buf, '<')] = 0;
  ASSERT_STR_EQUAL("&lt;", buf);

  buf[fmt_xmlescape(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("A", buf);

  buf[fmt_escapecharxml(buf, '&')] = 0;
  ASSERT_STR_EQUAL("&amp;", buf);
}

/*
 * size_t fmt_escapecharhtml(char* dest, unsigned int ch);
 */
TEST(test_fmt_escapecharhtml) {
  char buf[16];

  buf[fmt_escapecharhtml(buf, '<')] = 0;
  ASSERT_STR_EQUAL("&lt;", buf);
}

/*
 * size_t fmt_escapecharquotedprintable(char* dest, int ch);
 * size_t fmt_escapecharquotedprintableutf8(char* dest, int ch);
 */
TEST(test_fmt_escapecharquotedprintable) {
  char buf[8];

  buf[fmt_escapecharquotedprintable(buf, 0xab)] = 0;
  ASSERT_STR_EQUAL("=AB", buf);

  buf[fmt_escapecharquotedprintableutf8(buf, 'A')] = 0;
  ASSERT_STR_EQUAL("=41", buf);
}

#define RUN_FMT_TESTS() \
  RUN(test_fmt_long); \
  RUN(test_fmt_ulong); \
  RUN(test_fmt_xlong); \
  RUN(test_fmt_xlongu); \
  RUN(test_fmt_8long); \
  RUN(test_fmt_ulong0); \
  RUN(test_fmt_double); \
  RUN(test_fmt_plusminus); \
  RUN(test_fmt_minus); \
  RUN(test_fmt_str); \
  RUN(test_fmt_strn); \
  RUN(test_fmt_pad); \
  RUN(test_fmt_fill); \
  RUN(test_fmt_human); \
  RUN(test_fmt_humank); \
  RUN(test_fmt_httpdate); \
  RUN(test_fmt_iso8601); \
  RUN(test_fmt_utf8); \
  RUN(test_fmt_asn1derlength); \
  RUN(test_fmt_asn1dertag); \
  RUN(test_fmt_tohex); \
  RUN(test_fmt_longlong_family); \
  RUN(test_fmt_xlong0_family); \
  RUN(test_fmt_hexb); \
  RUN(test_fmt_repeat); \
  RUN(test_fmt_escapecharc); \
  RUN(test_fmt_escapecharcx); \
  RUN(test_fmt_escapecharcontrol); \
  RUN(test_fmt_escapecharnonprintable); \
  RUN(test_fmt_escapecharshell); \
  RUN(test_fmt_escapecharquotedshell); \
  RUN(test_fmt_escapechardoublequotedshell); \
  RUN(test_fmt_escapecharjson); \
  RUN(test_fmt_xmlescape); \
  RUN(test_fmt_escapecharhtml); \
  RUN(test_fmt_escapecharquotedprintable)

TESTS(fmt) { RUN_FMT_TESTS(); }
