#include "lib/open.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/scan.h"
#include "lib/getopt.h"
#include "lib/unix.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/path.h"
#include "lib/scan.h"
#include "lib/textcode.h"
#include "lib/alloc.h"
#include "lib/byte.h"
#include "lib/uint8.h"

#include <ctype.h>
#include <stdbool.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

static char quote[4] = {'"', 0};
static int one_line, indent = 2, compact;
static stralloc indent_str, queue, quote_chars;
static buffer output;
static const char* add_quotes = 0;
static int tab_size = -1;
static int quote_newline = false, quote_tabs = false, quote_nul = false;

typedef size_t fmt_function(char*, int, int);

size_t
fmt_default(char* dest, int c, int quote) {
  size_t n;
  uint8 ch = c;

  if(byte_chr(quote_chars.s, quote_chars.len, ch) < quote_chars.len) {
    n = 0;
    if(dest)
      dest[n] = '\\';
    if(c == '\t')
      c = 't';
    else if(c == '\n')
      c = 'n';
    else if(c == 0x1b)
      c = 'e';
    else if(c == '\v')
      c = 'v';
    else if(c == '\b')
      c = 'b';

    n++;
  }

  if(n < 2) {
    if(dest)
      dest[n] = c;
    n++;
  }

  return n;
}

static fmt_function* fmt_call = fmt_default;

size_t consume_output(stralloc* sa, buffer* out);
void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, iscntrl);
  buffer_putsa(b, &esc);
}

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] [FILES...]\n"
                       "\n"
                       "Options:\n"
                       "\n"
                       "  -h, --help              Show this help\n"
                       "  -i, --in-place          Write to input file\n"
                       "  -q, --quote-chars CHARS Characters to quote\n"
                       "  -a, --add-quotes QUOTE  Add quotes of type\n"
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

void
put_line(stralloc* sa, const char* x, ssize_t len) {
  if(len > 1) {
    if((x[len - 1] == '\n' || x[len - 1] == '\r'))

      len--;
    while(len >= 1 && isspace(x[len - 1])) len--;
  }

  if(len > 0) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "put_line(");
    buffer_dump(buffer_2, b);
    buffer_puts(buffer_2, ", ");
    buffer_putptr(buffer_2, x);
    buffer_puts(buffer_2, ", ");
    buffer_putlong(buffer_2, len);
    buffer_puts(buffer_2, ");");
    buffer_putnlflush(buffer_2);
#endif
    stralloc_catb(sa, x, len);
  }
  stralloc_catb(sa, "\n", 1);

  if(len > 0) {
    consume_output(sa, &output);
  }
}

size_t
eat_line(const char** s, size_t n, buffer* out) {
  size_t p, q;
  const char* x = *s;
  p = scan_noncharsetnskip(x, "\n\r", n);
  if(p >= 0) {
    q = scan_charsetnskip(&x[p], "\n\r", n - p);
    if(p == 0 && q == n)
      return p;
    buffer_put(out, x, p + (q > 2 ? 2 : q));
    x += p + q;
    n -= p + q;
  }
  *s = x;
  return p;
}

size_t
consume_output(stralloc* sa, buffer* out) {
  size_t prevlen = -1;
  const char* s = sa->s;
  const char* e = s + sa->len;

  while(s < e) {
    size_t r = eat_line(&s, e - s, out);
    if(r == 0) {
      if(prevlen)
        break;
      //      buffer_puts(out, "\n");
      //  break;
    }
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "eat_line() = ");
    buffer_putlong(buffer_2, r);
    buffer_putnlflush(buffer_2);
#endif
    prevlen = r;
  }

  if(s > sa->s && s < e) {
    stralloc_remove(sa, 0, s - sa->s);
  } else {
    stralloc_zero(sa);
  }
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "remaining ");
  buffer_putlong(buffer_2, e - s);
  buffer_putnlflush(buffer_2);
#endif
  return s - sa->s;
}

static inline bool
do_quote(int ch) {
  return byte_chr(quote_chars.s, quote_chars.len, ch) < quote_chars.len;
}

void
add_output(const char* x, size_t len, buffer* out) {
  size_t i, n;
  char tmp[32];

  for(i = 0; i < len; i++) {
    uint8 ch = x[i];
    unsigned int chlen = fmt_utf8(0, x[i]);

    if(do_quote(x[i]) || chlen > 1 || fmt_call != fmt_default) {
      n = fmt_call(tmp, x[i], 0);
    } else if(iscntrl(ch) || ch > 127) {
      n = 0;
      tmp[n++] = '\\';
      if(ch <= 63)
        tmp[n++] = '0';
      if(ch <= 7)
        tmp[n++] = '0';

      n += fmt_8long(&tmp[n],ch);
    } else {
      tmp[0] = x[i];
      n = 1;
    }

    if(quote_newline && n == 1 && tmp[0] == '\n')
      buffer_puts(out, "\\n");
    else if(quote_tabs && n == 1 && tmp[0] == '\t')
      buffer_puts(out, "\\t");
    else if(quote_nul && n == 1 && tmp[0] == '\0')
      buffer_puts(out, "\\0");
    else
      buffer_put(out, tmp, n);
  }
}

int
run_quote(buffer* in, buffer* out) {
  int c, prev_c, is_empty;
  const char* x;
  size_t p, n, line, col;
  bool ws = false;
  stralloc buf;
  stralloc_init(&buf);
  n = 0;
  line = 0;
  col = 0;
  prev_c = '\0';

  if(add_quotes)
    buffer_puts(out, add_quotes);
  c = 0;
  while(buffer_getc(in, (char*)&c) > 0) {

    if(c == ' ' && prev_c != ' ') {
      add_output(buf.s, buf.len, out);
      stralloc_zero(&buf);
      ws = true;
    }
    if(c != ' ') {
      if(ws)
        ws = false;
    }

    if(ws && tab_size > 0 && buf.len == tab_size - 1) {
      c = '\t';
      stralloc_zero(&buf);
    }

    stralloc_catc(&buf, c);

    n++;
    col++;

    prev_c = c;
  }

  if(buf.len > 0) {
    add_output(buf.s, buf.len, out);
    stralloc_zero(&buf);
  }
  if(add_quotes)
    buffer_puts(out, add_quotes);

  if(c != '\n')
    buffer_putc(out, '\n');

  buffer_flush(out);
  stralloc_free(&buf);
  return 0;
}

int
add_chars(const char* x, size_t len) {
  size_t i;
  int n = 0;

  for(i = 0; i < len; i++) {
    if(byte_chr(quote_chars.s, quote_chars.len, x[i]) == quote_chars.len) {
      stralloc_catb(&quote_chars, &x[i], 1);
      n++;
    }
  }
  return n;
}
int
main(int argc, char* argv[]) {
  int in_fd = STDIN_FILENO, out_fd = STDOUT_FILENO;
  stralloc tmp;
  int c;
  const char *in_path = 0, *out_path = 0;
  int index = 0;
  char buf[16384];
  buffer temp;
  int in_place = 0;
  buffer input;
  stralloc data;
  size_t n;
  const char* x;
  const char* tmpl = "/tmp/quote.XXXXXX";

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"in-place", 0, NULL, 'i'},
                           {"add-quotes", 1, NULL, 'a'},
                           {"tab-size", 1, NULL, 't'},
                           {"quote-chars", 1, NULL, 'q'},
                           {"quote-newline", 0, NULL, 'n'},
                           {"quote-tabs", 0, NULL, 9},
                           {"quote-nul", 0, NULL, '0'},
                           {"no-quote-newline", 0, &quote_newline, false},
                           {"no-quote-tabs", 0, &quote_tabs, false},
                           {"no-quote-nul", 0, &quote_nul, false},
                           {"escape-cmake", 1, NULL, 'C'},
                           {"escape-shell", 0, NULL, 'S'},
                           {"escape-doublequoted-shell", 0, NULL, 'D'},
                           {"escape-quoted-shell", 0, NULL, 'Q'},
                           {"escape-c", 0, NULL, 'c'},
                           {"escape-xml", 0, NULL, 'X'},
                           {"escape-json", 0, NULL, 'J'},
                           {"escape-printable", 0, NULL, 'P'},
                           {0, 0, 0, 0}};

  stralloc_init(&quote_chars);
  stralloc_ready(&quote_chars, 4);

  errmsg_iam(argv[0]);

  for(;;) {
    c = unix_getopt_long(argc, argv, "0a:CcDhiJq:nPQSt:X", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'i': in_place = 1; break;
      case 'h': usage(argv[0]); return 0;
      case 'a': add_quotes = unix_optarg; break;
      case 't': scan_uint(unix_optarg, &tab_size); break;
      case 'q':
        stralloc_ready(&quote_chars, str_len(unix_optarg) * 3 + 1);
        quote_chars.len = quote_chars.a;
        scan_cescape(unix_optarg, quote_chars.s, &quote_chars.len);
        break;
      case 'n': quote_newline = true; break;
      case 9: quote_tabs = true; break;
      case '0':
        quote_nul = true;
        break;
        //    case 'S': stralloc_copys(&quote_chars, "\"$`"); break;
      case 'C':
        add_quotes = "\"";
        tab_size = 2;
        quote_newline = quote_tabs = true;
        stralloc_copys(&quote_chars, "$");
        break;
      case 'c': stralloc_copys(&quote_chars, "\""); break;
      case 'J': fmt_call = (fmt_function*)&fmt_escapecharjson; break;
      case 'P': fmt_call = (fmt_function*)&fmt_escapecharquotedprintable; break;
      case 'S': fmt_call = (fmt_function*)&fmt_escapecharshell; break;
      case 'Q': fmt_call = (fmt_function*)&fmt_escapecharquotedshell; break;
      case 'D': fmt_call = (fmt_function*)&fmt_escapechardoublequotedshell; break;
      case 'X': fmt_call = (fmt_function*)&fmt_escapecharxml; break;

      default: usage(argv[0]); return 1;
    }
  }

  if(add_quotes)
    add_chars(add_quotes, str_len(add_quotes));

  if(quote_newline)
    add_chars("\n", 1);
  if(quote_tabs)
    add_chars("\t", 1);

  if(quote_chars.len > 0)
    add_chars("\\", 1);

  stralloc_init(&tmp);
  //  stralloc_init(&data);

  if(unix_optind < argc) {
#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "Opening input file '", argv[unix_optind], "'...", NULL);
    buffer_putnlflush(buffer_2);
#endif
    in_fd = open_read((in_path = argv[unix_optind]));
    unix_optind++;
  }
  if(unix_optind < argc) {
#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "Opening output file '", argv[unix_optind], "'...", NULL);
    buffer_putnlflush(buffer_2);
#endif
    out_fd = open_trunc((out_path = argv[unix_optind]));
    unix_optind++;
  }

  buffer_init_free(&input, (buffer_op_proto*)&read, in_fd, alloc(1024), 1024);

again:
  if(in_place) {
    stralloc_zero(&tmp);
    path_dirname(in_path, &tmp);
    stralloc_catc(&tmp, '/');

    stralloc_cats(&tmp, "quote.XXXXXX");
    stralloc_nul(&tmp);
    tmpl = tmp.s;

    if(out_fd == STDOUT_FILENO) {
      out_fd = open_temp(&tmpl);
      out_path = tmpl;
    }
  }
  buffer_write_fd(&output, out_fd);
  run_quote(&input, &output);

  buffer_flush(&output);
  close(output.fd);

  if(tmpl) {
    buffer_puts(buffer_1, "tmpl: ");
    buffer_puts(buffer_1, tmpl);
    buffer_putnlflush(buffer_1);
  }
  if(out_path) {
    buffer_puts(buffer_1, "out_path: ");
    buffer_puts(buffer_1, out_path);
    buffer_putnlflush(buffer_1);
  }
  /*
    if((x = mmap_read(out_path, &n)) && n > 1) {

      buffer_puts(buffer_1, "out: ");
      buffer_put(buffer_1, x, n);
      buffer_putnlflush(buffer_1);

      mmap_unmap(x, n);
    } */

  buffer_close(&input);

  if(in_place) {
    // buffer inplace;
    if(unlink(in_path) != 0)
      errmsg_warnsys("unlink: ", in_path, 0);

    if(link(tmpl, in_path) == 0) {
      unlink(tmpl);
    } else {
      errmsg_warnsys("unlink: ", tmpl, 0);
    }

    /*    buffer_truncfile(&inplace, out_path);
        buffer_put(&inplace, tmp.s, tmp.len);
        buffer_flush(&inplace);
        buffer_close(&inplace);
    */

    if(out_path == 0) {
      in_path = argv[unix_optind++];
      in_fd = open_read(in_path);
      goto again;
    }
  }
}
