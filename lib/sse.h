/**
 * @defgroup   sse
 * @brief      Server-sent events (text/event-stream) parser.
 * @{
 */
#ifndef SSE_H
#define SSE_H 1

#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* one event as it is handed to the callback, valid only during the call */
typedef struct {
  const char* event; /* the event type, "message" if the stream gave none */
  const char* data;  /* the data lines joined by newlines, 0 terminated */
  size_t data_len;
  const char* id;    /* the last event id, empty if there was none */
  long retry;        /* reconnection time in milliseconds, -1 if not given */
} sse_event;

typedef void sse_callback(const sse_event* ev, void* user);

/* the parser keeps what the stream has not completed yet, feed it any pieces */
typedef struct {
  stralloc line, data, event, id;
  long retry;
  unsigned cr : 1;      /* the last byte was a CR, a LF that follows belongs to it */
  unsigned has_data : 1;
  unsigned started : 1; /* some byte was seen, a byte order mark is only ignored at the start */
} sse;

void sse_init(sse* s);
void sse_free(sse* s);

/* Parses n bytes of the stream, calls cb for every event they complete and
 * returns how many that were. An event that is cut off at the end of the
 * stream is never dispatched. */
size_t sse_feed(sse* s, const char* x, size_t n, sse_callback* cb, void* user);

#ifdef __cplusplus
}
#endif

#endif /* defined SSE_H */
/** @} */
