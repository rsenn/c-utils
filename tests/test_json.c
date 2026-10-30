#include "unit_test.h"
#include "../lib/json.h"
#include "../lib/charbuf.h"
#include "../lib/stralloc.h"
#include "../lib/str.h"
#include "../lib/open.h"
#include "../lib/buffer.h"
#include <unistd.h>

/* parses text, prints it with the printer and returns the result */
static void
roundtrip(const char* text, json_print_fn* printer, stralloc* out) {
  charbuf cb;
  jsonval* doc;
  int fd;

  fd = open_trunc("test_json.tmp");
  write(fd, text, str_len(text));
  close(fd);

  fd = open_read("test_json.tmp");
  charbuf_init(&cb, (read_fn*)(void*)&read, fd, 2);
  doc = json_read_tree(&cb);
  charbuf_close(&cb);
  unlink("test_json.tmp");

  stralloc_zero(out);

  if(doc) {
    json_tosa(*doc, out, printer);
    json_free(doc);
  }
  stralloc_nul(out);
}

/*
 * jsonval* json_read_tree(charbuf*);
 * void json_tosa(jsonval, stralloc*, json_print_fn*);
 */
TEST(test_json_read_print) {
  stralloc sa;

  stralloc_init(&sa);

  /* the compact printer has no white space and quotes every string */
  roundtrip("{\"a\":1,\"b\":[true,null,\"x\"]}", &json_compact_printer, &sa);
  ASSERT_STR_EQUAL("{\"a\":1,\"b\":[true,null,\"x\"]}", sa.s);

  roundtrip("[\"x\",\"y z\",[\"w\"]]", &json_compact_printer, &sa);
  ASSERT_STR_EQUAL("[\"x\",\"y z\",[\"w\"]]", sa.s);

  roundtrip("\"just a string\"", &json_compact_printer, &sa);
  ASSERT_STR_EQUAL("\"just a string\"", sa.s);

  /* so does the default printer, whatever layout it chooses */
  roundtrip("{\"a\":[\"x\"]}", &json_default_printer, &sa);
  ASSERT_NE(str_len(sa.s), str_chr(sa.s, 'x'));
  ASSERT_EQ(0, str_diffn(sa.s + str_chr(sa.s, 'x') - 1, "\"x\"", 3));

  stralloc_free(&sa);
}

TEST(test_json_string_escapes) {
  stralloc sa;

  stralloc_init(&sa);

  roundtrip("[\"a\\nb\",\"q\\\"q\"]", &json_compact_printer, &sa);
  ASSERT_STR_EQUAL("[\"a\\nb\",\"q\\\"q\"]", sa.s);

  stralloc_free(&sa);
}

TEST(test_json_tostring) {
  jsonval v;
  stralloc sa;
  charbuf cb;
  int fd;

  fd = open_trunc("test_json.tmp");
  write(fd, "{\"k\":[1,\"s\"]}", 13);
  close(fd);

  fd = open_read("test_json.tmp");
  charbuf_init(&cb, (read_fn*)(void*)&read, fd, 2);
  v = *json_read_tree(&cb);
  charbuf_close(&cb);
  unlink("test_json.tmp");

  stralloc_init(&sa);
  ASSERT_STR_EQUAL("{\"k\":[1,\"s\"]}", json_tostring(v, &sa));
  stralloc_free(&sa);
}

#define RUN_JSON_TESTS() \
  RUN(test_json_read_print); \
  RUN(test_json_string_escapes); \
  RUN(test_json_tostring)

TESTS(json) { RUN_JSON_TESTS(); }
