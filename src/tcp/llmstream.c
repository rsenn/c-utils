#include "llmstream.h"

#include "../../lib/byte.h"
#include "../../lib/errmsg.h"
#include "../../lib/fmt.h"
#include "../../lib/io.h"
#include "../../lib/str.h"

#include <errno.h>

void
llm_json_escape(stralloc* out, const char* x, size_t n) {
  size_t i;

  for(i = 0; i < n; i++) {
    unsigned char c = (unsigned char)x[i];

    switch(c) {
      case '"': stralloc_catb(out, "\\\"", 2); break;
      case '\\': stralloc_catb(out, "\\\\", 2); break;
      case '\n': stralloc_catb(out, "\\n", 2); break;
      case '\r': stralloc_catb(out, "\\r", 2); break;
      case '\t': stralloc_catb(out, "\\t", 2); break;
      default:
        if(c < 0x20) {
          char tmp[2];

          tmp[0] = "0123456789abcdef"[c >> 4];
          tmp[1] = "0123456789abcdef"[c & 15];
          stralloc_catb(out, "\\u00", 4);
          stralloc_catb(out, tmp, 2);
        } else {
          stralloc_catb(out, (const char*)&c, 1);
        }
    }
  }
}

int
llm_stream_chunk(llm_stream* s, const char* x, size_t n, int escape, int use_length) {
  stralloc sa;
  int ret;

  stralloc_init(&sa);

  if(escape)
    llm_json_escape(&sa, x, n);
  else
    stralloc_catb(&sa, x, n);

  ret = use_length ? http_body(&s->h, sa.s, sa.len) : http_chunk(&s->h, sa.s, sa.len);
  stralloc_free(&sa);
  return ret;
}

/* the message of an error object, 0 if the value is not one */
static const char*
error_message(jsonval* v) {
  jsonval err = json_property_path(*v, "error.message");

  if(err.type != JSON_STRING)
    return 0;

  stralloc_nul(&err.stringv);
  return err.stringv.s;
}

static void
value(llm_stream* s, jsonval* v) {
  const char* msg = error_message(v);
  jsonval end;

  if(msg) {
    errmsg_warn("error from the server: ", msg, 0);
    s->failed = 1;
    return;
  }

  if(s->on_value)
    s->on_value(s, v, s->user);

  /* the NDJSON of Ollama and others ends with "done": true */
  end = json_property_path(*v, "done");
  if(end.type == JSON_BOOL && end.boolv)
    s->done = 1;
}

static void
on_event(const sse_event* ev, void* user) {
  llm_stream* s = user;
  jsonval* v;

  /* the end mark of the APIs that follow OpenAI */
  if(ev->data_len == 6 && !byte_diff(ev->data, 6, "[DONE]")) {
    s->done = 1;
    return;
  }

  if(s->on_raw) {
    s->on_raw(s, ev->data, ev->data_len, s->user);
    return;
  }

  if((v = json_parse_text(ev->data, ev->data_len))) {
    value(s, v);
    json_free(v);
  }
}

static void
on_line(jsonval* v, void* user) {
  llm_stream* s = user;

  value(s, v);
}

/* picks SSE or NDJSON from the Content-Type of the answer, once */
static void
pick_format(llm_stream* s) {
  size_t n = 0;
  http_response* r = s->h.response;
  const char* type = http_get_header(r->data.s, r->headers_len, "Content-Type", &n);

  if(s->force_ndjson || (type && byte_finds(type, n, "json") < n))
    s->format = 2;
  else
    s->format = 1;
}

/* what the response has received so far */
static void
drain(llm_stream* s) {
  char buf[4096];
  ssize_t n;
  http_response* r = s->h.response;

  while((n = http_recv(&s->h, buf, sizeof(buf))) > 0) {
    if(r->code >= 400) {
      stralloc_catb(&s->error_body, buf, (size_t)n);
      continue;
    }

    if(!s->format)
      pick_format(s);

    if(s->format == 2) {
      if(s->on_raw) {
        /* the lines are not cut here, a client of raw data gets the pieces */
        s->on_raw(s, buf, (size_t)n, s->user);
      } else {
        ndjson_feed(&s->nd, buf, (size_t)n, on_line, s);
      }
    } else {
      sse_feed(&s->parser, buf, (size_t)n, on_event, s);
    }
  }

  if(n < 0) {
    errmsg_warnsys("reading the answer: ", 0);
    s->failed = 1;
  }

  if(r->status == HTTP_STATUS_FINISH)
    s->done = 1;
}

int
llm_stream_open(llm_stream* s, const char* url) {
  byte_zero(s, sizeof(llm_stream));
  sse_init(&s->parser);
  ndjson_init(&s->nd);

  http_init(&s->h, 0, 0);
  s->h.version = 11;
  s->h.stream = 1;

  return http_post(&s->h, url);
}

int
llm_stream_run(llm_stream* s) {
  http* h = &s->h;

  while(!s->done && !s->failed) {
    fd_type fd;

    io_wait();

    while((fd = io_canwrite()) != -1) {
      if(fd != h->sock)
        continue;

      if(http_canwrite(h, &io_onlywantread, &io_onlywantwrite) == -1 && h->err != EAGAIN && h->err != EWOULDBLOCK) {
        errmsg_warnsys("sending the request: ", 0);
        return 1;
      }
    }

    while((fd = io_canread()) != -1) {
      if(fd != h->sock)
        continue;

      if(http_canread(h, &io_onlywantread, &io_onlywantwrite) == -1 && h->err != EAGAIN && h->err != EWOULDBLOCK) {
        errmsg_warnsys("reading the answer: ", 0);
        return 1;
      }

      drain(s);
    }
  }

  s->status = h->response ? h->response->code : 0;

  if(s->status >= 400) {
    char num[FMT_ULONG];
    jsonval* v = json_parse_text(s->error_body.s, s->error_body.len);
    const char* msg = v ? error_message(v) : 0;

    num[fmt_ulong(num, (unsigned long)s->status)] = '\0';
    stralloc_nul(&s->error_body);
    errmsg_warn("HTTP status ", num, ": ", msg ? msg : s->error_body.s ? s->error_body.s : "", 0);

    if(v)
      json_free(v);
    s->failed = 1;
  }

  return s->failed;
}

void
llm_stream_close(llm_stream* s) {
  http_close(&s->h);
  http_free(&s->h);
  sse_free(&s->parser);
  ndjson_free(&s->nd);
  stralloc_free(&s->error_body);
}
