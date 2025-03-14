#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/json.h"
#include "lib/open.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/scan.h"
#include "lib/unix.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static char quote[4] = {'"', 0};
static int one_line, indent = 2, compact, depth_arg = 3, in_place, no_compliant;
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
max_depth_fn(jsonval* v, int* in_file, int depth) {
  if(depth > *in_file)
    *in_file = depth;
}

static int
get_depth(const jsonval* v) {
  int max_depth = -1;
  json_recurse((jsonval*)v, max_depth_fn, &max_depth);
  return max_depth;
}

static void
compact_printer(jsonfmt* p, jsonval* v, int depth, int index, char q) {
  int valdepth = v ? get_depth(v) : 0;
  int multi_line = !one_line && depth < depth_arg;

  p->indent = !multi_line || depth > depth_arg ? "" : indent_str.s;

  p->newline = multi_line ? "\n" : "";
  p->spacing = spacing ? spacing : compact ? "" : " ";

  p->separat = separator ? separator : multi_line ? ",\n" : p->spacing[0] ? ", " : ",";
  p->quote[0] = quote[0];
  p->quote[1] = quote[1];
  p->precision = 3;
  p->depth = depth;
  p->index = index;
  p->compliant = !no_compliant;
};

static void
default_printer(jsonfmt* p, jsonval* v, int depth, int index, char q) {
  int multi_line = !one_line && depth < depth_arg;
  p->indent = depth > depth_arg ? "" : indent_str.s;

  p->spacing = spacing ? spacing : compact ? "" : " ";
  p->newline = multi_line ? "\n" : p->spacing;
  p->separat = separator ? separator : multi_line ? ",\n" : p->spacing[0] ? ", " : ",";
  p->quote[0] = quote[0];
  p->quote[1] = quote[1];
  p->precision = 10;
  p->depth = depth - (index == -2);
  p->index = index;
  p->compliant = !no_compliant;
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
                       " [OPTIONS] [FILES...]\n\n",
                       "Options:\n\n",
                       "  -h, --help              Show this help\n",
                       "  -s, --single-quote      Use ' as quote\n",
                       "  -d, --double-quote      Use \" as quote\n",
                       "  -O, --one-line          One-line\n",
                       "  -c, --compact           Compact\n",
                       "  -l, --indent NUM        Indent level\n",
                       "  -D, --depth NUM         Depth level\n",
                       "  -S, --separator CHARS   Separator\n",
                       "  -W, --spacing CHARS     Spacing\n\n",
                       "  -C, --compliant         Compliant\n",
                       "",
                       NULL);
  buffer_flush(buffer_1);
}

ssize_t
charbuf_read(fd_type fd, char* buf, size_t len, void* ptr) {
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
  stralloc tmp;
  int c;
  int index = 0;
  int out_fd = 1;
  char* out_file = 0;
  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"single-quote", 0, NULL, 's'},
      {"double-quote", 0, NULL, 'd'},
      {"depth", 0, NULL, 'D'},
      {"separator", 0, NULL, 'S'},
      {"spacing", 0, NULL, 'W'},
      {"one-line", 0, NULL, 'O'},
      {"compact", 0, NULL, 'c'},
      {"no-compliant", 0, NULL, 'C'},
      {"inplace", 0, NULL, 'i'},
      {"indent", 0, NULL, 'l'},
      {"output", 1, NULL, 'o'},
      {0, 0, 0, 0},
  };

  errmsg_iam(argv[0]);

  for(;;) {
    c = unix_getopt_long(argc, argv, "hsdOl:cD:S:W:iCo:", opts, &index);

    if(c == -1)
      break;

    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 's': quote[0] = '\''; break;
      case 'd': quote[0] = '"'; break;
      case 'S': separator = unix_optarg; break;
      case 'W': spacing = unix_optarg; break;
      case 'D': scan_int(unix_optarg, &depth_arg); break;
      case 'O': one_line = 1; break;
      case 'o': out_file = unix_optarg; break;
      case 'c': {
        if(compact)
          spacing = "";
        compact = 1;
        break;
      }
      case 'C': no_compliant = 1; break;
      case 'l': scan_int(unix_optarg, &indent); break;
      case 'i': in_place = 1; break;
      default: usage(argv[0]); return 1;
    }
  }

  if(!no_compliant) {
    quote[0] = quote[1] = '"';
  }

  stralloc_init(&indent_str);

  for(c = 0; c < indent; ++c)
    stralloc_cats(&indent_str, " ");
  stralloc_nul(&indent_str);

  stralloc_init(&tmp);

  if(unix_optind == argc) {
    argv[++argc] = NULL;
  }

#ifdef DEBUG_OUTPUT_
  if(out_file) {
    buffer_puts(buffer_2, "out file: ");
    buffer_puts(buffer_2, out_file);
    buffer_putnlflush(buffer_2);
  }
#endif

  if(unix_optind == argc)
    argv[argc++] = 0;

  while(unix_optind < argc) {
    charbuf in_buf;
    buffer out_buf;
    jsonval* doc;
    const char* in_file = argv[unix_optind];

    unix_optind++;

    fd = in_file ? open_read(in_file) : 0;

    charbuf_init(&in_buf, (read_fn*)(void*)&read, fd, 5);

    if(out_fd > 2)
      close(out_fd);

    if(in_place) {
      const char tmpl[] = ".tmpXXXXXX";
      out_file = alloc(str_len(in_file) + str_len(tmpl));

      str_copy(&out_file[str_copyn(out_file, in_file, str_rchr(in_file, '.'))], tmpl);

      out_fd = open_temp(&out_file);
    } else if(out_file) {
      out_fd = open_trunc(out_file);
    }

#ifdef DEBUG_OUTPUT_
    if(out_fd != -1) {
      buffer_puts(buffer_2, "out fd: ");
      buffer_putlong(buffer_2, out_fd);
      buffer_putnlflush(buffer_2);
    }
#endif

    buffer_write_fd(&out_buf, out_fd);

    doc = json_read_tree(&in_buf);

#ifdef DEBUG_CHARBUF
    charbuf_dump(&in_buf, buffer_2);
    buffer_putnlflush(buffer_2);
#endif

    json_pretty_print(*doc, &out_buf);
    (one_line ? &buffer_putnlflush : &buffer_flush)(&out_buf);

    charbuf_close(&in_buf);
    buffer_close(&out_buf);

    json_free(doc);

    if(in_place) {
      if(unlink(in_file) == -1) {
        errmsg_infosys("unlink", 0);
        return 1;
      }

      if(rename(out_file, in_file) == -1) {
        errmsg_infosys("rename", 0);
        return 1;
      }
    }
  }
}
