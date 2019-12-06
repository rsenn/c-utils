#include "lib/json.h"
#include "lib/open.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/scan.h"
#include "lib/getopt.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static charbuf infile;
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

static void
max_depth_fn(jsonval* v, int* arg, int depth) {
  if(depth > *arg)
    *arg = depth;
}

static int
get_depth(jsonval* v) {
  int max_depth = -1;
  json_recurse(v, max_depth_fn, &max_depth);
  return max_depth;
}

static void
compact_printer(jsonfmt* p, jsonval* v, int depth, int index) {
  int valdepth = v ? get_depth(v) : 0;
  int pretty = depth < 4 && valdepth > 1;
  /*

  if(depth < 2) {
    buffer_putnspace(buffer_2, depth*2);
    buffer_puts(buffer_2, "json_print_separator(");
    buffer_putulong(buffer_2, depth);
    buffer_puts(buffer_2, ", ");
    buffer_putlong(buffer_2, index);
    buffer_puts(buffer_2, ")");
    buffer_putnlflush(buffer_2);
  }*/
  p->newline = "\n"; //(!one_line  && valdepth > 1 && ((index > -1) || index == -2)) ? "\n" : "";
  p->indent = indent_str.s;
  p->spacing = ((valdepth < 1 && index > 0) || (valdepth >= 1 && index > -1)) ? " " : "";
  p->separat = ","; // : ",";
                    p->quote = quote;
//  byte_copy(&p->quote, 2, quote[0] ? quote : "\"");
  p->precision = 16;
};

void
json_pretty_print(jsonval val, buffer* b) {
   void *printfn = compact ? &compact_printer : 0;
  stralloc out;
  stralloc_init(&out);
  json_tosa(val, &out, printfn);

  if(out.len > 16384) {
    json_print(val, b, printfn);
  } else {
    buffer_putsa(b, &out);
  }
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
                       "  -s, --single-quote      Use ' as quote\n"
                       "  -d, --double-quote      Use \" as quote\n"
                       "  -o, --one-line          One-line\n"
                       "  -c, --compact           Compact\n"
                       "  -l, --indent NUM        Indent level\n"
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int fd;
  jsonval* doc;
  stralloc tmp;
  int c;
  int index = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"single-quote", 0, NULL, 's'},
                           {"double-quote", 0, NULL, 'd'},
                           {"one-line", 0, NULL, 'o'},
                           {"compact", 0, NULL, 'c'},
                           {"indent", 0, NULL, 'l'},
                           {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "hsdol:c", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 's': quote[0] = '\''; break;
      case 'd': quote[0] = '"'; break;
      case 'o': one_line = 1; break;
      case 'c': compact = 1; break;
      case 'l': scan_int(optarg, &indent); break;
      default: usage(argv[0]); return 1;
    }
  }

  for(c = 0; c < indent; ++c) stralloc_cats(&indent_str, " ");
  stralloc_nul(&indent_str);

  stralloc_init(&tmp);

  fd = optind < argc ? open_read(argv[optind]) : 0;

  charbuf_init(&infile, (read_fn*)&read, fd);

  doc = json_read_tree(&infile);

  buffer_puts(buffer_1, "max_depth: ");
  buffer_putulong(buffer_1, get_depth(doc));
  buffer_putnlflush(buffer_1);

  json_pretty_print(*doc, buffer_1);

  charbuf_close(&infile);

  json_free(doc);
}
