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
  static const char* const hex[] = {"0x2a", "0X2A", "0xdeadBEEF", "0x0", "0xFFu", "0x1UL", "0xffll", "0x7;", "0x10 + 1"};
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
TEST(test_tokentype_to_str) {
  ASSERT_NE(NULL, tokentype_to_str(TT_IDENTIFIER));
}

#define RUN_TOKENIZER_TESTS() \
  RUN(test_tokenizer_next); \
  RUN(test_tokenizer_hex_literal); \
  RUN(test_tokenizer_hex_in_expression); \
  RUN(test_tokentype_to_str)

TESTS(tokenizer) { RUN_TOKENIZER_TESTS(); }
