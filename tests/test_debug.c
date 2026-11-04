#include "unit_test.h"
#include "../lib/debug.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <unistd.h>

/* run fn() with the debug output going to a pipe and return what it wrote */
static size_t
capture(void (*fn)(void), char* out, size_t max) {
  buffer b, *saved = debug_buf;
  char mem[1024];
  int fd[2];
  ssize_t n;

  if(pipe(fd))
    return 0;
  buffer_init(&b, (buffer_op_ptr)write, fd[1], mem, sizeof mem);
  debug_buf = &b;
  fn();
  buffer_flush(&b);
  debug_buf = saved;
  close(fd[1]);
  n = read(fd[0], out, max - 1);
  close(fd[0]);
  if(n < 0)
    n = 0;
  out[n] = 0;
  return (size_t)n;
}

static void
do_int(void) {
  debug_int("i", 42);
  debug_long("l", -7);
}
static void
do_str(void) {
  debug_str("s", "hello");
}
static void
do_byte(void) {
  debug_byte("b", "ab", 2);
}
static void
do_sa(void) {
  stralloc sa = {0};

  stralloc_copys(&sa, "sa-text");
  debug_sa("sa", &sa);
  stralloc_free(&sa);
}
static void
do_sl(void) {
  strlist sl;

  strlist_init(&sl, '\0');
  strlist_push(&sl, "x");
  strlist_push(&sl, "y");
  debug_sl("sl", &sl, ",");
  stralloc_free(&sl.sa);
}
static void
do_strarray(void) {
  strarray a;

  strarray_init(&a);
  strarray_push(&a, "p");
  strarray_push(&a, "q");
  debug_strarray("ar", &a);
  strarray_free(&a);
}

/*
 * debug_int, debug_long, debug_str, debug_byte, debug_sa, debug_sl, debug_strarray,
 * debug_buf, debug_nl: "name: value" lines on the debug buffer
 */
TEST(test_debug_scalars) {
  char out[256];

  capture(do_int, out, sizeof out);
  ASSERT_EQ(1, str_start(out, "i: 42"));
  ASSERT_EQ(1, str_contains(out, "l: -7"));
  capture(do_str, out, sizeof out);
  ASSERT_STR_EQUAL("s: 'hello'\n", out);
  capture(do_byte, out, sizeof out);
  ASSERT_EQ(1, str_start(out, "b: ab"));
  capture(do_sa, out, sizeof out);
  ASSERT_STR_EQUAL("sa: sa-text\n", out);
}

TEST(test_debug_containers) {
  char out[256];

  capture(do_sl, out, sizeof out);
  ASSERT_EQ(1, str_start(out, "sl:"));
  ASSERT_EQ(1, str_contains(out, "x") && str_contains(out, "y"));
  capture(do_strarray, out, sizeof out);
  ASSERT_EQ(1, str_start(out, "ar:"));
  ASSERT_EQ(1, str_contains(out, "p") && str_contains(out, "q"));
  ASSERT_STR_EQUAL("\n", debug_nl);
  ASSERT_EQ(1, debug_buf != NULL);
}

#define RUN_DEBUG_TESTS() \
  RUN(test_debug_scalars); \
  RUN(test_debug_containers)

TESTS(debug) { RUN_DEBUG_TESTS(); }
