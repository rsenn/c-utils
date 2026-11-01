#include "unit_test.h"
#include "../lib/json.h"
#include "../lib/byte.h"
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


static jsonval*
parse(const char* s) {
  return json_parse_text(s, str_len(s));
}

/*
 * json_length, json_toint, json_todouble, json_isnull, json_property_get_str, json_array_at, json_array_items
 */
TEST(test_json_values) {
  static const char t[] = "{\"n\":42,\"d\":2.5,\"s\":\"hi\",\"b\":true,\"z\":null,\"a\":[1,2,3],\"o\":{\"k\":\"v\"}}";
  jsonval* v = parse(t);
  jsonval n, d, s, b, z, u, a;
  jsonitem* it;
  size_t count = 0;

  ASSERT_EQ(1, v != NULL);
  ASSERT_EQ(JSON_OBJECT, v->type);
  ASSERT_EQ(7, json_length(*v));

  n = json_property_get_str(*v, "n");
  ASSERT_EQ(JSON_INT, n.type);
  ASSERT_EQ(42, json_toint(n));
  d = json_property_get_str(*v, "d");
  ASSERT_EQ(JSON_DOUBLE, d.type);
  ASSERT_EQ(1, json_todouble(d) == 2.5);
  ASSERT_EQ(2, json_toint(d)); /* truncated */
  s = json_property_get_str(*v, "s");
  ASSERT_EQ(JSON_STRING, s.type);
  ASSERT_EQ(2, json_length(s));
  ASSERT_STR_EQUAL("hi", json_string_cstr(&s));
  b = json_property_get_str(*v, "b");
  ASSERT_EQ(JSON_BOOL, b.type);
  ASSERT_EQ(1, b.boolv);
  z = json_property_get_str(*v, "z");
  ASSERT_EQ(1, json_isnull(z));
  u = json_property_get_str(*v, "nope");
  ASSERT_EQ(JSON_UNDEFINED, u.type);
  ASSERT_EQ(0, json_isnull(u));

  a = json_property_get_str(*v, "a");
  ASSERT_EQ(JSON_ARRAY, a.type);
  ASSERT_EQ(3, json_length(a));
  it = json_array_at(a, 1);
  ASSERT_EQ(2, json_toint(it->value));
  ASSERT_EQ(1, json_array_at(a, 9) == NULL);
  for(it = json_array_items(a); it; it = it->next)
    ++count;
  ASSERT_EQ(3, count);
  ASSERT_EQ(1, json_toint(json_property_get_int(a, 0)));
  json_free(v);
}

TEST(test_json_build) {
  jsonval* arr = json_newnode(JSON_ARRAY);
  jsonval iv, o, key;
  stralloc sa;

  stralloc_init(&sa);
  iv.type = JSON_INT;
  iv.intv = 7;
  json_array_push(arr, iv);
  json_array_push(arr, iv);
  ASSERT_EQ(2, json_length(*arr));
  json_tosa(*arr, &sa, json_compact_printer);
  ASSERT_EQ(1, sa.len == 5 && byte_equal(sa.s, 5, "[7,7]"));

  o = json_object();
  ASSERT_EQ(JSON_OBJECT, o.type);
  ASSERT_EQ(0, json_length(o));
  byte_zero(&key, sizeof key);
  key.type = JSON_STRING;
  stralloc_copys(&key.stringv, "kk");
  json_property_set(&o, key, iv);
  json_property_set_str(&o, "two", iv);
  ASSERT_EQ(2, json_length(o));
  ASSERT_EQ(7, json_toint(json_property_get_str(o, "two")));
  stralloc_zero(&sa);
  json_tosa(o, &sa, json_compact_printer);
  ASSERT_EQ(1, sa.len == 16 && byte_equal(sa.s, 16, "{\"kk\":7,\"two\":7}"));
  stralloc_free(&sa);
  json_free(arr);
}

/*
 * numbers: a number that ends the input is not dropped (it was) and signs/exponents work
 */
TEST(test_json_numbers) {
  jsonval* v;

  v = parse("42");
  ASSERT_EQ(JSON_INT, v->type);
  ASSERT_EQ(42, v->intv);
  v = parse("-0.5e2");
  ASSERT_EQ(JSON_DOUBLE, v->type);
  ASSERT_EQ(1, v->doublev == -50.0);
  v = parse("1E3");
  ASSERT_EQ(1, v->doublev == 1000.0);
  v = parse("[1e-2, 2.5E+1]");
  ASSERT_EQ(2, json_length(*v));
  ASSERT_EQ(1, json_array_at(*v, 0)->value.doublev == 0.01);
  ASSERT_EQ(1, json_array_at(*v, 1)->value.doublev == 25.0);
}

TEST(test_json_literals_and_errors) {
  static const char* const bad[] = {"{", "[1,]", "{\"a\":}", "nul", "\"unterminated", "01", "1e", "{'a':1}"};
  size_t i;
  jsonval* v;

  ASSERT_EQ(JSON_BOOL, parse("true")->type);
  ASSERT_EQ(JSON_NULL, parse("null")->type);
  ASSERT_EQ(JSON_ARRAY, parse("  [ 1 , 2 ]  ")->type);
  v = parse("\"\\u00e9\\n\"");
  ASSERT_EQ(JSON_STRING, v->type);
  ASSERT_EQ(3, json_length(*v)); /* e acute is two bytes in UTF-8, then the newline */
  for(i = 0; i < sizeof bad / sizeof bad[0]; ++i)
    ASSERT_EQ(1, parse(bad[i]) == NULL);
}

TEST(test_json_print_buffer) {
  jsonval* v = parse("{\"a\":[1,true]}");
  buffer b;
  char mem[128], got[64];
  int fd[2];
  ssize_t n;

  ASSERT_EQ(0, pipe(fd));
  buffer_init(&b, (buffer_op_ptr)write, fd[1], mem, sizeof mem);
  json_print(*v, &b, json_compact_printer); /* flushes at the end */
  close(fd[1]);
  n = read(fd[0], got, sizeof got);
  close(fd[0]);
  ASSERT_EQ(14, n);
  ASSERT_EQ(0, byte_diff(got, 14, "{\"a\":[1,true]}"));
  json_free(v);
}

/*
 * json_parse, json_parse_null, _boolean, _number, _string, _array, _object, _stralloc, json_reader_init
 */
TEST(test_json_parse_direct) {
  charbuf cb;
  jsonval v;
  jsonreader r;
  stralloc sa;

  stralloc_init(&sa);

  charbuf_froms(&cb, "null", 8);
  ASSERT_EQ(1, json_parse_null(&v, &cb));
  ASSERT_EQ(1, json_isnull(v));
  charbuf_free(&cb);

  charbuf_froms(&cb, "true", 8);
  ASSERT_EQ(1, json_parse_boolean(&v, &cb));
  ASSERT_EQ(JSON_BOOL, v.type);
  ASSERT_EQ(1, v.boolv);
  charbuf_free(&cb);
  charbuf_froms(&cb, "false", 8);
  ASSERT_EQ(1, json_parse_boolean(&v, &cb));
  ASSERT_EQ(0, v.boolv);
  charbuf_free(&cb);

  charbuf_froms(&cb, "-12.5e1", 8);
  ASSERT_EQ(1, json_parse_number(&v, &cb));
  ASSERT_EQ(1, json_todouble(v) == -125.0);
  charbuf_free(&cb);

  charbuf_froms(&cb, "\"a\\tb\"", 8);
  ASSERT_EQ(1, json_parse_stralloc(&cb, &sa, 0) >= 0); /* unquoted: up to a colon or the end */
  charbuf_free(&cb);

  charbuf_froms(&cb, "\"hi\\n\"", 8);
  ASSERT_EQ(1, json_parse(&v, &cb));
  ASSERT_EQ(JSON_STRING, v.type);
  ASSERT_EQ(3, json_length(v));
  charbuf_free(&cb);

  charbuf_froms(&cb, "[1,2]", 8);
  ASSERT_EQ(1, json_parse_array(&v, &cb));
  ASSERT_EQ(JSON_ARRAY, v.type);
  ASSERT_EQ(2, json_length(v));
  charbuf_free(&cb);

  charbuf_froms(&cb, "{\"k\":1}", 8);
  ASSERT_EQ(1, json_parse_object(&v, &cb));
  ASSERT_EQ(JSON_OBJECT, v.type);
  ASSERT_EQ(1, json_length(v));
  charbuf_free(&cb);

  charbuf_froms(&cb, "  [", 8);
  json_reader_init(&r, &cb);
  ASSERT_EQ(JSON_UNDEFINED, r.doc->type);
  ASSERT_EQ(&cb, r.b);
  ASSERT_EQ(&r.doc, r.loc);
  charbuf_free(&cb);

  stralloc_free(&sa);
}

/*
 * json_append, json_array_push, json_recurse, json_property_set_int, json_property_get
 */
static void
count_nodes(jsonval* v, void* arg, int depth) {
  (void)v;
  (void)depth;
  ++*(int*)arg;
}

TEST(test_json_append_recurse_set) {
  jsonval* arrp = json_newnode(JSON_ARRAY);
  jsonitem* list;
  jsonval* obj;
  jsonval got;
  int nodes = 0;

  ASSERT_EQ(1, json_append(&arrp->itemv, json_int(1)) != NULL);
  ASSERT_EQ(1, json_append(&arrp->itemv, json_int(2)) != NULL);
  list = arrp->itemv;
  ASSERT_EQ(1, json_toint(list->value));
  ASSERT_EQ(2, json_toint(list->next->value));
  ASSERT_EQ(NULL, list->next->next);

  ASSERT_EQ(2, json_length(*arrp));
  json_array_push(arrp, json_string("x"));
  ASSERT_EQ(3, json_length(*arrp));
  json_recurse(arrp, (void (*)())count_nodes, &nodes);
  ASSERT_EQ(4, nodes); /* the array and its three members */
  json_free(arrp);

  obj = json_newnode(JSON_OBJECT);
  json_property_set_int(obj, 7, json_string("seven"));
  got = json_property_get_int(*obj, 7);
  ASSERT_EQ(JSON_STRING, got.type);
  ASSERT_STR_EQUAL("seven", json_string_cstr(&got));
  json_free(obj);
}

#define RUN_JSON_TESTS() \
  RUN(test_json_parse_direct); \
  RUN(test_json_append_recurse_set); \
  RUN(test_json_values); \
  RUN(test_json_build); \
  RUN(test_json_numbers); \
  RUN(test_json_literals_and_errors); \
  RUN(test_json_print_buffer); \
  RUN(test_json_read_print); \
  RUN(test_json_string_escapes); \
  RUN(test_json_tostring); \
  RUN(test_json_string_decode); \
  RUN(test_json_string_print); \
  RUN(test_json_valid); \
  RUN(test_json_property_path); \
  RUN(test_ndjson_feed)

TESTS(json) { RUN_JSON_TESTS(); }
