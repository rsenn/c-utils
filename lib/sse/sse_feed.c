#include "../sse.h"
#include "../byte.h"
#include "../str.h"

/* handles one complete line, returns 1 if it dispatched an event */
static int
sse_line(sse* s, sse_callback* cb, void* user) {
  const char *x = s->line.s, *value;
  size_t n = s->line.len, name;

  if(n == 0) {
    sse_event ev;

    /* an empty line ends the event, one without data is dropped */
    if(!s->has_data) {
      s->event.len = 0;
      return 0;
    }

    /* every data line added a newline, the last one is not part of the data */
    if(s->data.len)
      s->data.len--;

    stralloc_nul(&s->data);
    stralloc_nul(&s->event);
    stralloc_nul(&s->id);

    ev.event = s->event.len ? s->event.s : "message";
    ev.data = s->data.s;
    ev.data_len = s->data.len;
    ev.id = s->id.s ? s->id.s : "";
    ev.retry = s->retry;

    if(cb)
      cb(&ev, user);

    s->data.len = 0;
    s->event.len = 0;
    s->has_data = 0;
    return 1;
  }

  /* a comment, often used to keep the connection alive */
  if(x[0] == ':')
    return 0;

  name = byte_chr(x, n, ':');
  value = x + name;
  n -= name;

  if(n) {
    value++;
    n--;
    /* one space after the colon is not part of the value */
    if(n && *value == ' ') {
      value++;
      n--;
    }
  }

  if(name == 4 && !byte_diff(x, 4, "data")) {
    stralloc_catb(&s->data, value, n);
    stralloc_catb(&s->data, "\n", 1);
    s->has_data = 1;
  } else if(name == 5 && !byte_diff(x, 5, "event")) {
    stralloc_copyb(&s->event, value, n);
  } else if(name == 2 && !byte_diff(x, 2, "id")) {
    /* an id with a NUL is ignored */
    if(byte_chr(value, n, '\0') == n)
      stralloc_copyb(&s->id, value, n);
  } else if(name == 5 && !byte_diff(x, 5, "retry")) {
    unsigned long v = 0;
    size_t i = 0;

    /* only digits count, the value is not terminated so it is read by hand */
    while(i < n && value[i] >= '0' && value[i] <= '9' && v < 100000000UL)
      v = v * 10 + (unsigned long)(value[i++] - '0');

    if(i == n && n)
      s->retry = (long)v;
  }

  return 0;
}

size_t
sse_feed(sse* s, const char* x, size_t n, sse_callback* cb, void* user) {
  size_t i, events = 0;

  for(i = 0; i < n; i++) {
    char c = x[i];

    /* the LF of a CR LF pair has been taken care of with the CR */
    if(s->cr) {
      s->cr = 0;
      if(c == '\n')
        continue;
    }

    if(c == '\r' || c == '\n') {
      s->cr = (c == '\r');
      events += sse_line(s, cb, user);
      s->line.len = 0;
      continue;
    }

    /* a byte order mark at the start of the stream is not data */
    if(!s->started) {
      s->started = 1;
      if((unsigned char)c == 0xef && n - i >= 3 && (unsigned char)x[i + 1] == 0xbb && (unsigned char)x[i + 2] == 0xbf) {
        i += 2;
        continue;
      }
    }

    stralloc_catb(&s->line, &c, 1);
  }

  return events;
}
