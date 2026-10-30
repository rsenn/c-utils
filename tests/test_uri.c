#include "unit_test.h"
#include "../lib/uri.h"
#include "../lib/alloc.h"
#include "../lib/str.h"

/*
 * size_t uri_scan(uri_t*, const char*, size_t);
 */
TEST(test_uri_scan) {
  uri_t u;
  const char* s = "https://user:pw@example.com:8443/a/b.html#top";

  uri_init(&u);

  ASSERT_EQ(45, uri_scan(&u, s, str_len(s)));
  ASSERT_STR_EQUAL("https", u.proto);
  ASSERT_STR_EQUAL("user", u.username);
  ASSERT_STR_EQUAL("pw", u.password);
  ASSERT_STR_EQUAL("example.com", u.host);
  ASSERT_EQ(8443, u.port);
  ASSERT_STR_EQUAL("/a/b.html", u.location);
  ASSERT_STR_EQUAL("#top", u.anchor);

  uri_free(&u);
}

/*
 * default port, and an absolute path keeps the host
 */
TEST(test_uri_scan_relative) {
  uri_t u;
  const char* s = "http://example.com/dir/page.html";

  uri_init(&u);
  uri_scan(&u, s, str_len(s));
  ASSERT_EQ(80, u.port);

  uri_scan(&u, "/other.html", 11);
  ASSERT_STR_EQUAL("/other.html", u.location);
  ASSERT_STR_EQUAL("example.com", u.host);

  uri_free(&u);
}

/*
 * size_t uri_fmt(char*, const uri_t*);
 */
TEST(test_uri_fmt) {
  uri_t u;
  char buf[128];
  const char* s = "https://user:pw@example.com:8443/a/b.html#top";
  size_t n;

  uri_init(&u);
  uri_scan(&u, s, str_len(s));

  ASSERT_EQ(45, uri_fmt(NULL, &u));
  n = uri_fmt(buf, &u);
  buf[n] = '\0';
  ASSERT_STR_EQUAL(s, buf);

  /* default port is left out */
  uri_free(&u);
  uri_scan(&u, "http://example.com/x", 20);
  n = uri_fmt(buf, &u);
  buf[n] = '\0';
  ASSERT_STR_EQUAL("http://example.com/x", buf);

  uri_free(&u);
}

/*
 * char* uri_str(const uri_t*);
 */
TEST(test_uri_str) {
  uri_t u;
  char* s;

  uri_init(&u);
  uri_scan(&u, "ftp://host/file", 15);
  s = uri_str(&u);

  ASSERT_STR_EQUAL("ftp://host/file", s);

  alloc_free(s);
  uri_free(&u);
}

/*
 * size_t uri_find(const char*, size_t);
 */
TEST(test_uri_find) {
  const char* s = "see <a href=\"http://x.org/\">";

  ASSERT_EQ(13, uri_find(s, str_len(s)));
  ASSERT_EQ(5, uri_find("abcde", 5));
}

/*
 * void uri_copy(uri_t*, const uri_t*);
 */
TEST(test_uri_copy) {
  uri_t a, b;

  uri_init(&a);
  uri_scan(&a, "http://example.com/p#a", 22);
  uri_copy(&b, &a);

  ASSERT_STR_EQUAL("example.com", b.host);
  ASSERT_STR_EQUAL("/p", b.location);
  ASSERT_STR_EQUAL("#a", b.anchor);
  ASSERT_NE(a.host, b.host);

  uri_clear_anchor(&b);
  ASSERT_EQ(NULL, b.anchor);

  uri_free(&a);
  uri_free(&b);
}

#define RUN_URI_TESTS() \
  RUN(test_uri_scan); \
  RUN(test_uri_scan_relative); \
  RUN(test_uri_fmt); \
  RUN(test_uri_str); \
  RUN(test_uri_find); \
  RUN(test_uri_copy)

TESTS(uri) { RUN_URI_TESTS(); }
