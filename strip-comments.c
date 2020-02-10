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
                       "  -i, --in-place          Write to input file\n"
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
put_line(buffer* b, const char* x, ssize_t len) {
  if(len > 1) {
    if((x[len - 1] == '\n' || x[len - 1] == '\r'))

      len--;
  }
  if(len > 0) {
    buffer_puts(buffer_2, "put_line(");
    buffer_dump(buffer_2, b);
    buffer_puts(buffer_2, ", ");
    buffer_putptr(buffer_2, x);
    buffer_puts(buffer_2, ", ");
    buffer_putlong(buffer_2, len);
    buffer_puts(buffer_2, ");");
    buffer_putnlflush(buffer_2);

    buffer_put(b, x, len);
  }
  buffer_puts(b, "\n");
}

int
strip_comments(charbuf* in, buffer* out) {
  int c, is_empty;
  const char* x;
  size_t p, n;
  char buf[2];
  stralloc line;
  stralloc_init(&line);
  n = 0;
  while((c = charbuf_get(in)) > 0) {

    if(c == '/') {
      int c2 = charbuf_peek(in);
      if(c2 == '/') {
        charbuf_skip_until(in, '\n');
        /*   charbuf_skip(in);
           stralloc_catc(&line, '\n');*/
        continue;
      } else if(c2 == '*') {
        charbuf_skip(in);
        do {
          if(charbuf_skip_until(in, '*') <= 0)
            break;
          charbuf_skip(in);
          c2 = charbuf_peek(in);
        } while(c2 != '/');
        charbuf_skip(in);
        continue;
      }
    }
    buf[0] = c;

    //   buffer_putc(out, c);
    n++;
    if(c == '\n') {
      /*      p = scan_charsetnskip(line.s, " \t\r\v", line.len);
          is_empty = line.len == 0 || p == line.len || (line.s[line.len - 1] == '\n' && p == line.len - 1);
           if(is_empty)
               continue;*/
      put_line(out, line.s, line.len);
      stralloc_zero(&line);
      continue;
    }
    stralloc_catb(&line, buf, 1);
  }
end:
  put_line(out, line.s, line.len);
  buffer_flush(out);
  stralloc_free(&line);
  charbuf_close(in);
}

int
main(int argc, char* argv[]) {
  int in_fd = STDIN_FILENO, out_fd = STDOUT_FILENO;
  stralloc tmp;
  int c;
  const char *in_path = 0, *out_path = 0;
  int index = 0;
  char buf[16384];
  buffer output, temp;
  int in_place = 0;
  charbuf input;
  stralloc data;
  size_t n;
  const char* x;
  const char* tmpl = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"in-place", 0, NULL, 'i'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "hi", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'i': in_place = 1; break;
      case 'h': usage(argv[0]); return 0;

      default: usage(argv[0]); return 1;
    }
  }

  stralloc_init(&tmp);
  //  stralloc_init(&data);

  if(optind < argc) {
    buffer_putm_internal(buffer_2, "Opening input file '", argv[optind], "'...", 0);
    buffer_putnlflush(buffer_2);
    in_fd = open_read((in_path = argv[optind]));
    optind++;
  }
  if(optind < argc) {
    buffer_putm_internal(buffer_2, "Opening output file '", argv[optind], "'...", 0);
    buffer_putnlflush(buffer_2);
    out_fd = open_trunc((out_path = argv[optind]));
    optind++;
  }

  charbuf_init(&input, (read_fn*)&read, in_fd);

  if(in_place) {
    if(out_fd == STDOUT_FILENO) {

      out_fd = open_temp(&tmpl);

      out_path = tmpl;
    }
  }
  buffer_write_fd(&output, out_fd);
  strip_comments(&input, &output);

  buffer_flush(&output);
  close(output.fd);

  buffer_puts(buffer_1, "tmpl: ");
  buffer_puts(buffer_1, tmpl);
  buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "out_path: ");
  buffer_puts(buffer_1, out_path);
  buffer_putnlflush(buffer_1);

  if((x = mmap_read(out_path, &n)) && n > 1) {

    buffer_puts(buffer_1, "out: ");
    buffer_put(buffer_1, x, n);
    buffer_putnlflush(buffer_1);

    mmap_unmap(x, n);
  }

  if(in_place) {
    // buffer inplace;
      unlink(in_path);
      link(tmpl, in_path);
/*    buffer_truncfile(&inplace, out_path);
    buffer_put(&inplace, tmp.s, tmp.len);
    buffer_flush(&inplace);
    buffer_close(&inplace);
*/  }

charbuf_close(&input);
}
