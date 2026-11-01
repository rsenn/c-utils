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


/* the decoded text of the string "a" in the document */
static void
string_of(const char* text, stralloc* out) {
  jsonval* doc = json_parse_text(text, str_len(text));
  jsonval a;

  stralloc_zero(out);

  if(doc) {
    a = json_property_get_str(*doc, "a");
    if(a.type == JSON_STRING)
      stralloc_copy(out, &a.stringv);
    json_free(doc);
  }
  stralloc_nul(out);
}

/*
 * the stored string is the text itself
 */
TEST(test_json_string_decode) {
  stralloc sa;

  stralloc_init(&sa);

  string_of("{\"a\":\"x\\ny \\\"q\\\" \\\\ \\/\"}", &sa);
  ASSERT_STR_EQUAL("x\ny \"q\" \\ /", sa.s);

  /* \u escapes and UTF-8 give the same bytes */
  string_of("{\"a\":\"\\u00e4\\u20ac \xc3\xa4\"}", &sa);
  ASSERT_STR_EQUAL("\xc3\xa4\xe2\x82\xac \xc3\xa4", sa.s);

  /* a surrogate pair is one code point, U+1F600 */
  string_of("{\"a\":\"\\ud83d\\ude00!\"}", &sa);
  ASSERT_STR_EQUAL("\xf0\x9f\x98\x80!", sa.s);

  /* a lone surrogate becomes U+FFFD */
  string_of("{\"a\":\"\\ud83dx\"}", &sa);
  ASSERT_STR_EQUAL("\xef\xbf\xbd" "x", sa.s);

  /* an escaped backslash before u00e4 is not an escape of its own */
  string_of("{\"a\":\"\\\\u00e4\"}", &sa);
  ASSERT_STR_EQUAL("\\u00e4", sa.s);

  stralloc_free(&sa);
}

/*
 * what is read can be printed again
 */
TEST(test_json_string_print) {
  stralloc sa;
  jsonval* doc;
  const char* text = "{\"a\":\"back\\\\slash \\\\u00e4 \xc3\xa4 \\n \\\"\"}";

  stralloc_init(&sa);
  doc = json_parse_text(text, str_len(text));
  ASSERT_NE(0, doc);
  json_tosa(*doc, &sa, &json_compact_printer);
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL(text, sa.s);
  json_free(doc);
  stralloc_free(&sa);
}

/*
 * int json_valid(const char*, size_t);
 */
#define VALID(s) json_valid((s), str_len(s))

TEST(test_json_valid) {
  ASSERT_EQ(1, VALID("{\"a\":[1,-2.5e+3,true,false,null,{}],\"b\":\"x\"}"));
  ASSERT_EQ(1, VALID(" [ ] "));
  ASSERT_EQ(1, VALID("0"));
  ASSERT_EQ(1, VALID("\"s\""));

  ASSERT_EQ(0, VALID("{\"a\":"));
  ASSERT_EQ(0, VALID("{\"a\":1,}"));
  ASSERT_EQ(0, VALID("[1 2]"));
  ASSERT_EQ(0, VALID("01"));
  ASSERT_EQ(0, VALID("1."));
  ASSERT_EQ(0, VALID("{'a':1}"));
  ASSERT_EQ(0, VALID("\"a\nb\""));
  ASSERT_EQ(0, VALID("\"\\x\""));
  ASSERT_EQ(0, VALID("\"\\u12g4\""));
  ASSERT_EQ(0, VALID("{} {}"));
  ASSERT_EQ(0, VALID(""));
  ASSERT_EQ(0, VALID("nul"));
}

/*
 * jsonval json_property_path(jsonval, const char*);
 */
TEST(test_json_property_path) {
  static const char gemini[] =
      "{\"candidates\":[{\"content\":{\"parts\":[{\"text\":\"Hi\"},{\"text\":\"there\"}],\"role\":\"model\"},\"index\":0}],"
      "\"usageMetadata\":{\"totalTokenCount\":3},\"a.b\":{\"c\":true}}";
  jsonval* doc = json_parse_text(gemini, sizeof(gemini) - 1);
  jsonval v;

  ASSERT_NE(0, doc);

  v = json_property_path(*doc, "candidates[0].content.parts[0].text");
  ASSERT_EQ(JSON_STRING, v.type);
  ASSERT_EQ(2, v.stringv.len);
  ASSERT_EQ(0, str_diffn(v.stringv.s, "Hi", 2));

  v = json_property_path(*doc, "candidates[0].content.parts[1].text");
  ASSERT_EQ(JSON_STRING, v.type);
  ASSERT_EQ(5, v.stringv.len);

  v = json_property_path(*doc, "usageMetadata.totalTokenCount");
  ASSERT_EQ(JSON_INT, v.type);
  ASSERT_EQ(3, v.intv);

  v = json_property_path(*doc, "[\"a.b\"].c");
  ASSERT_EQ(JSON_BOOL, v.type);

  /* a step that is missing gives undefined, not a crash */
  ASSERT_EQ(JSON_UNDEFINED, json_property_path(*doc, "candidates[1].content").type);
  ASSERT_EQ(JSON_UNDEFINED, json_property_path(*doc, "nope.deeper").type);
  ASSERT_EQ(JSON_UNDEFINED, json_property_path(*doc, "usageMetadata.totalTokenCount.x").type);

  json_free(doc);
}

static unsigned long nd_seen, nd_sum;

static void
nd_value(jsonval* value, void* user) {
  jsonval n = json_property_get_str(*value, "n");

  (void)user;
  nd_seen++;
  if(n.type == JSON_INT)
    nd_sum += (unsigned long)n.intv;
}

/*
 * size_t ndjson_feed(ndjson*, const char*, size_t, ndjson_callback*, void*);
 */
TEST(test_ndjson_feed) {
  static const char stream[] = "{\"n\":1}\n\n{\"n\":2}\r\nnot json\n{\"n\":30}\n{\"n\":4";
  ndjson nd;
  size_t i, got = 0;

  ndjson_init(&nd);
  nd_seen = nd_sum = 0;

  /* one byte at a time must give the same */
  for(i = 0; i < sizeof(stream) - 1; i++)
    got += ndjson_feed(&nd, stream + i, 1, &nd_value, 0);

  ASSERT_EQ(3, got);
  ASSERT_EQ(3, nd_seen);
  ASSERT_EQ(33, nd_sum);
  ASSERT_EQ(1, nd.errors);

  /* the line that was cut off completes with the rest */
  ASSERT_EQ(1, ndjson_feed(&nd, "}\n", 2, &nd_value, 0));
  ASSERT_EQ(37, nd_sum);

  ndjson_free(&nd);
}

#define RUN_JSON_TESTS() \
  RUN(test_json_read_print); \
  RUN(test_json_string_escapes); \
  RUN(test_json_tostring); \
  RUN(test_json_string_decode); \
  RUN(test_json_string_print); \
  RUN(test_json_valid); \
  RUN(test_json_property_path); \
  RUN(test_ndjson_feed)

TESTS(json) { RUN_JSON_TESTS(); }
