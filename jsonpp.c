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
static int one_line, indent = 2, compact, depth_arg = 3;
static stralloc indent_str;
static const char *spacing, *separator;

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
get_depth(const jsonval* v) {
  int max_depth = -1;
  json_recurse((jsonval*)v, max_depth_fn, &max_depth);
  return max_depth;
}

static void
compact_printer(jsonfmt* p, jsonval* v, int depth, int index, char quot) {

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
  p->newline = "\n";
  p->spacing = " ";
  p->separat = ",\n";
  p->quote = quote;
  p->precision = 3;
  p->depth = depth;
  p->index = index;
};

static void
default_printer(jsonfmt* p, jsonval* v, int depth, int index, char quot) {
  int pretty = v && get_depth(v) > 1;

  p->indent = depth > depth_arg ? "" : "  "; // depth <= 1 ? "  " : depth > 3 ? "  " : " ";
  p->spacing = spacing ? spacing : " ";
  p->newline = depth > depth_arg ? p->spacing : "\n";
  p->separat = separator ? separator : depth > depth_arg ? ", " : ",\n";
  p->quote = quote;
  p->precision = 10;
  p->depth = depth - (index == -2);
  p->index = index;
};

void
json_pretty_print(jsonval val, buffer* b) {
  json_print_fn* printfn = compact ? &compact_printer : &default_printer;

  json_print(val, b, printfn);
  /*  stralloc out;
   stralloc_init(&out);
   json_tosa(val, &out, printfn);

   if(out.len > 16384) {
     json_print(val, b, printfn);
   } else {
     buffer_putsa(b, &out);
   } */
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
                       "  -D, --depth NUM         Depth level\n"
                       "  -S, --separator CHARS   Separator\n"
                       "  -W, --spacing CHARS     Spacing\n"
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

ssize_t
charbuf_read(fd_t fd, char* buf, size_t len, void* ptr) {
  ssize_t r = read(fd, buf, len);

#ifdef DEBUG_OUTPUT_
  if(r > 0) {
    buffer_puts(buffer_2, "Read ");
    buffer_putulong(buffer_2, r);
    buffer_puts(buffer_2, " bytes: '");
    buffer_put(buffer_2, buf, r);
    buffer_putnlflush(buffer_2);
  }
#endif
  return r;
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
                           {"depth", 0, NULL, 'D'},
                           {"separator", 0, NULL, 'S'},
                           {"spacing", 0, NULL, 'W'},
                           {"one-line", 0, NULL, 'o'},
                           {"compact", 0, NULL, 'c'},
                           {"indent", 0, NULL, 'l'},
                           {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "hsdol:cD:S:W:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 's': quote[0] = '\''; break;
      case 'd': quote[0] = '"'; break;
      case 'S': separator = optarg; break;
      case 'W': spacing = optarg; break;
      case 'D': scan_int(optarg, &depth_arg); break;
      case 'o': one_line = 1; break;
      case 'c': compact = 1; break;
      case 'l': scan_int(optarg, &indent); break;
      default: usage(argv[0]); return 1;
    }
  }

  stralloc_init(&indent_str);
  for(c = 0; c < indent; ++c) stralloc_cats(&indent_str, " ");
  stralloc_nul(&indent_str);

  stralloc_init(&tmp);

  fd = optind < argc ? open_read(argv[optind]) : 0;

  charbuf_init(&infile, (read_fn*)&read, fd);

  doc = json_read_tree(&infile);

  //  buffer_puts(buffer_2, "max_depth: ");
  //  buffer_putulong(buffer_2, get_depth(doc));
  //  buffer_putnlflush(buffer_2);

  json_pretty_print(*doc, buffer_1);
  buffer_flush(buffer_1);

  charbuf_close(&infile);

  json_free(doc);
}
