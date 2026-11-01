# http

`lib/http.h` + `lib/http/*.c` -- an HTTP/1.0 and HTTP/1.1 client for plain
TCP and TLS, built for the event loop of `lib/io.h`. One function per file
under `lib/http/`, following the module convention described in `CLAUDE.md`.
Every function has a manual page in `lib/http/<name>.3`; this document is
the guide that ties them together (`http(3)` is the short form of it).

Together with `lib/sse.h` (server-sent events) and the NDJSON reader of
`lib/json.h` it can drive streaming APIs: the chat APIs of language models
(OpenAI, Gemini, Ollama, ...) are the reason the streaming parts exist, see
[Streaming APIs](#streaming-apis).

Covers: GET and POST (also PUT, DELETE, PATCH, HEAD) with headers, a body of
known length or a chunked body that is produced while it is sent, response
bodies with `Content-Length`, chunked or until close, https with SNI.
Does not cover: redirects, content decoding (gzip), cookies, proxies,
HTTP/2, IPv6, keep-alive reuse of a connection (see
[Limitations](#limitations)).

## The lifecycle of a request

```c
http h;

http_init(&h, 0, 0);                  /* 1. prepare                         */
http_post(&h, "https://host/path");   /* 2. resolve, socket, start connect  */
http_header(&h, "Content-Type", "application/json");     /* 3. describe   */
http_body(&h, json, json_len);
...                                   /* 4. event loop: canwrite/canread    */
http_close(&h);                       /* 5. close                           */
http_free(&h);                        /* 6. release the memory              */
```

Nothing is written when `http_open` returns. The connection is still being
established; the request goes out from `http_canwrite`, which is why headers
and body can still be added after `http_post`. For https the TLS handshake
happens inside `http_canwrite`/`http_canread` as well.

### The event loop

The module does not block (unless `h.nonblocking` is cleared before the
request is started). The caller waits for the socket and then calls the
handler for the direction that is ready:

```c
for(;;) {
  fd_type fd;

  io_wait();

  while((fd = io_canwrite()) != -1)
    if(fd == h.sock && http_canwrite(&h, io_onlywantread, io_onlywantwrite) == -1 &&
       h.err != EAGAIN && h.err != EWOULDBLOCK)
      fail("send");

  while((fd = io_canread()) != -1)
    if(fd == h.sock) {
      if(http_canread(&h, io_onlywantread, io_onlywantwrite) == -1 &&
         h.err != EAGAIN && h.err != EWOULDBLOCK)
        fail("receive");
      /* take the body out here, see "Reading the response" */
    }
}
```

The two function arguments are the functions of the event loop that change
what the socket is watched for. A TLS handshake or write that has to wait
for the opposite direction uses them; with plain TCP they are never called.
`h.err` holds `errno` (or the TLS error) of the last operation; `EAGAIN` and
`EWOULDBLOCK` only say that nothing is ready yet.

After the request is out the socket is watched for reading only. Writing is
switched on again by `http_chunk` / `http_chunk_end` when new body data was
queued, and by `http_body_flush` when the connection did not take it all.

## Starting a request

| function | purpose |
| --- | --- |
| `http_init(h, host, port)` | clear the structure, set defaults |
| `http_open(h, type, location)` | start a request with any method |
| `http_get(h, location)`, `http_post(h, location)` | `http_open` with GET / POST |
| `http_method(type)` | `"GET"`, `"POST"`, ... |

`location` is an URL (`https://api.example.com:8443/v1/models?x=1`) or a path
that starts with a slash, which uses the host of the structure. The scheme
`https` selects TLS and port 443, a port after the host name overrides the
default. An URL without a path asks for `/`. Everything from the first CR,
LF or NUL of the location on is dropped, so a location cannot inject
headers. The return value is 1 if the connection is on its way, 0 if the host
could not be resolved or the socket could not be created.

Flags of the structure that are meant to be changed after `http_init` and
before `http_open`:

| field | default | meaning |
| --- | --- | --- |
| `version` | 0 | `11` writes `HTTP/1.1`; otherwise a GET is `HTTP/1.0`. POSTs and chunked bodies are always 1.1 |
| `nonblocking` | 1 | non-blocking socket; 0 for a blocking one |
| `keepalive` | 0 | 0 sends `Connection: close` |
| `stream` | 0 | leave the response body for `http_recv` (see below) |

The host is resolved with the blocking `dns_ip4`, and only when `h.addr` is
still `0.0.0.0`: a structure that was used once keeps the address, so use a
fresh `http_init` for every host.

## Headers and body

```c
int http_header(http* h, const char* name, const char* value);
int http_body(http* h, const char* x, size_t n);
int http_chunked(http* h);
int http_chunk(http* h, const char* x, size_t n);
int http_chunk_end(http* h);
ssize_t http_body_flush(http* h);
```

All of them work on the request that was started last. They return 1 on
success and 0 on failure (`http_body_flush` has its own result, see below).

The module writes the request line, `Host` (with the port when it is not the
default one), `Connection`, the framing of the body, and `Accept: */*` unless
you added an `Accept` header. Anything else is yours: `Content-Type`,
`Authorization`, `User-Agent`, API keys. `http_header` refuses a name that is
empty or contains `:`, and a name or value with a CR or LF in it, because
those would split or end the header block.

### A body of known length

`http_body` appends to the body; call it as often as you like before the
request goes out. It is sent with `Content-Length`. A POST, PUT or PATCH
without any body is sent with `Content-Length: 0`.

### A chunked body

`http_chunked` declares that the body uses `Transfer-Encoding: chunked`.
`http_chunk` encodes its argument as one chunk and queues it; `http_chunk_end`
queues the last chunk. They can be called

* before the request is sent: the chunks follow the headers directly;
* after it: the data is written by the next `http_canwrite`, as far as the
  connection takes it, and what does not fit stays queued (the queue grows
  with the speed of the producer, it is not bounded).

This is what lets a client send a prompt while it is still being read from
a pipe, or start the request before the body is complete. A chunk of length
0 is ignored (it would end the body). After `http_chunk_end`, `http_chunk`
fails. `http_chunked` has to come before the headers are sent and excludes
`http_body`.

Not every server accepts a chunked request. `copenai -l` shows how to fall
back to `http_body` with the same code.

`http_body_flush` writes the queue; `http_sendreq` and `http_canwrite` call
it, so you rarely need to. It returns 1 when everything is written, 0 when the
socket is full (it is then watched for writing) or the headers are not out
yet, -1 on an error. The body goes to the socket directly and not through
`h.q.out`, because `buffer_flush` forgets what a non-blocking socket refused.

## Reading the response

`http_canread` reads the socket and parses the head of the response into
`h.response`:

```c
typedef struct http_response_s {
  http_transfer_type transfer;  /* HTTP_TRANSFER_UNDEF/CHUNKED/LENGTH/BOUNDARY */
  http_status status;           /* HTTP_RECV_HEADER, HTTP_RECV_DATA, HTTP_STATUS_CLOSED,
                                   HTTP_STATUS_ERROR, HTTP_STATUS_BUSY, HTTP_STATUS_FINISH */
  int code;                     /* status code, -1 until the status line is read */
  uint64 headers_len;           /* length of the head in data */
  uint64 content_length;        /* from Content-Length, otherwise (uint64)-1 */
  stralloc data;                /* status line and headers, CRLF kept */
  ...
  int (*header)(struct http_s*, const char*, size_t);  /* optional, per header line */
} http_response;
```

`status` is `HTTP_RECV_HEADER` until the empty line after the headers, then
`HTTP_RECV_DATA`. `code` is the HTTP status code. Header values are looked up
with `http_get_header(response->data.s, response->headers_len, "Content-Type", &len)`,
which returns a pointer into the data (not NUL terminated) and the length.
Set `response->header` before the headers arrive to be called for each header
line instead.

### The body: `http_recv`

```c
ssize_t http_recv(http* h, char* buf, size_t len);
```

Set `h.stream = 1` before the request is sent. `http_canread` then leaves the
body in the input buffer, and `http_recv` hands it out decoded:

* chunked: sizes, extensions, line ends and trailers removed;
* `Content-Length`: exactly that many bytes, the rest stays in the buffer;
* neither: everything until the peer closes.

`http_recv` never touches the socket, so it is safe to call in a loop after
each `http_canread`:

```c
while((n = http_recv(&h, buf, sizeof(buf))) > 0)
  consume(buf, n);

if(n < 0)
  /* errno EPIPE: cut off in the middle of the body, EPROTO: bad chunk size */;
else if(h.response->status == HTTP_STATUS_FINISH)
  /* the body is complete */;
```

A result of 0 with the status not yet `HTTP_STATUS_FINISH` means "nothing
for now, wait for the socket". When the connection closes and the body had
no length, `http_recv` turns the close into `HTTP_STATUS_FINISH`; when it
closes in the middle of a chunked or sized body the result is -1 with `EPIPE`.

### The old readers

`http_read`, `http_read_internal`, `http_read_header` and `http_response_read`
are the first reader of the module, shaped as `buffer` operations
(`mediathek-list` reads through them with `buffer_lzma`). They require
`h.stream` to be 0 and have a known problem: after the headers `http_canread`
copies the data of the input buffer into `response->data`, one byte of the body
included. New code should use `h.stream` and `http_recv`.

### Errors

A status code of 400 or more is not an error of the module: the headers and
the body (usually a JSON error message) are read as for any response, it is
up to the caller to look at `response->code`. Transport errors are `-1` from
`http_canwrite` / `http_canread` / `http_recv`; `http_strerror(h, -1)` gives
the text (TLS error or `strerror(errno)`).

## Closing

`http_close(h)` shuts down TLS, closes the socket and removes it from the
event loop; requests, responses and buffers stay so that the response can be
inspected afterwards. `http_free(h)` releases them (about 48 kB of buffers
per connection) and the host name. Call both after every request; then
`http_init` can reuse the structure.

## TLS

https URLs create a TLS client (`lib/tls.h`) with the host name announced via
SNI (`tls_servername`), which hosts with many names, like the API providers,
require. Writes use `SSL_MODE_ENABLE_PARTIAL_WRITE` and
`SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER`, so a write that is retried after
`WANT_WRITE` may come from a queue that grew or moved.

**The certificate of the server is not verified** (`tls_context` uses
`SSL_VERIFY_NONE`, see `BUGS`). Do not send secrets over a network you do not
trust.

## Streaming APIs

The pieces of a streamed answer:

| piece | where | what it does |
| --- | --- | --- |
| chunked POST | `http_chunked`, `http_chunk`, `http_chunk_end` | send the request while it is produced |
| `http_recv` | `lib/http.h` | decode the response body as it arrives |
| `sse_feed` | `lib/sse.h` | cut `text/event-stream` into events |
| `ndjson_feed` | `lib/json.h` | cut newline delimited JSON into values |
| `json_parse_text`, `json_property_path` | `lib/json.h` | strict parse, then `choices[0].delta.content` |

Which of the two formats an answer uses is told by its `Content-Type`:
`text/event-stream` is SSE, `application/x-ndjson` (or other `json` types) is
NDJSON. The tools share `src/tcp/llmstream.c`, which wraps this: it starts
the request, runs the event loop, picks the format from the header, parses each
event or line, reports `{"error":{"message":...}}` objects and non-2xx
bodies, and ends at `[DONE]`, `"done": true` or the end of the body.

The same, written by hand:

```c
static void
on_event(const sse_event* ev, void* user) {
  jsonval* v;

  if(ev->data_len == 6 && !byte_diff(ev->data, 6, "[DONE]"))
    return;

  if((v = json_parse_text(ev->data, ev->data_len))) {
    jsonval t = json_property_path(*v, "choices[0].delta.content");

    if(t.type == JSON_STRING)
      buffer_put(buffer_1, t.stringv.s, t.stringv.len);
    json_free(v);
  }
}

...
h.version = 11;
h.stream = 1;
http_post(&h, "https://api.openai.com/v1/chat/completions");
http_header(&h, "Authorization", "Bearer sk-...");
http_header(&h, "Content-Type", "application/json");
http_header(&h, "Accept", "text/event-stream");
http_chunked(&h);
http_chunk(&h, "{\"model\":\"gpt-4o-mini\",\"stream\":true,\"messages\":[", 47);
...
http_chunk_end(&h);
/* event loop; after each http_canread: */
while((n = http_recv(&h, buf, sizeof(buf))) > 0)
  sse_feed(&parser, buf, n, on_event, 0);
```

### SSE

`sse_feed(&s, x, n, cb, user)` takes the stream in any pieces (also a CR and
LF in different pieces) and calls `cb` for every complete event with
`event` (default `"message"`), `data` (several `data:` lines joined by `\n`),
`id`, `retry`. Comments (`:`), unknown fields and events without data are
ignored; an event cut off by the end of the stream is dropped.
`sse_init` / `sse_free` bracket the parser.

### NDJSON

`ndjson_feed(&nd, x, n, cb, user)` collects lines and parses every complete
one with `json_parse_text`; the callback gets the tree, which is released
when it returns. Empty lines are skipped, invalid lines are skipped and
counted in `nd.errors`.

### JSON for model output

The strings in the JSON tree are the text itself (UTF-8): `\uXXXX` escapes
and surrogate pairs are decoded on reading, `json_print` escapes them again.
`json_valid` / `json_parse_text` accept exactly RFC 8259 (the older
`json_read_tree` is lenient and does not tell when it gave up).
`json_property_path(v, "candidates[0].content.parts[0].text")` follows
objects and arrays and gives a value of type `JSON_UNDEFINED` for a missing
step, never a crash. A key with dots is written `["a.b"]`.

## Tools built on it

| tool | what it does |
| --- | --- |
| `src/tcp/chttpcat.c` | HTTP/HTTPS client like curl: URLs as arguments, stdin as chunked POST body, `-i -H -X -L -o`. Has its own blocking transport |
| `src/tcp/copenai.c` | asks an OpenAI-compatible chat API, prompt from the arguments or stdin, answer streamed to stdout. `-m -u -k -s -l -n -r`; key from `$OPENAI_API_KEY`. The request is a chunked POST (`-l`: with Content-Length) |
| `src/tcp/cgemini.c` | a conversation with Google Gemini, key from `$GEMINI_API_KEY` |
| `src/tcp/llmstream.c` | the shared event loop, format detection and error handling of the last two |

### cgemini

```sh
GEMINI_API_KEY=... cgemini                 # terminal: prompt, answer follows the line
echo "why is the sky blue?" | GEMINI_API_KEY=... cgemini
GEMINI_API_KEY=... cgemini -l 7000         # TCP socket on 127.0.0.1:7000
```

It posts to `<url>/models/<model>:streamGenerateContent?alt=sse` with the
key in the `x-goog-api-key` header (not in the URL), keeps the conversation
as the `contents` of the request, and prints the `text` of every part of the
first candidate as it arrives. `-m` sets the model, `-u` the API base,
`-s` a system instruction.

Input: one line is one message; a line that ends in a backslash continues in
the next one. `/reset` forgets the conversation, `/quit` ends the session.

Output protocol when stdin is not a terminal, or on the socket: no prompts;
the answer to every message is followed by a line that holds only `0x04`
(EOT), or `0x18` (CAN) if the request failed (the reason goes to standard
error of `cgemini`). A client reads until it sees that line, then sends the
next message. With `-l` every connection is a conversation of its own in a
forked process; the socket is bound to `127.0.0.1` unless `-l ADDR:PORT`
says otherwise, and everyone who can connect spends your quota.

## Limitations

* The certificate of a TLS server is not verified.
* The host name is resolved with a blocking call, IPv4 only.
* One request per connection: `Connection: close` is sent unless
  `keepalive` is set, and nothing reuses the connection afterwards
  (`http_recv` knows where the response ends, so it could; see `TODO`).
* No redirects, no `Content-Encoding` decoding, no cookies, no proxies.
* A multipart response (`HTTP_TRANSFER_BOUNDARY`) is recognized, not decoded.
* The queue of a chunked body is not bounded.
* Gemini without `alt=sse` answers with a streamed JSON array, which is
  neither SSE nor NDJSON and is not read (see `TODO`).
* `buffer_flush` loses data after `EAGAIN`; the headers go out through it
  and are assumed to fit into the socket buffer (see `BUGS`).

## Tests

`tests/test_http.c` covers the chunked reader and writer, the request
queue (headers, body, chunks, the refusal of line ends) and `http_recv` for
all framings and a cut-off body; `tests/test_sse.c` and `tests/test_json.c`
cover the stream parsers. The exchange with a server was tried against local
mock servers (a chunked request in, an SSE or NDJSON answer in pieces out).
