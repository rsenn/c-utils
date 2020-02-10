#include "lib/open.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/scan.h"
#include "lib/getopt.h"
#include "lib/charbuf.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include <ctype.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

static charbuf infile;
static buffer* outbuf;
static char quote[4] = {'"', 0};
static int one_line, indent = 2, compact;
static stralloc indent_str;

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
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
put_line(buffer* b, const char* x, size_t len) {
  while(len > 1) {
    if(!(x[len - 1] == '\n' || x[len - 1] == '\r' || isspace(x[len - 1])))
      break;
    len--;
  }
  if(len > 0) {
    buffer_put(b, x, len);
    buffer_puts(b, "\n");
  }
}

int
strip_comments(charbuf* in) {
  int c;
  const char* x;
  size_t n;
  char buf[2];
  stralloc line;
  stralloc_init(&line);

  while((c = charbuf_get(in)) > 0) {

    if(c == '/') {
      c = charbuf_peek(in);

      if(c == '/') {
        charbuf_skip_until(in, '\n');
        continue;
      } else if(c == '*') {
        charbuf_skip(in);
        do {
          if(charbuf_skip_until(in, '*') <= 0)
            goto end;
          charbuf_skip(in);
          c = charbuf_peek(in);
        } while(c != '/');
      }
    }

    stralloc_catc(&line, c);

    if(c != '\n')
      continue;

    put_line(outbuf, line.s, line.len);

   n = scan_charsetnskip(line.s, " \t\r\v", line.len);
   if((n < line.len && line.s[n] == '\n') || n == line.len)
    continue;


    stralloc_zero(&line);
  }
end:
  put_line(outbuf, line.s, line.len);

  buffer_flush(outbuf);
}

int
main(int argc, char* argv[]) {
  int fd;
  stralloc tmp;
  int c;
  int index = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"indent", 0, NULL, 'l'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "h", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;

      default: usage(argv[0]); return 1;
    }
  }

  stralloc_init(&tmp);

  outbuf = buffer_1;

  fd = optind < argc ? open_read(argv[optind]) : 0;

  charbuf_init(&infile, (read_fn*)&read, fd);

  strip_comments(&infile);

  buffer_puts(buffer_1, "max_depth: ");
  buffer_putulong(buffer_1, 2);
  buffer_putnlflush(buffer_1);

  charbuf_close(&infile);
}
