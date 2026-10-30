#include "unit_test.h"
#include "../lib/tokenizer.h"
#include "../lib/buffer.h"

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

/*
 * const char* tokentype_to_str(enum tokentype);
 */
TEST(test_tokentype_to_str) {
  ASSERT_NE(NULL, tokentype_to_str(TT_IDENTIFIER));
}

#define RUN_TOKENIZER_TESTS() \
  RUN(test_tokenizer_next); \
  RUN(test_tokentype_to_str)

TESTS(tokenizer) { RUN_TOKENIZER_TESTS(); }
