#include "unit_test.h"
#include "../lib/stralloc.h"
#include "../lib/ucs.h"
#include "../lib/str.h"
#include "../lib/byte.h"

/*
 * wchar_t ucs_from_latin1_char(int); char ucs_to_latin1_char(wchar_t);
 */
TEST(test_ucs_latin1_char) {
  ASSERT_EQ('A', ucs_from_latin1_char('A'));
  ASSERT_EQ(0xe9, ucs_from_latin1_char(0xe9));
  ASSERT_EQ(0xff, ucs_from_latin1_char(0xff));
  ASSERT_EQ(0xe9, (unsigned char)ucs_to_latin1_char(0xe9));
  ASSERT_EQ(0, ucs_to_latin1_char(0x20ac)); /* euro sign is not in Latin-1 */
}

/*
 * size_t scan_utf8_ucs4(const char*, size_t, uint32*);
 * size_t fmt_ucs4_utf8(char*, uint32);
 */
TEST(test_ucs_utf8_ucs4) {
  uint32 u = 0;
  char buf[8];
  size_t n;

  ASSERT_EQ(1, scan_utf8_ucs4("A", 1, &u));
  ASSERT_EQ('A', u);
  ASSERT_EQ(3, scan_utf8_ucs4("\xe2\x82\xac", 3, &u));
  ASSERT_EQ(0x20ac, u);
  ASSERT_EQ(4, scan_utf8_ucs4("\xf0\x9f\x98\x80", 4, &u));
  ASSERT_EQ(0x1f600, u);
  ASSERT_EQ(0, scan_utf8_ucs4("\xc0\x80", 2, &u));         /* overlong */
  ASSERT_EQ(0, scan_utf8_ucs4("\xe2\x82", 2, &u));         /* truncated */
  ASSERT_EQ(0, scan_utf8_ucs4("\xed\xa0\x80", 3, &u));     /* surrogate */
  ASSERT_EQ(0, scan_utf8_ucs4("", 0, &u));

  ASSERT_EQ(1, fmt_ucs4_utf8(buf, 'z'));
  ASSERT_EQ(2, fmt_ucs4_utf8(buf, 0xe9));
  ASSERT_EQ(0, byte_diff(buf, 2, "\xc3\xa9"));
  ASSERT_EQ(4, fmt_ucs4_utf8(NULL, 0x1f600));
  ASSERT_EQ(0, fmt_ucs4_utf8(buf, 0x110000)); /* beyond Unicode */
  ASSERT_EQ(0, fmt_ucs4_utf8(buf, 0xd800));   /* surrogate */

  /* round trip over the whole range, sampled */
  for(u = 0; u < 0x110000; u += 97) {
    uint32 v = 0;

    if(u >= 0xd800 && u <= 0xdfff)
      continue;
    n = fmt_ucs4_utf8(buf, u);
    ASSERT_EQ(1, n >= 1 && n <= 4);
    ASSERT_EQ(n, scan_utf8_ucs4(buf, n, &v));
    ASSERT_EQ(u, v);
  }
}

/*
 * uint32 ucs_tolower(uint32);
 */
TEST(test_ucs_tolower) {
  ASSERT_EQ('a', ucs_tolower('A'));
  ASSERT_EQ('a', ucs_tolower('a'));
  ASSERT_EQ('1', ucs_tolower('1'));
  ASSERT_EQ(0xe9, ucs_tolower(0xc9));   /* E acute */
  ASSERT_EQ(0x436, ucs_tolower(0x416)); /* Cyrillic Zhe */
  ASSERT_EQ(0x3b1, ucs_tolower(0x391)); /* Greek alpha */
}

/*
 * size_t fmt_punycode(char*, const uint32*, size_t);
 * size_t scan_punycode(const char*, size_t, uint32*, size_t* max);
 * (test vectors: RFC 3492 section 7.1 style, and the classic Munich and Buecher)
 */
TEST(test_ucs_punycode) {
  uint32 s[] = {'m', 0xfc, 'n', 'c', 'h', 'e', 'n'};
  uint32 back[16];
  size_t max = 16, n;
  char out[64];

  n = fmt_punycode(out, s, 7);
  ASSERT_EQ(10, n);
  ASSERT_EQ(0, byte_diff(out, 10, "mnchen-3ya"));
  ASSERT_EQ(10, fmt_punycode(NULL, s, 7));

  n = scan_punycode("mnchen-3ya", 10, back, &max);
  ASSERT_EQ(10, n);
  ASSERT_EQ(7, max); /* *max is set to the number of code points decoded */
  ASSERT_EQ(0xfc, back[1]);
  ASSERT_EQ('n', back[6]);

  max = 3; /* too small a buffer */
  ASSERT_EQ(0, scan_punycode("mnchen-3ya", 10, back, &max));

  /* pure ASCII keeps its text and gets a trailing delimiter in the encoded
   * form; decoding must give it back */
  {
    uint32 a[] = {'a', 'b', 'c'};
    max = 16;
    n = fmt_punycode(out, a, 3);
    ASSERT_EQ(1, n >= 3);
    n = scan_punycode(out, n, back, &max);
    ASSERT_EQ(1, n > 0);
    ASSERT_EQ(3, max);
    ASSERT_EQ('c', back[2]);
  }

  /* garbage must be rejected, not decoded */
  max = 16;
  ASSERT_EQ(0, scan_punycode("a-!!!", 5, back, &max));
}

/*
 * size_t ucs_idna_to_ascii / ucs_idna_to_unicode
 */
TEST(test_ucs_idna) {
  char out[64];
  size_t n;
  const char* uni = "b\xc3\xbc" "cher.example"; /* bücher.example, 15 bytes */

  n = ucs_idna_to_ascii(out, sizeof out, uni, 15);
  ASSERT_EQ(21, n);
  ASSERT_EQ(0, byte_diff(out, 21, "xn--bcher-kva.example"));

  n = ucs_idna_to_unicode(out, sizeof out, "xn--bcher-kva.example", 21);
  ASSERT_EQ(15, n);
  ASSERT_EQ(0, byte_diff(out, 15, uni));

  n = ucs_idna_to_ascii(out, sizeof out, "Example.COM", 11);
  ASSERT_EQ(11, n);

  ASSERT_EQ(0, ucs_idna_to_ascii(out, 4, uni, 15)); /* does not fit */
}

/*
 * size_t fmt_latin1_utf8(char*, char); size_t scan_latin1_utf8(...);
 * size_t str_utf8_latin1(char*);
 * stralloc_latin1_to_utf8 / stralloc_utf8_to_latin1 / stralloc_utf8_latin1
 */
TEST(test_ucs_latin1_utf8) {
  char out[8];
  char c = 0;
  char s[] = "caf\xc3\xa9";
  stralloc sa;

  ASSERT_EQ(1, fmt_latin1_utf8(out, 'x'));
  ASSERT_EQ(2, fmt_latin1_utf8(out, (char)0xe9));
  ASSERT_EQ(0, byte_diff(out, 2, "\xc3\xa9"));

  ASSERT_EQ(2, scan_latin1_utf8("\xc3\xa9", 2, &c));
  ASSERT_EQ(0xe9, (unsigned char)c);

  ASSERT_EQ(4, str_utf8_latin1(s));
  ASSERT_EQ(0xe9, (unsigned char)s[3]);
  ASSERT_EQ(4, str_len(s));

  /* invalid UTF-8 is kept byte for byte and must terminate */
  {
    char bad[] = "a\xff" "b";
    ASSERT_EQ(3, str_utf8_latin1(bad));
    ASSERT_EQ(0xff, (unsigned char)bad[1]);
  }

  stralloc_init(&sa);
  ASSERT_EQ(1, stralloc_latin1_to_utf8(&sa, "caf\xe9", 4) > 0);
  ASSERT_EQ(5, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, 5, "caf\xc3\xa9"));
  stralloc_zero(&sa);
  ASSERT_EQ(1, stralloc_utf8_to_latin1(&sa, "caf\xc3\xa9", 5) > 0);
  ASSERT_EQ(4, sa.len);
  ASSERT_EQ(0xe9, (unsigned char)sa.s[3]);
  stralloc_free(&sa);
}

#define RUN_UCS_TESTS() \
  RUN(test_ucs_latin1_char); \
  RUN(test_ucs_utf8_ucs4); \
  RUN(test_ucs_tolower); \
  RUN(test_ucs_punycode); \
  RUN(test_ucs_idna); \
  RUN(test_ucs_latin1_utf8)

TESTS(ucs) { RUN_UCS_TESTS(); }
