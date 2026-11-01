#include "llmstream.h"

#include "../../lib/buffer.h"
#include "../../lib/byte.h"
#include "../../lib/env.h"
#include "../../lib/errmsg.h"
#include "../../lib/ip4.h"
#include "../../lib/scan.h"
#include "../../lib/socket.h"
#include "../../lib/str.h"
#include "../../lib/stralloc.h"
#include "../../lib/unix.h"

#include <signal.h>
#include <unistd.h>

#define EOT "\x04\n"
#define CAN "\x18\n"

static const char *model = "gemini-2.0-flash", *base = "https://generativelanguage.googleapis.com/v1beta", *key,
                  *system_prompt;
static int interactive;
static llm_stream stream;
static buffer *sin, *sout;
/* the conversation so far, the JSON of the items of "contents" */
static stralloc history;
/* the answer to the turn that is running */
static stralloc reply;
static stralloc url;

static void
usage(const char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [OPTIONS]\n"
                       "\n"
                       "A conversation with Google Gemini, the answers are written as they arrive.\n"
                       "The API key is taken from $GEMINI_API_KEY. A line is a message to the\n"
                       "model, a line that ends in a backslash goes on in the next one.\n"
                       "\n"
                       "On a terminal there is a prompt and the answer follows the line. When\n"
                       "standard input is a pipe, or with -l a client of the socket, there is\n"
                       "no prompt and the answer to every line ends with a line that holds\n"
                       "only the byte 0x04 (EOT), or 0x18 (CAN) if the request failed.\n"
                       "\n"
                       "  -m, --model NAME     model, default gemini-2.0-flash\n"
                       "  -u, --url URL        API base url, default\n"
                       "                       https://generativelanguage.googleapis.com/v1beta\n"
                       "  -s, --system TEXT    system instruction\n"
                       "  -l, --listen [ADDR:]PORT\n"
                       "                       serve conversations on a TCP socket instead of standard\n"
                       "                       input, one for each client; ADDR is 127.0.0.1 unless\n"
                       "                       given, everyone who connects spends your quota\n"
                       "  -h, --help           show this help\n"
                       "\n"
                       "Commands: /reset forgets the conversation, /quit leaves.\n",
                       NULL);
  buffer_flush(buffer_1);
}

/* the text of the parts of the first candidate */
static void
on_value(llm_stream* s, jsonval* v, void* user) {
  int i;
  jsonval block = json_property_path(*v, "promptFeedback.blockReason");

  (void)user;

  if(block.type == JSON_STRING) {
    stralloc_nul(&block.stringv);
    errmsg_warn("the prompt was blocked: ", block.stringv.s, 0);
    s->failed = 1;
    return;
  }

  for(i = 0; i < 64; i++) {
    stralloc path;
    jsonval text;

    stralloc_init(&path);
    stralloc_cats(&path, "candidates[0].content.parts[");
    stralloc_catulong(&path, (unsigned long)i);
    stralloc_cats(&path, "].text");
    stralloc_nul(&path);
    text = json_property_path(*v, path.s);
    stralloc_free(&path);

    if(text.type != JSON_STRING)
      break;

    buffer_put(sout, text.stringv.s, text.stringv.len);
    buffer_flush(sout);
    stralloc_catb(&reply, text.stringv.s, text.stringv.len);
  }
}

static void
part(const char* x, size_t n, int escape) {
  llm_stream_chunk(&stream, x, n, escape, 0);
}

/* sends one turn of the conversation, 0 if the model answered */
static int
ask(const char* text, size_t n) {
  http* h;
  stralloc turn;
  int failed;

  if(!llm_stream_open(&stream, url.s)) {
    errmsg_warn("cannot connect to ", url.s, 0);
    return 1;
  }

  h = &stream.h;
  stream.on_value = on_value;
  stralloc_zero(&reply);

  http_header(h, "x-goog-api-key", key);
  http_header(h, "Content-Type", "application/json");
  http_header(h, "Accept", "text/event-stream, application/x-ndjson");
  http_chunked(h);

  /* the item for this turn is kept for the history */
  stralloc_init(&turn);
  stralloc_cats(&turn, "{\"role\":\"user\",\"parts\":[{\"text\":\"");
  llm_json_escape(&turn, text, n);
  stralloc_cats(&turn, "\"}]}");

  part("{\"contents\":[", 13, 0);
  if(history.len) {
    part(history.s, history.len, 0);
    part(",", 1, 0);
  }
  part(turn.s, turn.len, 0);
  part("]", 1, 0);

  if(system_prompt) {
    part(",\"systemInstruction\":{\"parts\":[{\"text\":\"", 40, 0);
    part(system_prompt, str_len(system_prompt), 1);
    part("\"}]}", 4, 0);
  }

  part("}", 1, 0);
  http_chunk_end(h);

  failed = llm_stream_run(&stream);
  llm_stream_close(&stream);

  if(!failed && reply.len) {
    if(history.len)
      stralloc_cats(&history, ",");
    stralloc_catb(&history, turn.s, turn.len);
    stralloc_cats(&history, ",{\"role\":\"model\",\"parts\":[{\"text\":\"");
    llm_json_escape(&history, reply.s, reply.len);
    stralloc_cats(&history, "\"}]}");
  }

  stralloc_free(&turn);
  return failed;
}

/* reads a message, lines that end in a backslash go on, 0 at the end of the input */
static int
read_message(buffer* in, stralloc* msg) {
  stralloc line;
  int got = 0;

  stralloc_init(&line);
  stralloc_zero(msg);

  for(;;) {
    stralloc_zero(&line);

    if(buffer_getline_sa(in, &line) <= 0)
      break;

    stralloc_trimr(&line, "\r\n", 2);
    got = 1;

    if(line.len && line.s[line.len - 1] == '\\') {
      line.len--;
      stralloc_cat(msg, &line);
      stralloc_cats(msg, "\n");
      continue;
    }

    stralloc_cat(msg, &line);
    break;
  }

  stralloc_free(&line);
  return got;
}

/* a conversation on in and out until the input is at its end */
static void
session(buffer* in, buffer* out) {
  stralloc msg;

  sin = in;
  sout = out;
  stralloc_init(&msg);
  stralloc_zero(&history);

  for(;;) {
    int failed;

    if(interactive) {
      buffer_puts(out, "you> ");
      buffer_flush(out);
    }

    if(!read_message(in, &msg))
      break;

    if(msg.len == 0)
      continue;

    stralloc_nul(&msg);

    if(msg.s[0] == '/') {
      if(str_equal(msg.s, "/quit") || str_equal(msg.s, "/exit"))
        break;

      if(str_equal(msg.s, "/reset")) {
        stralloc_zero(&history);
        if(interactive)
          buffer_putsflush(out, "(conversation forgotten)\n");
        continue;
      }
    }

    if(interactive)
      buffer_puts(out, "gemini> ");

    failed = ask(msg.s, msg.len);

    if(interactive)
      buffer_putsflush(out, "\n");
    else
      buffer_putsflush(out, failed ? CAN : EOT);
  }

  stralloc_free(&msg);
}

static ssize_t
fd_read(int fd, void* x, size_t n, buffer* b) {
  (void)b;
  return read(fd, x, n);
}

static ssize_t
fd_write(int fd, void* x, size_t n, buffer* b) {
  (void)b;
  return write(fd, x, n);
}

/* the conversations of the clients of a socket */
static int
serve(const char* ip, uint16 port) {
  int s;
  char addr[16];

  if(!scan_ip4(ip, addr)) {
    errmsg_warn("bad address ", ip, 0);
    return 2;
  }

  if((s = socket_tcp4b()) == -1 || socket_bind4_reuse(s, addr, port) == -1 || socket_listen(s, 8) == -1) {
    errmsg_warnsys("cannot listen on ", ip, 0);
    return 1;
  }

  /* the clients are not waited for */
  signal(SIGCHLD, SIG_IGN);

  for(;;) {
    char peer[16];
    uint16 peer_port;
    int c;

    if((c = socket_accept4(s, peer, &peer_port)) == -1)
      continue;

    if(fork() == 0) {
      buffer in, out;
      char inbuf[4096], outbuf[4096];

      close(s);
      buffer_init(&in, fd_read, c, inbuf, sizeof(inbuf));
      buffer_init(&out, fd_write, c, outbuf, sizeof(outbuf));
      session(&in, &out);
      close(c);
      _exit(0);
    }

    close(c);
  }
}

int
main(int argc, char* argv[]) {
  static const struct unix_longopt opts[] = {{"model", 1, NULL, 'm'},
                                             {"url", 1, NULL, 'u'},
                                             {"system", 1, NULL, 's'},
                                             {"listen", 1, NULL, 'l'},
                                             {"help", 0, NULL, 'h'},
                                             {0, 0, 0, 0}};
  int c, index = 0;
  const char* listen_arg = 0;

  errmsg_iam(argv[0]);

  while((c = unix_getopt_long(argc, argv, "m:u:s:l:h", opts, &index)) != -1) {
    switch(c) {
      case 'm': model = unix_optarg; break;
      case 'u': base = unix_optarg; break;
      case 's': system_prompt = unix_optarg; break;
      case 'l': listen_arg = unix_optarg; break;
      case 'h': usage(argv[0]); return 0;
      default: return 2;
    }
  }

  if(!(key = env_get("GEMINI_API_KEY")) || !*key) {
    errmsg_warn("the environment variable GEMINI_API_KEY is not set", 0);
    return 2;
  }

  /* a client that went away must not end the process */
  signal(SIGPIPE, SIG_IGN);

  stralloc_init(&url);
  stralloc_copys(&url, base);
  while(url.len && url.s[url.len - 1] == '/')
    url.len--;
  stralloc_cats(&url, "/models/");
  stralloc_cats(&url, model);
  stralloc_cats(&url, ":streamGenerateContent?alt=sse");
  stralloc_nul(&url);

  if(listen_arg) {
    char ip[64];
    size_t colon = str_rchr(listen_arg, ':'), n;
    unsigned int port = 0;

    if(listen_arg[colon]) {
      n = colon < sizeof(ip) - 1 ? colon : sizeof(ip) - 1;
      byte_copy(ip, n, listen_arg);
      ip[n] = '\0';
      n = scan_uint(listen_arg + colon + 1, &port);
    } else {
      str_copy(ip, "127.0.0.1");
      n = scan_uint(listen_arg, &port);
    }

    if(!n || port == 0 || port > 65535) {
      errmsg_warn("bad port in ", listen_arg, 0);
      return 2;
    }

    return serve(ip, (uint16)port);
  }

  interactive = isatty(0);
  session(buffer_0, buffer_1);
  return 0;
}
