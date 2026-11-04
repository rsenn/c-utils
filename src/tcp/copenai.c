#include "llmstream.h"

#include "../../lib/buffer.h"
#include "../../lib/env.h"
#include "../../lib/errmsg.h"
#include "../../lib/str.h"
#include "../../lib/stralloc.h"
#include "../../lib/unix.h"

#include <unistd.h>

static const char *model, *base, *key, *system_prompt;
static int use_length;
static llm_stream stream;

static void
usage(const char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [OPTIONS] [PROMPT...]\n"
                       "\n"
                       "Asks a chat model of an OpenAI compatible API and writes the answer as it\n"
                       "arrives. The prompt is the arguments, or standard input if there are none.\n"
                       "The request is POSTed with the chunked transfer encoding, the answer is\n"
                       "read as server-sent events or as newline delimited JSON, as the\n"
                       "Content-Type of the answer says.\n"
                       "\n"
                       "  -m, --model NAME     model, default $OPENAI_MODEL or gpt-4o-mini\n"
                       "  -u, --url URL        base url, default $OPENAI_API_URL or\n"
                       "                       https://api.openai.com/v1\n"
                       "  -k, --key KEY        API key, default $OPENAI_API_KEY\n"
                       "  -s, --system TEXT    system prompt\n"
                       "  -l, --length         send the request with a Content-Length instead of chunks\n"
                       "  -n, --ndjson         read the answer as newline delimited JSON whatever it says\n"
                       "  -r, --raw            write the data of every event instead of the text\n"
                       "  -h, --help           show this help\n"
                       "\n"
                       "The exit status is 1 if the request failed or the answer was an error.\n",
                       NULL);
  buffer_flush(buffer_1);
}

/* the text of one piece of the answer */
static void
on_value(llm_stream* s, jsonval* v, void* user) {
  jsonval text = json_property_path(*v, "choices[0].delta.content");

  (void)s;
  (void)user;

  /* servers with a dialect of their own put it elsewhere */
  if(text.type != JSON_STRING)
    text = json_property_path(*v, "message.content");

  if(text.type == JSON_STRING) {
    buffer_put(buffer_1, text.stringv.s, text.stringv.len);
    buffer_flush(buffer_1);
  }
}

static void
on_raw(llm_stream* s, const char* x, size_t n, void* user) {
  (void)s;
  (void)user;
  buffer_put(buffer_1, x, n);
  buffer_putnlflush(buffer_1);
}

static void
part(const char* x, size_t n, int escape) {
  llm_stream_chunk(&stream, x, n, escape, use_length);
}

static void
build_request(int argc, char* argv[]) {
  stralloc auth;
  char tmp[4096];
  ssize_t r;
  http* h = &stream.h;

  stralloc_init(&auth);
  stralloc_copys(&auth, "Bearer ");
  stralloc_cats(&auth, key);
  stralloc_nul(&auth);

  http_header(h, "Authorization", auth.s);
  http_header(h, "Content-Type", "application/json");
  http_header(h, "Accept", "text/event-stream, application/x-ndjson");
  http_header(h, "User-Agent", "copenai");
  stralloc_free(&auth);

  if(!use_length)
    http_chunked(h);

  part("{\"model\":\"", 10, 0);
  part(model, str_len(model), 1);
  part("\",\"stream\":true,\"messages\":[", 28, 0);

  if(system_prompt) {
    part("{\"role\":\"system\",\"content\":\"", 28, 0);
    part(system_prompt, str_len(system_prompt), 1);
    part("\"},", 3, 0);
  }

  part("{\"role\":\"user\",\"content\":\"", 26, 0);

  if(argc > 0) {
    int i;

    for(i = 0; i < argc; i++) {
      if(i)
        part(" ", 1, 1);
      part(argv[i], str_len(argv[i]), 1);
    }
  } else {
    /* the prompt goes out in the pieces it is read in */
    while((r = read(0, tmp, sizeof(tmp))) > 0)
      part(tmp, (size_t)r, 1);
  }

  part("\"}]}", 4, 0);

  if(!use_length)
    http_chunk_end(h);
}

int
main(int argc, char* argv[]) {
  static const struct unix_longopt opts[] = {{"model", 1, NULL, 'm'},
                                             {"url", 1, NULL, 'u'},
                                             {"key", 1, NULL, 'k'},
                                             {"system", 1, NULL, 's'},
                                             {"length", 0, NULL, 'l'},
                                             {"ndjson", 0, NULL, 'n'},
                                             {"raw", 0, NULL, 'r'},
                                             {"help", 0, NULL, 'h'},
                                             {0, 0, 0, 0}};
  int c, index = 0, rc, raw = 0, ndjson = 0;
  stralloc url;

  errmsg_iam(argv[0]);

  while((c = unix_getopt_long(argc, argv, "m:u:k:s:lnrh", opts, &index)) != -1) {
    switch(c) {
      case 'm': model = unix_optarg; break;
      case 'u': base = unix_optarg; break;
      case 'k': key = unix_optarg; break;
      case 's': system_prompt = unix_optarg; break;
      case 'l': use_length = 1; break;
      case 'n': ndjson = 1; break;
      case 'r': raw = 1; break;
      case 'h': usage(argv[0]); return 0;
      default: return 2;
    }
  }

  if(!model)
    model = env_get("OPENAI_MODEL");

  if(!model || !*model)
    model = "gpt-4o-mini";

  if(!base)
    base = env_get("OPENAI_API_URL");

  if(!base || !*base)
    base = "https://api.openai.com/v1";

  if(!key)
    key = env_get("OPENAI_API_KEY");

  if(!key)
    key = "";

  stralloc_init(&url);
  stralloc_copys(&url, base);
  while(url.len && url.s[url.len - 1] == '/')
    url.len--;
  stralloc_cats(&url, "/chat/completions");
  stralloc_nul(&url);

  if(!llm_stream_open(&stream, url.s)) {
    errmsg_warn("cannot connect to ", url.s, 0);
    return 1;
  }

  stream.on_value = on_value;
  stream.force_ndjson = ndjson;
  if(raw)
    stream.on_raw = on_raw;

  build_request(argc - unix_optind, argv + unix_optind);

  rc = llm_stream_run(&stream);

  buffer_putnlflush(buffer_1);
  llm_stream_close(&stream);
  return rc;
}
