#ifndef LLMSTREAM_H
#define LLMSTREAM_H 1

/*
 * What the clients of streaming language model APIs have in common: one POST
 * with a chunked body whose answer comes back as server-sent events or as
 * newline delimited JSON, driven by the event loop of the io module.
 *
 * llm_stream_open starts the request, the caller then adds headers and the
 * body with the http_* functions on s->h (llm_stream_chunk does the escaping
 * for a JSON string), llm_stream_run runs the exchange to its end and gives
 * every value of the answer to the on_value callback.
 */

#include "../../lib/http.h"
#include "../../lib/json.h"
#include "../../lib/sse.h"
#include "../../lib/stralloc.h"

typedef struct llm_stream_s {
  http h;
  sse parser;
  ndjson nd;
  /* 0 until the Content-Type is known, then 1 for SSE and 2 for NDJSON */
  int format;
  int force_ndjson;
  int done, failed;
  int status; /* HTTP status of the answer, 0 if there was none */
  stralloc error_body;
  /* one JSON value of the answer, errors the server reports are not passed on */
  void (*on_value)(struct llm_stream_s*, jsonval*, void*);
  /* if set, the data of every event or line as it is, not parsed */
  void (*on_raw)(struct llm_stream_s*, const char*, size_t, void*);
  void* user;
} llm_stream;

/* appends text to out as the inside of a JSON string */
void llm_json_escape(stralloc* out, const char* x, size_t n);

/* queues the escaped text as a part of the body, as a chunk unless use_length */
int llm_stream_chunk(llm_stream* s, const char* x, size_t n, int escape, int use_length);

int llm_stream_open(llm_stream* s, const char* url);
int llm_stream_run(llm_stream* s);
void llm_stream_close(llm_stream* s);

#endif /* defined LLMSTREAM_H */
