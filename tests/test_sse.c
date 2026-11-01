#include "unit_test.h"
#include "../lib/sse.h"
#include "../lib/str.h"
#include "../lib/stralloc.h"

/* what the callback saw, one line per event: type|data|id|retry */
static stralloc seen;

static void
on_event(const sse_event* ev, void* user) {
  (void)user;
  stralloc_cats(&seen, ev->event);
  stralloc_cats(&seen, "|");
  stralloc_catb(&seen, ev->data, ev->data_len);
  stralloc_cats(&seen, "|");
  stralloc_cats(&seen, ev->id);
  stralloc_cats(&seen, "|");
  stralloc_catlong(&seen, ev->retry);
  stralloc_cats(&seen, "\n");
}

static size_t
feed_all(const char* text, size_t step) {
  sse s;
  size_t i, n = str_len(text), events = 0;

  stralloc_zero(&seen);
  sse_init(&s);

  for(i = 0; i < n; i += step)
    events += sse_feed(&s, text + i, n - i < step ? n - i : step, &on_event, 0);

  sse_free(&s);
  stralloc_nul(&seen);
  return events;
}

/*
 * size_t sse_feed(sse*, const char*, size_t, sse_callback*, void*);
 */
TEST(test_sse_feed) {
  static const char stream[] = ": keep alive\n"
                               "data: {\"a\":1}\n\n"
                               "event: ping\r\nid: 7\r\ndata: one\r\ndata: two\r\n\r\n"
                               "retry: 2500\ndata:nospace\n\n"
                               "data: [DONE]\n\n"
                               "data: cut off\n";
  size_t step;

  /* the pieces may be cut anywhere, also between CR and LF */
  for(step = 1; step <= 7; step += 6) {
    ASSERT_EQ(4, feed_all(stream, step));
    ASSERT_STR_EQUAL("message|{\"a\":1}||-1\n"
                     "ping|one\ntwo|7|-1\n"
                     "message|nospace|7|2500\n"
                     "message|[DONE]|7|2500\n",
                     seen.s);
  }
}

/* an event without data is not one, the type does not carry over */
TEST(test_sse_no_data) {
  ASSERT_EQ(1, feed_all("event: x\n\ndata: y\n\n", 3));
  ASSERT_STR_EQUAL("message|y||-1\n", seen.s);

  /* a lone CR ends a line too */
  ASSERT_EQ(1, feed_all("data: z\r\r", 2));
  ASSERT_STR_EQUAL("message|z||-1\n", seen.s);
}

#define RUN_SSE_TESTS() \
  RUN(test_sse_feed); \
  RUN(test_sse_no_data)

TESTS(sse) { RUN_SSE_TESTS(); }
