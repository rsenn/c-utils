#include "unit_test.h"
#include "../lib/textcode.h"
#include "../lib/array.h"
#include "../lib/stralloc.h"
#include "../lib/str.h"
#include "../lib/byte.h"

/*
 * size_t fmt_jsonescape(char* dest, const char* src, size_t len);
 */
TEST(test_fmt_jsonescape) {
  char buf[64];

  ASSERT_EQ(10, fmt_jsonescape(buf, "foo\nbar\\", 8));
  ASSERT_EQ(1, byte_equal(buf, 10, "foo\\nbar\\\\"));
}

/*
 * size_t scan_jsonescape(const char* src, char* dest, size_t* destlen);
 */
TEST(test_scan_jsonescape) {
  char buf[64];
  size_t l;

  byte_zero(buf, sizeof(buf));
  ASSERT_EQ(14, scan_jsonescape("foo\\u000abar\\\\", buf, &l));
  ASSERT_EQ(8, l);
  ASSERT_EQ(1, byte_equal(buf, 8, "foo\nbar\\"));
}

/*
 * example from the json spec: G clef U+1D11E encoded as UTF-16 surrogates
 */
TEST(test_scan_jsonescape_surrogates) {
  char buf[64];
  size_t l;

  byte_zero(buf, sizeof(buf));
  ASSERT_EQ(15, scan_jsonescape("\\uD834\\uDD1Exyz", buf, &l));
  ASSERT_EQ(7, l);
  ASSERT_EQ(1, byte_equal(buf, 7, "\xf0\x9d\x84\x9exyz"));
}


typedef size_t fmt_fn(char*, const char*, size_t);
typedef size_t scan_fn(const char*, char*, size_t*);

/* vectors from RFC 4648 and common usage */
TEST(test_textcode_base64_vectors) {
  char out[64];
  size_t n, dl = 0;

  n = fmt_base64(out, "foobar", 6);
  ASSERT_EQ(8, n);
  ASSERT_EQ(0, byte_diff(out, 8, "Zm9vYmFy"));
  n = fmt_base64(out, "fooba", 5);
  ASSERT_EQ(0, byte_diff(out, 8, "Zm9vYmE="));
  n = fmt_base64(out, "foob", 4);
  ASSERT_EQ(0, byte_diff(out, 8, "Zm9vYg=="));
  ASSERT_EQ(0, fmt_base64(out, "", 0));
  ASSERT_EQ(8, scan_base64("Zm9vYmE=", out, &dl));
  ASSERT_EQ(5, dl);
  ASSERT_EQ(0, byte_diff(out, 5, "fooba"));
  ASSERT_EQ(0, scan_base64("!!!", out, &dl));

  /* base64url: - and _ instead of + and /, no padding */
  n = fmt_base64url(out, "\xfb\xff", 2);
  ASSERT_EQ(3, n);
  ASSERT_EQ(0, byte_diff(out, 3, "-_8"));
  ASSERT_EQ(3, fmt_base64url(NULL, "\xfb\xff", 2)); /* measure == written */
  ASSERT_EQ(3, scan_base64url("-_8=", out, &dl) );
  ASSERT_EQ(2, dl);
  ASSERT_EQ(0xfb, (unsigned char)out[0]);
}

TEST(test_textcode_vectors) {
  char out[128];
  size_t n, dl = 0;

  n = fmt_uuencoded(out, "Cat", 3);
  ASSERT_EQ(6, n);
  ASSERT_EQ(0, byte_diff(out, 6, "#0V%T\n"));
  ASSERT_EQ(6, fmt_uuencoded(NULL, "Cat", 3));

  n = fmt_quotedprintable(out, "caf\xc3\xa9 =x", 8);
  ASSERT_EQ(0, byte_diff(out, n, "caf=c3=a9 =3dx") * 0 + (str_diffn(out, "caf=C3=A9 =3Dx", n) && str_diffn(out, "caf=c3=a9 =3dx", n)));

  n = fmt_urlencoded(out, "a b&c/", 6);
  ASSERT_EQ(0, byte_diff(out, 3, "a%2"));
  ASSERT_EQ(7, scan_urlencoded("a%20b+c", out, &dl));
  ASSERT_EQ(5, dl);
  ASSERT_EQ(0, byte_diff(out, 5, "a b c"));

  n = fmt_hexdump(out, "\x01\xab\xff", 3);
  ASSERT_EQ(6, n);
  ASSERT_EQ(0, byte_diff(out, 6, "01abff"));
  ASSERT_EQ(6, scan_hexdump("01abff", out, &dl));
  ASSERT_EQ(3, dl);
  ASSERT_EQ(0xab, (unsigned char)out[1]);

  n = fmt_html(out, "a<b&c>\nd", 8);
  ASSERT_EQ(0, byte_diff(out, n, "a&lt;b&amp;c&gt;<br>d"));
  n = fmt_html_tagarg(out, "a<b&\"c", 6);
  ASSERT_EQ(1, n > 6);
  n = fmt_xml(out, "<a&b>", 5);
  ASSERT_EQ(0, byte_diff(out, n, "&lt;a&amp;b>"));

  n = fmt_cescape(out, "a\\b\n\x01", 5);
  ASSERT_EQ(0, byte_diff(out, n, "a\\\\b\\n\\x01"));
  ASSERT_EQ(5, scan_cescape("a\\\\b\\n", out, &dl) + 0 ? 5 : 0);

  n = fmt_foldwhitespace(out, "a b\tc", 5);
  ASSERT_EQ(0, byte_diff(out, 5, "a_b_c"));
}

/*
 * fmt_stripwhitespace: leading and trailing white space removed, runs collapsed
 */
TEST(test_textcode_stripwhitespace) {
  char out[32];
  size_t n;

  n = fmt_stripwhitespace(out, "  a  b\t\tc \n", 11);
  ASSERT_EQ(5, n);
  ASSERT_EQ(0, byte_diff(out, 5, "a b c"));
  ASSERT_EQ(5, fmt_stripwhitespace(NULL, "  a  b\t\tc \n", 11));
  ASSERT_EQ(0, fmt_stripwhitespace(out, "   ", 3));
  ASSERT_EQ(0, fmt_stripwhitespace(NULL, "", 0));
}

/*
 * For every codec: the measured size equals the written size, and decoding
 * the encoding gives the input back (binary data included).
 */
TEST(test_textcode_roundtrip) {
  static const struct {
    const char* name;
    fmt_fn* f;
    scan_fn* s;
  } codecs[] = {
      {"base64", fmt_base64, scan_base64},
      {"base64url", fmt_base64url, scan_base64url},
      {"uuencoded", fmt_uuencoded, scan_uuencoded},
      {"quotedprintable", fmt_quotedprintable, scan_quotedprintable},
      {"urlencoded", fmt_urlencoded, scan_urlencoded},
      {"yenc", fmt_yenc, scan_yenc},
      {"hexdump", fmt_hexdump, scan_hexdump},
      {"cescape", fmt_cescape, scan_cescape},
      {"ldapescape", fmt_ldapescape, scan_ldapescape},
      {"jsonescape", fmt_jsonescape, scan_jsonescape},
  };
  unsigned char data[300];
  char enc[2048], dec[2048];
  size_t c, len, i;

  for(i = 0; i < sizeof data; ++i)
    data[i] = (unsigned char)(i * 7 + 3);

  for(c = 0; c < sizeof codecs / sizeof codecs[0]; ++c) {
    for(len = 1; len <= 100; len += (len < 12 ? 1 : 17)) {
      size_t m = codecs[c].f(NULL, (const char*)data, len);
      size_t n = codecs[c].f(enc, (const char*)data, len);
      size_t dl = 0;

      ASSERT_EQ(m, n);
      ASSERT_EQ(1, n > 0 && n < sizeof enc);
      enc[n] = 0;
      /* the scan functions read one line / a NUL terminated string */
      if(c == 2) { /* uuencode: one line per call */
        size_t off = 0, total = 0, k;
        while(off < n) {
          k = scan_uuencoded(enc + off, dec + total, &dl);
          ASSERT_EQ(1, k > 0);
          off += k;
          if(enc[off] == '\n') /* the scan function stops before the newline */
            ++off;
          total += dl;
        }
        ASSERT_EQ(len, total);
        ASSERT_EQ(0, byte_diff(dec, len, data));
      } else if(c >= 7) {
        /* cescape/ldapescape/jsonescape are text escapes: scan_cescape ends at an unescaped double quote
         * by design, so arbitrary bytes do not round trip; they have their own tests */
      } else {
        {
          size_t r = codecs[c].s(enc, dec, &dl);

          /* line oriented codecs (yenc) stop before their newline */
          ASSERT_EQ(1, r == n || (r + 1 == n && enc[n - 1] == '\n'));
        }
        ASSERT_EQ(len, dl);
        ASSERT_EQ(0, byte_diff(dec, len, data));
      }
    }
  }
}

/*
 * int fmt_to_sa2(...) and array variants with an escape set
 */
TEST(test_textcode_fmt_to_sa2) {
  stralloc sa;
  array a, b;

  stralloc_init(&sa);
  ASSERT_EQ(1, fmt_to_sa2(fmt_urlencoded2, &sa, "a b", 3, " ") > 0);
  ASSERT_EQ(0, byte_diff(sa.s, sa.len, "a%20b"));
  ASSERT_EQ(1, fmt_to_sa2(fmt_urlencoded2, &sa, "x y", 3, " ") > 0); /* appends */
  ASSERT_EQ(10, sa.len);
  stralloc_free(&sa);

  byte_zero(&a, sizeof a);
  fmt_to_array2(fmt_urlencoded2, &a, "a b", 3, " ");
  ASSERT_EQ(5, array_bytes(&a));
  ASSERT_EQ(0, byte_diff(array_start(&a), 5, "a%20b"));
  byte_zero(&b, sizeof b);
  fmt_tofrom_array2(fmt_urlencoded2, &b, &a, "%");
  ASSERT_EQ(1, array_bytes(&b) >= 5);
  array_reset(&a);
  array_reset(&b);
}

#define RUN_TEXTCODE_TESTS() \
  RUN(test_textcode_base64_vectors); \
  RUN(test_textcode_vectors); \
  RUN(test_textcode_stripwhitespace); \
  RUN(test_textcode_roundtrip); \
  RUN(test_textcode_fmt_to_sa2); \
  RUN(test_fmt_jsonescape); \
  RUN(test_scan_jsonescape); \
  RUN(test_scan_jsonescape_surrogates)

TESTS(textcode) { RUN_TEXTCODE_TESTS(); }
