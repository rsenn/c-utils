#include "unit_test.h"
#include "../lib/tokenizer.h"
#include "../lib/buffer.h"
#include "../lib/str.h"

/*
 * void tokenizer_init(tokenizer*, buffer* in, int flags);
 * int tokenizer_next(tokenizer*, token* out);
 */
TEST(test_tokenizer_next) {
  static const char src[] = "x = 42; \"str\"";
  static const enum tokentype expected[] = {TT_IDENTIFIER, TT_SEP, TT_DEC_INT_LIT, TT_SEP, TT_DQSTRING_LIT, TT_EOF};
  buffer in;
  tokenizer t;
  token tok;
  size_t i;

  buffer_frombuf(&in, src, sizeof(src) - 1);
  tokenizer_init(&t, &in, TF_PARSE_STRINGS);
  tokenizer_set_filename(&t, "<test>");

  /* a separator token carries its character, white space is one too */
  for(i = 0; i < 6; i++) {
    do {
      ASSERT_GT(tokenizer_next(&t, &tok), 0);
    } while(tok.type == TT_SEP && tok.value == 32);

    ASSERT_EQ(expected[i], tok.type);

    if(i == 1)
      ASSERT_EQ('=', tok.value);
    if(i == 3)
      ASSERT_EQ(';', tok.value);
  }
}

/* the type of the first token of src, skipping white space */
static enum tokentype
first_type(const char* src, int* ret) {
  buffer in;
  tokenizer t;
  token tok;

  buffer_frombuf(&in, src, str_len(src));
  tokenizer_init(&t, &in, TF_PARSE_STRINGS);

  do {
    *ret = tokenizer_next(&t, &tok);
  } while(tok.type == TT_SEP && tok.value == 32);

  return tok.type;
}

TEST(test_tokenizer_hex_literal) {
  static const char* const hex[] = {
      "0x2a", "0X2A", "0xdeadBEEF", "0x0", "0xFFu", "0x1UL", "0xffll", "0x7;", "0x10 + 1"};
  static const char* const bad[] = {"0x", "0xg", "0x1z", "0xu"};
  size_t i;
  int ret;

  for(i = 0; i < sizeof(hex) / sizeof(hex[0]); i++) {
    ASSERT_EQ(TT_HEX_INT_LIT, first_type(hex[i], &ret));
    ASSERT_EQ(1, ret);
  }

  for(i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) {
    ASSERT_NE(TT_HEX_INT_LIT, first_type(bad[i], &ret));
    ASSERT_EQ(0, ret);
  }

  /* the other number forms keep their type */
  ASSERT_EQ(TT_DEC_INT_LIT, first_type("42", &ret));
  ASSERT_EQ(TT_OCT_INT_LIT, first_type("017", &ret));
  ASSERT_EQ(TT_FLOAT_LIT, first_type("1.5e3", &ret));
}

TEST(test_tokenizer_hex_in_expression) {
  static const char src[] = "x = 0x2a + y";
  static const enum tokentype expected[] = {TT_IDENTIFIER, TT_SEP, TT_HEX_INT_LIT, TT_SEP, TT_IDENTIFIER, TT_EOF};
  buffer in;
  tokenizer t;
  token tok;
  size_t i;

  buffer_frombuf(&in, src, sizeof(src) - 1);
  tokenizer_init(&t, &in, TF_PARSE_STRINGS);

  for(i = 0; i < 6; i++) {
    do {
      ASSERT_GT(tokenizer_next(&t, &tok), 0);
    } while(tok.type == TT_SEP && tok.value == 32);

    ASSERT_EQ(expected[i], tok.type);
  }

  /* the end of the input is a token and stays one */
  ASSERT_GT(tokenizer_next(&t, &tok), 0);
  ASSERT_EQ(TT_EOF, tok.type);
}

/*
 * const char* tokentype_to_str(enum tokentype);
 */
TEST(test_tokentype_to_str) { ASSERT_NE(NULL, tokentype_to_str(TT_IDENTIFIER)); }

/* the types of all tokens of src (white space skipped), as a string of letters */
static void
types_of(const char* src, char* out, size_t max, int flags, int comments) {
  buffer in;
  tokenizer t;
  token tok;
  size_t n = 0;

  buffer_frombuf(&in, src, str_len(src));
  tokenizer_init(&t, &in, flags);
  if(comments) {
    tokenizer_register_marker(&t, MT_SINGLELINE_COMMENT_START, "//");
    tokenizer_register_marker(&t, MT_MULTILINE_COMMENT_START, "/*");
    tokenizer_register_marker(&t, MT_MULTILINE_COMMENT_END, "*/");
  }
  while(n + 1 < max && tokenizer_next(&t, &tok) > 0) {
    if(tok.type == TT_SEP && tok.value == 32)
      continue;
    if(tok.type == TT_EOF)
      break;
    out[n++] = tok.type == TT_IDENTIFIER     ? 'i'
               : tok.type == TT_HEX_INT_LIT  ? 'h'
               : tok.type == TT_DEC_INT_LIT  ? 'd'
               : tok.type == TT_FLOAT_LIT    ? 'f'
               : tok.type == TT_SQSTRING_LIT ? 'q'
               : tok.type == TT_DQSTRING_LIT ? 's'
               : tok.type == TT_SEP          ? (char)tok.value
               : tok.type >= TT_CUSTOM       ? 'C'
                                             : '?';
  }
  out[n] = 0;
}

/*
 * void tokenizer_register_marker(tokenizer*, enum markertype, const char*);
 * comments are skipped once their markers are registered
 */
TEST(test_tokenizer_comments) {
  char out[64];

  types_of("a /* skipped */ b // to the end\n c", out, sizeof out, TF_PARSE_STRINGS, 1);
  ASSERT_STR_EQUAL("iii", out);
  types_of("a /* skipped */ b", out, sizeof out, TF_PARSE_STRINGS, 0);
  ASSERT_EQ(1, str_len(out) > 3); /* without markers '/' and '*' are separators */
}

/*
 * void tokenizer_register_custom_token(tokenizer*, int tokentype, const char*);
 */
TEST(test_tokenizer_custom_token) {
  buffer in;
  tokenizer t;
  token tok;

  buffer_frombuf(&in, "a<<=b", 5);
  tokenizer_init(&t, &in, 0);
  tokenizer_register_custom_token(&t, TT_CUSTOM, "<<=");
  ASSERT_EQ(1, tokenizer_next(&t, &tok) > 0);
  ASSERT_EQ(TT_IDENTIFIER, tok.type);
  ASSERT_EQ(1, tokenizer_next(&t, &tok) > 0);
  ASSERT_EQ(TT_CUSTOM, tok.type);
}

/*
 * int tokenizer_getc / tokenizer_peek / tokenizer_ungetc / tokenizer_skip_chars /
 * tokenizer_read_until / tokenizer_skip_until / tokenizer_peek_token
 */
TEST(test_tokenizer_chars) {
  buffer in;
  tokenizer t;
  token pk;
  int count = 0;

  buffer_frombuf(&in, "abc def; ghi", 12);
  tokenizer_init(&t, &in, 0);
  ASSERT_EQ('a', tokenizer_peek(&t));
  ASSERT_EQ('a', tokenizer_getc(&t));
  ASSERT_EQ('b', tokenizer_getc(&t));
  ASSERT_EQ(1, tokenizer_ungetc(&t, 'b') != TOKENIZER_ERROR);
  ASSERT_EQ('b', tokenizer_getc(&t)); /* pushed back, read again */
  ASSERT_EQ(1, tokenizer_skip_chars(&t, "c ", &count) > 0);
  ASSERT_EQ(2, count);
  ASSERT_EQ('d', tokenizer_peek(&t));
  ASSERT_EQ(1, tokenizer_read_until(&t, ";", 0) > 0);
  ASSERT_STR_EQUAL("def", t.buf);
  tokenizer_skip_until(&t, "h");
  ASSERT_EQ('i', tokenizer_peek(&t));
  ASSERT_EQ(1, tokenizer_peek_token(&t, &pk) > 0);
  ASSERT_EQ(TT_IDENTIFIER, pk.type);
}

TEST(test_tokenizer_flags_filename) {
  buffer in;
  tokenizer t;

  buffer_frombuf(&in, "x", 1);
  tokenizer_init(&t, &in, TF_PARSE_STRINGS);
  ASSERT_EQ(TF_PARSE_STRINGS, tokenizer_get_flags(&t));
  tokenizer_set_flags(&t, TF_PARSE_STRINGS | TF_PARSE_WIDE_STRINGS);
  ASSERT_EQ(3, tokenizer_get_flags(&t));
  tokenizer_set_filename(&t, "file.c");
  ASSERT_STR_EQUAL("file.c", t.filename);
  ASSERT_EQ(0, tokenizer_ftello(&t)); /* nothing read, or an input that cannot tell */
}

TEST(test_tokenizer_literals) {
  char out[64];

  types_of("0x1F 42 3.5 'a' \"str\"", out, sizeof out, TF_PARSE_STRINGS, 0);
  ASSERT_STR_EQUAL("hdfqs", out);
  types_of("foo(bar, 1);", out, sizeof out, TF_PARSE_STRINGS, 0);
  ASSERT_STR_EQUAL("i(i,d);", out);
}

#define RUN_TOKENIZER_TESTS() \
  RUN(test_tokenizer_comments); \
  RUN(test_tokenizer_custom_token); \
  RUN(test_tokenizer_chars); \
  RUN(test_tokenizer_flags_filename); \
  RUN(test_tokenizer_literals); \
  RUN(test_tokenizer_next); \
  RUN(test_tokenizer_hex_literal); \
  RUN(test_tokenizer_hex_in_expression); \
  RUN(test_tokentype_to_str)

TESTS(tokenizer) { RUN_TOKENIZER_TESTS(); }
