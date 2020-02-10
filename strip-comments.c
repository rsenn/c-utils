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
put_line(buffer* b, const char* x, ssize_t len) {
  /*  if(len > 1) {
      if((x[len - 1] == '\n' || x[len - 1] == '\r'))

      len--;
    }*/
  if(len > 0)
    buffer_put(b, x, len);
  //   buffer_puts(b, "\n");
}

int
strip_comments(charbuf* in, buffer* out) {
  int c;
  const char* x;
  size_t n;
  char buf[2];
  stralloc line;
  stralloc_init(&line);

  while(!in->eof && !in->err && (c = charbuf_get(in)) > 0) {
    if(c == '/') {
      c = charbuf_peek(in);

      if(c <= 0)
        goto end;

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

    if(c == '\n' && line.len > 0) {
      put_line(out, line.s, line.len);
      stralloc_zero(&line);
    }
  }
end:
  put_line(out, line.s, line.len);
  stralloc_free(&line);
  charbuf_close(in);
}

int
main(int argc, char* argv[]) {
  int in_fd = STDIN_FILENO, out_fd = STDOUT_FILENO;
  stralloc tmp;
  int c;
  int index = 0;
  char buf[16384];
  buffer output;
  int in_place = 0;
  charbuf input;
  stralloc data;

  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"in-place", 0, NULL, 'i'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "hi", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'i': in_place=1;break;
      case 'h': usage(argv[0]); return 0;

      default: usage(argv[0]); return 1;
    }
  }

  stralloc_init(&tmp);
//  stralloc_init(&data);

  if(optind < argc) {
    buffer_putm_internal(buffer_2, "Opening input file '", argv[optind], "'...", 0);
    buffer_putnlflush(buffer_2);
    in_fd = open_read(argv[optind]);
    optind++;
  }
  if(optind < argc) {
    buffer_putm_internal(buffer_2, "Opening output file '", argv[optind], "'...", 0);
    buffer_putnlflush(buffer_2);
    out_fd = open_trunc(argv[optind]);
    optind++;
  }


  charbuf_init(&input, (read_fn*)&read, in_fd);

  if(in_place) {

  }

if(in_place)
  buffer_fromsa(&output, &tmp);
else
  buffer_init(&output, &write, out_fd, buf, sizeof(buf));
  strip_comments(&input, &output);

  buffer_flush(&output);

  

  buffer_puts(buffer_1, "max_depth: ");
  buffer_putulong(buffer_1, 2);
  buffer_putnlflush(buffer_1);

  charbuf_close(&input);
  buffer_close(&output);
}
