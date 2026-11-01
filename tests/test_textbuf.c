#include "unit_test.h"
#include "../lib/textbuf.h"
#include "../lib/byte.h"
#include "../lib/buffer.h"

/*
 * void textbuf_init(buffer*, buffer* source, size_t bufsize);
 * uint64 textbuf_line(buffer*); uint64 textbuf_column(buffer*);
 * textbuf* is_textbuf(buffer*);  void textbuf_free(buffer*);
 */
TEST(test_textbuf_position) {
  buffer src, tb;
  char data[] = "ab\ncd\nef";
  char out[16];

  buffer_frombuf(&src, data, 8);
  textbuf_init(&tb, &src, 16);
  ASSERT_EQ(1, is_textbuf(&tb) != NULL);
  ASSERT_EQ(0, textbuf_line(&tb));
  ASSERT_EQ(0, textbuf_column(&tb));

  ASSERT_EQ(8, buffer_get(&tb, out, 8));
  ASSERT_EQ(0, byte_diff(out, 8, "ab\ncd\nef"));
  ASSERT_EQ(2, textbuf_line(&tb));   /* two newlines seen */
  ASSERT_EQ(2, textbuf_column(&tb)); /* "ef" after the last one */
  textbuf_free(&tb);
}

TEST(test_textbuf_foreign_buffer) {
  buffer b;
  char data[] = "x";

  buffer_frombuf(&b, data, 1);
  ASSERT_EQ(1, is_textbuf(&b) == NULL); /* an ordinary buffer is no textbuf */
  ASSERT_EQ(0, textbuf_line(&b));
  ASSERT_EQ(0, textbuf_column(&b));
}

#define RUN_TEXTBUF_TESTS() \
  RUN(test_textbuf_position); \
  RUN(test_textbuf_foreign_buffer)

TESTS(textbuf) { RUN_TEXTBUF_TESTS(); }
