#include "unit_test.h"
#include "../lib/json.h"
#include "../lib/charbuf.h"
#include "../lib/stralloc.h"
#include "../lib/str.h"
#include "../lib/open.h"
#include "../lib/buffer.h"
#include <unistd.h>

/*
 * jsonval* json_read_tree(charbuf*);
 * void json_tosa(jsonval, stralloc*, json_print_fn*);
 */
TEST(test_json_read_print) {
  static const char text[] = "{\"a\":1,\"b\":[true,null,\"x\"]}";
  charbuf cb;
  jsonval* doc;
  stralloc sa;
  int fd;

  fd = open_trunc("test_json.tmp");
  write(fd, text, sizeof(text) - 1);
  close(fd);

  fd = open_read("test_json.tmp");
  charbuf_init(&cb, (read_fn*)(void*)&read, fd, 2);
  doc = json_read_tree(&cb);
  charbuf_close(&cb);
  unlink("test_json.tmp");

  ASSERT_NE(NULL, doc);
  ASSERT_EQ(JSON_OBJECT, doc->type);

  stralloc_init(&sa);
  json_tosa(*doc, &sa, &json_compact_printer);
  stralloc_nul(&sa);
  ASSERT_EQ(0, str_diffn(sa.s, "{ \"a\": 1, \"b\": [ true, null,", 28));

  stralloc_free(&sa);
  json_free(doc);
}

#define RUN_JSON_TESTS() RUN(test_json_read_print)

TESTS(json) { RUN_JSON_TESTS(); }
