#include "lib/open.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/scan.h"
#include "lib/getopt.h"
#include "lib/charbuf.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include <ctype.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

static char quote[4] = {'"', 0};
static int one_line, indent = 2, compact;
static stralloc indent_str, queue;
static buffer output;

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
                       "\n",
                       0);
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
      put_line(&queue, line.s, line.len);
      stralloc_zero(&line);
      continue;
    }
    stralloc_catb(&line, buf, 1);
  }
end:
  put_line(&queue, line.s, line.len);
  if(queue.len > 0)
    consume_output(&queue, out);

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
  buffer temp;
  int in_place = 0;
  charbuf input;
  stralloc data;
  size_t n;
  const char* x;
  const char* tmpl = "/tmp/strip-comments.XXXXXX";

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

again:
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

  charbuf_close(&input);

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
      in_path = argv[optind++];
      in_fd = open_read(in_path);
      goto again;
    }
  }
}
