#include "unit_test.h"
#include "../lib/utf8.h"
#include "../lib/stralloc.h"
#include "../lib/str.h"
#include "../lib/byte.h"

/* U+00E4 (a diaeresis) 2 bytes, U+20AC (euro) 3 bytes, U+1F600 (grinning face) 4 bytes */
#define AE "\xc3\xa4"
#define EURO "\xe2\x82\xac"
#define FACE "\xf0\x9f\x98\x80"

/*
 * size_t u8_len(const char*, size_t);
 */
TEST(test_u8_len) {
  ASSERT_EQ(1, u8_len("a", 1));
  ASSERT_EQ(2, u8_len(AE, 1));
  ASSERT_EQ(3, u8_len(EURO, 1));
  ASSERT_EQ(4, u8_len(FACE, 1));

  /* a count of 0, the end of the string and a byte that starts nothing give 0 */
  ASSERT_EQ(0, u8_len("a", 0));
  ASSERT_EQ(0, u8_len("", 1));
  ASSERT_EQ(0, u8_len("\x80", 1));
  ASSERT_EQ(0, u8_len("\xff", 1));
}

/*
 * size_t wc_u8len(const wchar_t);
 */
TEST(test_wc_u8len) {
  ASSERT_EQ(1, wc_u8len(0));
  ASSERT_EQ(1, wc_u8len(0x7f));
  ASSERT_EQ(2, wc_u8len(0x80));
  ASSERT_EQ(2, wc_u8len(0x7ff));
  ASSERT_EQ(3, wc_u8len(0x800));
  ASSERT_EQ(3, wc_u8len(0xffff));
  ASSERT_EQ(4, wc_u8len(0x10000));
  ASSERT_EQ(4, wc_u8len(0x10ffff));
  ASSERT_EQ(0, wc_u8len(0x110000));
  ASSERT_EQ(0, wc_u8len(0x200000));
  ASSERT_EQ(0, wc_u8len((wchar_t)-1));
}

/*
 * size_t wc_to_u8(char*, wchar_t);
 */
TEST(test_wc_to_u8) {
  char buf[8];

  ASSERT_EQ(1, wc_to_u8(buf, 'A'));
  ASSERT_STR_EQUAL("A", buf);

  ASSERT_EQ(2, wc_to_u8(buf, 0xe4));
  ASSERT_STR_EQUAL(AE, buf);

  ASSERT_EQ(3, wc_to_u8(buf, 0x20ac));
  ASSERT_STR_EQUAL(EURO, buf);

  ASSERT_EQ(4, wc_to_u8(buf, 0x1f600));
  ASSERT_STR_EQUAL(FACE, buf);

  /* it stores nothing for a value that has no encoding */
  ASSERT_EQ(4, wc_to_u8(buf, 0x10ffff));
  ASSERT_EQ(0, wc_to_u8(buf, 0x110000));
  ASSERT_EQ(0, wc_to_u8(buf, 0x200000));
}

/*
 * size_t u8_to_wc(wchar_t*, const char*);
 */
TEST(test_u8_to_wc) {
  wchar_t w = 0;

  ASSERT_EQ(1, u8_to_wc(&w, "A"));
  ASSERT_EQ('A', w);
  ASSERT_EQ(2, u8_to_wc(&w, AE));
  ASSERT_EQ(0xe4, w);
  ASSERT_EQ(3, u8_to_wc(&w, EURO));
  ASSERT_EQ(0x20ac, w);
  ASSERT_EQ(4, u8_to_wc(&w, FACE));
  ASSERT_EQ(0x1f600, w);

  /* the limits of every length */
  ASSERT_EQ(2, u8_to_wc(&w, "\xc2\x80"));
  ASSERT_EQ(0x80, w);
  ASSERT_EQ(2, u8_to_wc(&w, "\xdf\xbf"));
  ASSERT_EQ(0x7ff, w);
  ASSERT_EQ(3, u8_to_wc(&w, "\xe0\xa0\x80"));
  ASSERT_EQ(0x800, w);
  ASSERT_EQ(3, u8_to_wc(&w, "\xef\xbf\xbf"));
  ASSERT_EQ(0xffff, w);
  ASSERT_EQ(4, u8_to_wc(&w, "\xf0\x90\x80\x80"));
  ASSERT_EQ(0x10000, w);
  ASSERT_EQ(4, u8_to_wc(&w, "\xf4\x8f\xbf\xbf"));
  ASSERT_EQ(0x10ffff, w);
}

/* what is not valid UTF-8 gives 0 and leaves the output alone */
TEST(test_u8_to_wc_invalid) {
  wchar_t w = 0x1234;

  ASSERT_EQ(0, u8_to_wc(&w, ""));             /* end of the string */
  ASSERT_EQ(0, u8_to_wc(&w, "\x80"));         /* continuation byte alone */
  ASSERT_EQ(0, u8_to_wc(&w, "\xff"));         /* never valid */
  ASSERT_EQ(0, u8_to_wc(&w, "\xc3"));         /* cut off by the end */
  ASSERT_EQ(0, u8_to_wc(&w, "\xc3z"));        /* wrong continuation */
  ASSERT_EQ(0, u8_to_wc(&w, "\xe2\x82"));     /* three bytes, two there */
  ASSERT_EQ(0, u8_to_wc(&w, "\xf0\x9f\x98")); /* four bytes, three there */
  ASSERT_EQ(0, u8_to_wc(&w, "\xc0\x80"));     /* overlong NUL */
  ASSERT_EQ(0, u8_to_wc(&w, "\xc1\xbf"));     /* overlong 0x7f */
  ASSERT_EQ(0, u8_to_wc(&w, "\xe0\x80\x80")); /* overlong */
  ASSERT_EQ(0, u8_to_wc(&w, "\xf0\x80\x80\x80"));
  ASSERT_EQ(0, u8_to_wc(&w, "\xed\xa0\x80"));     /* U+D800, a surrogate */
  ASSERT_EQ(0, u8_to_wc(&w, "\xed\xbf\xbf"));     /* U+DFFF */
  ASSERT_EQ(0, u8_to_wc(&w, "\xf4\x90\x80\x80")); /* U+110000 */
  ASSERT_EQ(0x1234, w);
}

/*
 * size_t u8s_len(const char*);
 * size_t u8b_len(const char*, size_t);
 */
TEST(test_u8_len_chars) {
  ASSERT_EQ(0, u8s_len(""));
  ASSERT_EQ(5, u8s_len("hello"));
  ASSERT_EQ(4, u8s_len("a" AE EURO FACE));
  ASSERT_EQ(4, u8b_len("a" AE EURO FACE, 10));
  ASSERT_EQ(2, u8b_len("abc", 2));
  ASSERT_EQ(0, u8b_len("abc", 0));

  /* a byte that starts nothing counts for its entity "&#xFF;" */
  ASSERT_EQ(6, u8s_len("\xff"));
  ASSERT_EQ(7, u8s_len("a\x80"));
  /* a sequence that is cut off must not make it read beyond the end */
  ASSERT_EQ(6 + 6, u8s_len("\xe2\x82"));
}

TEST(test_u8b_len_cut_off) {
  /* the last character needs three bytes but only two are counted */
  ASSERT_EQ(1 + 6 + 6, u8b_len("a" EURO, 3));
  ASSERT_EQ(2, u8b_len("a" EURO, 4));
}

/*
 * size_t wcs_u8slen(const wchar_t*);
 */
TEST(test_wcs_u8slen) {
  static const wchar_t ok[] = {'a', 0xe4, 0x20ac, 0x1f600, 0};
  static const wchar_t bad[] = {'a', 0x200000, 0};

  ASSERT_EQ(0, wcs_u8slen(L""));
  ASSERT_EQ(1 + 2 + 3 + 4, wcs_u8slen(ok));
  /* a value that has no encoding is written as "&#x00;" */
  ASSERT_EQ(1 + 6, wcs_u8slen(bad));
}

/*
 * size_t wcs_to_u8s(char*, const wchar_t*, size_t);
 */
TEST(test_wcs_to_u8s) {
  static const wchar_t in[] = {'a', 0xe4, 0x20ac, 0x1f600, 0};
  static const wchar_t bad[] = {'a', 0x200000, 'b', 0};
  char out[32];

  ASSERT_EQ(10, wcs_to_u8s(0, in, 0));

  byte_zero(out, sizeof(out));
  ASSERT_EQ(10, wcs_to_u8s(out, in, sizeof(out)));
  ASSERT_STR_EQUAL("a" AE EURO FACE, out);

  byte_zero(out, sizeof(out));
  ASSERT_EQ(8, wcs_to_u8s(out, bad, sizeof(out)));
  ASSERT_STR_EQUAL("a&#x00;b", out);

  /* it stops before a character that does not fit, and never writes beyond count */
  byte_fill(out, sizeof(out), 'X');
  ASSERT_EQ(6, wcs_to_u8s(out, in, 6));
  ASSERT_EQ(0, byte_diff(out, 6, "a" AE EURO));
  ASSERT_EQ('X', out[6]);

  byte_fill(out, sizeof(out), 'X');
  ASSERT_EQ(0, wcs_to_u8s(out, in, 0));
  ASSERT_EQ('X', out[0]);
}

/*
 * size_t wcs_to_u8sa(stralloc*, const wchar_t*);
 */
TEST(test_wcs_to_u8sa) {
  static const wchar_t in[] = {'a', 0xe4, 0x20ac, 0x1f600, 0};
  static const wchar_t bad[] = {0x200000, 0};
  stralloc sa;

  stralloc_init(&sa);
  ASSERT_EQ(10, wcs_to_u8sa(0, in));

  ASSERT_EQ(10, wcs_to_u8sa(&sa, in));
  ASSERT_EQ(10, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "a" AE EURO FACE));

  /* it appends */
  ASSERT_EQ(6, wcs_to_u8sa(&sa, bad));
  ASSERT_EQ(16, sa.len);
  ASSERT_EQ(0, byte_diff(sa.s + 10, 6, "&#x00;"));

  stralloc_free(&sa);
}

/*
 * size_t u8s_to_wcs(wchar_t*, const char*, size_t);
 */
TEST(test_u8s_to_wcs) {
  wchar_t out[16];

  ASSERT_EQ(4, u8s_to_wcs(0, "a" AE EURO FACE, 0));

  ASSERT_EQ(5, u8s_to_wcs(out, "a" AE EURO FACE, 16));
  ASSERT_EQ('a', out[0]);
  ASSERT_EQ(0xe4, out[1]);
  ASSERT_EQ(0x20ac, out[2]);
  ASSERT_EQ(0x1f600, out[3]);
  ASSERT_EQ(0, out[4]);

  /* without room for the terminator it is not stored */
  ASSERT_EQ(4, u8s_to_wcs(out, "a" AE EURO FACE, 4));
  /* and it stops at count */
  ASSERT_EQ(2, u8s_to_wcs(out, "abcdef", 2));

  ASSERT_EQ(1, u8s_to_wcs(out, "", 4));
  ASSERT_EQ(0, out[0]);
}

/* invalid input is an error, not an endless or wrong conversion */
TEST(test_u8s_to_wcs_invalid) {
  wchar_t out[16];

  ASSERT_EQ((size_t)-1,
            u8s_to_wcs(out,
                       "ab\xff"
                       "cd",
                       16));
  ASSERT_EQ((size_t)-1, u8s_to_wcs(out, "ab\xe2\x82", 16));
  ASSERT_EQ((size_t)-1, u8s_to_wcs(out, "\xc0\x80", 16));
}

/*
 * size_t u8b_to_wcs(wchar_t*, const char*, size_t, size_t);
 */
TEST(test_u8b_to_wcs) {
  wchar_t out[16];
  static const char text[] = "a" AE EURO FACE;

  ASSERT_EQ(4, u8b_to_wcs(0, text, sizeof(text) - 1, 0));

  ASSERT_EQ(5, u8b_to_wcs(out, text, sizeof(text) - 1, 16));
  ASSERT_EQ(0x1f600, out[3]);
  ASSERT_EQ(0, out[4]);

  ASSERT_EQ(2, u8b_to_wcs(out, text, sizeof(text) - 1, 2));

  /* the bytes are not terminated: nothing beyond n is looked at */
  {
    char raw[3] = {'a', (char)0xe2, (char)0x82};

    ASSERT_EQ((size_t)-1, u8b_to_wcs(out, raw, 3, 16));
    ASSERT_EQ(2, u8b_to_wcs(out, raw, 1, 16));
  }

  ASSERT_EQ((size_t)-1, u8b_to_wcs(out, "\xff", 1, 16));
}

/*
 * size_t u8s_chrs(const char*, const char[], size_t);
 * size_t u8s_rchrs(const char*, const char[], size_t);
 * size_t u8b_chrs(const char*, size_t, const char[], size_t);
 * size_t u8b_rchrs(const char*, size_t, const char[], size_t);
 */
TEST(test_u8_chrs) {
  const char text[] = "ab" AE "cd" AE "e";
  const size_t n = sizeof(text) - 1;
  const char needles[] = AE "d";
  const size_t nn = sizeof(needles) - 1;

  /* positions are byte offsets */
  ASSERT_EQ(2, u8s_chrs(text, needles, nn));
  ASSERT_EQ(2, u8b_chrs(text, n, needles, nn));
  ASSERT_EQ(6, u8s_rchrs(text, needles, nn));
  ASSERT_EQ(6, u8b_rchrs(text, n, needles, nn));

  /* a single byte needle */
  ASSERT_EQ(1, u8s_chrs(text, "b", 1));
  ASSERT_EQ(1, u8s_rchrs(text, "b", 1));

  /* not found: the length */
  ASSERT_EQ(n, u8s_chrs(text, "xyz", 3));
  ASSERT_EQ(n, u8b_chrs(text, n, "xyz", 3));
  ASSERT_EQ(n, u8s_rchrs(text, "xyz", 3));
  ASSERT_EQ(n, u8b_rchrs(text, n, "xyz", 3));

  /* the bytes of a needle are not a character of their own */
  ASSERT_EQ(4, u8s_chrs("x" EURO, "\x82", 1));

  /* the range limits u8b: the match at 2 is not in the first two bytes */
  ASSERT_EQ(2, u8b_chrs(text, 2, needles, nn));
  ASSERT_EQ(0, u8s_chrs("", "a", 1));
}

/* invalid bytes must not make the search run for ever */
TEST(test_u8_chrs_invalid) {
  ASSERT_EQ(3,
            u8s_chrs("\xff\xfe"
                     "x"
                     "y",
                     "y",
                     1));
  ASSERT_EQ(4,
            u8b_chrs("\xff\xfe"
                     "xy",
                     4,
                     "q",
                     1));
  ASSERT_EQ(3,
            u8s_rchrs("\xff\xfe"
                      "xy",
                      "y",
                      1));
  ASSERT_EQ(4,
            u8b_rchrs("\xff\xfe"
                      "xy",
                      4,
                      "q",
                      1));
  /* a needle cut off at the end of the list */
  ASSERT_EQ(3, u8s_chrs("abc", "\xc3", 1));
}

/*
 * int u8s_diff(const char*, const char*);
 * int u8b_diff(const char*, size_t, const char*, size_t);
 */
TEST(test_u8_diff) {
  ASSERT_EQ(0, u8s_diff("", ""));
  ASSERT_EQ(0, u8s_diff("abc" AE, "abc" AE));
  ASSERT_LT(u8s_diff("abc", "abd"), 0);
  ASSERT_GT(u8s_diff("abd", "abc"), 0);
  ASSERT_LT(u8s_diff("ab", "abc"), 0);
  ASSERT_GT(u8s_diff("abc", "ab"), 0);
  /* compared by character, not by byte */
  ASSERT_EQ(0xe4 - 'a', u8s_diff(AE, "a"));
  ASSERT_LT(u8s_diff(AE, EURO), 0);

  /* invalid bytes are compared as they are, the comparison ends */
  ASSERT_EQ(0, u8s_diff("a\xff", "a\xff"));
  ASSERT_NE(0, u8s_diff("a\xff", "a\xfe"));

  ASSERT_EQ(0, u8b_diff("abc", 3, "abc", 3));
  ASSERT_EQ(0, u8b_diff("", 0, "", 0));
  ASSERT_LT(u8b_diff("abc", 3, "abd", 3), 0);
  ASSERT_LT(u8b_diff("ab", 2, "abc", 3), 0);
  ASSERT_GT(u8b_diff("abc", 3, "ab", 2), 0);
  ASSERT_EQ(0, u8b_diff("abc" AE, 5, "abc" AE, 5));
  /* the lengths decide, not what follows them */
  ASSERT_EQ(0, u8b_diff("abcX", 3, "abcY", 3));
  ASSERT_EQ(0, u8b_diff("a\xff", 2, "a\xff", 2));
}

/*
 * int wc_charwidth(wchar_t);
 */
TEST(test_wc_charwidth) {
  /* control characters and combining marks take no column */
  ASSERT_EQ(0, wc_charwidth(0));
  ASSERT_EQ(0, wc_charwidth(0x1b));
  ASSERT_EQ(0, wc_charwidth(0x0300));
  ASSERT_EQ(0, wc_charwidth(0x200b));

  ASSERT_EQ(1, wc_charwidth('a'));
  ASSERT_EQ(1, wc_charwidth(0xe4));
  ASSERT_EQ(1, wc_charwidth(0x20ac));

  /* wide characters take two */
  ASSERT_EQ(2, wc_charwidth(0x4e2d));  /* CJK */
  ASSERT_EQ(2, wc_charwidth(0xac00));  /* Hangul */
  ASSERT_EQ(2, wc_charwidth(0xff21));  /* fullwidth A */
  ASSERT_EQ(2, wc_charwidth(0x1f600)); /* emoji */
}

/* a round trip through both directions gives the text back */
TEST(test_u8_roundtrip) {
  static const char text[] = "Gr" AE "\xc3\x9f"
                             "e " EURO " " FACE " end";
  wchar_t w[64];
  char back[128];
  size_t n;

  n = u8s_to_wcs(w, text, 64);
  ASSERT_NE((size_t)-1, n);
  ASSERT_EQ(u8s_len(text) + 1, n);

  byte_zero(back, sizeof(back));
  ASSERT_EQ(sizeof(text) - 1, wcs_to_u8s(back, w, sizeof(back)));
  ASSERT_STR_EQUAL(text, back);
}

#define RUN_UTF8_TESTS() \
  RUN(test_u8_len); \
  RUN(test_wc_u8len); \
  RUN(test_wc_to_u8); \
  RUN(test_u8_to_wc); \
  RUN(test_u8_to_wc_invalid); \
  RUN(test_u8_len_chars); \
  RUN(test_u8b_len_cut_off); \
  RUN(test_wcs_u8slen); \
  RUN(test_wcs_to_u8s); \
  RUN(test_wcs_to_u8sa); \
  RUN(test_u8s_to_wcs); \
  RUN(test_u8s_to_wcs_invalid); \
  RUN(test_u8b_to_wcs); \
  RUN(test_u8_chrs); \
  RUN(test_u8_chrs_invalid); \
  RUN(test_u8_diff); \
  RUN(test_wc_charwidth); \
  RUN(test_u8_roundtrip)

TESTS(utf8) { RUN_UTF8_TESTS(); }
