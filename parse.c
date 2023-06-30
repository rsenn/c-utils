#include "lib/windoze.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/path.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/byte.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/getopt.h"
#include "lib/open.h"

#if WINDOWS
#include <windows.h>
#endif

static stralloc prog;

const char*
get_prog_name(void) {
  ssize_t len;

  stralloc_ready(&prog, PATH_MAX);

#ifndef WINDOWS_NATIVE
  if((len = readlink("/proc/self/exe", prog.s, prog.a)) > 0) {
    prog.len = len;
    return prog.s;
  }
#endif

#if WINDOWS
  if((len = GetModuleFileNameA(0, prog.s, prog.a)) > 0) {
    prog.len = len;
    return prog.s;
  }
#endif

  stralloc_copys(&prog, errmsg_argv0);
  stralloc_nul(&prog);
  return prog.s;
}

int
write_log(const strlist* argv, const char* file) {
  stralloc sa;
  fd_t fd;

  if((fd = open_append(file)) != -1) {
    buffer b;
    buffer_write_fd(&b, fd);

    stralloc_init(&sa);
    strlist_join(argv, &sa, ' ');

    buffer_putsa(&b, &sa);
    buffer_putnlflush(&b);
    buffer_close(&b);

    return 1;
  }
  return 0;
}

ssize_t
parse_string(const char* x, ssize_t n, stralloc* out) {
  const char* start = x;
  ssize_t i, j;

  i = byte_chr(x, n, '"');

  if(i != 0)
    return 0;

  i += 1;

  x += i;
  n -= i;

  i = byte_chr(x, n, '"');

  if(x[i] == '\0')
    return 0;

  stralloc_copyb(out, x, i);
  stralloc_nul(out);

  i += 1;
  x += i;
  n -= i;

  return x - start;
}

ssize_t
parse_strlist(const char* x, ssize_t n, strlist* out) {
  const char* start = x;
  ssize_t i;
  stralloc arg;
  stralloc_init(&arg);
  if(*x != '[')
    return 0;
  x += 1;
  n -= 1;
  for(; (i = parse_string(x, n, &arg));) {
    strlist_push_sa(out, &arg);
    x += i, n -= i;
    if(byte_diff(x, 2, ", "))
      break;
    x += 2;
    n -= 2;
  }
  if(x[0] == ']') {
    x += 1, n -= 1;
  }
  stralloc_free(&arg);
  return x - start;
}

void
print_args(buffer* b, const strlist* sl) {
  stralloc sa;
  stralloc_init(&sa);
  strlist_joins(sl, &sa, "' '");
  buffer_puts(b, "'");
  buffer_putsa(b, &sa);
  buffer_putsflush(b, "'\n");
}

ssize_t
parse_line(const char* x, ssize_t n) {
  ssize_t i;
  strlist args;
  stralloc str;
  strlist_init(&args, '\0');
  stralloc_init(&str);
  i = byte_chr(x, n, '(');
  if(x[i] == '\0')
    return 0;
  i += 1;
  x += i;
  n -= i;
  if(n <= 0)
    return 0;
  for(;;) {
    /*  buffer_puts(buffer_2, "char: ");
      buffer_putc(buffer_2, x[0]);
      buffer_putnlflush(buffer_2);*/
    if(x[0] == '[')
      i = parse_strlist(x, n, &args);
    else if(x[0] == '"')
      i = parse_string(x, n, &str);
    else
      break;
    if(i <= 0)
      break;
    x += i;
    n -= i;
    if(byte_equal(x, 2, ", ")) {
      x += 2;
      n -= 2;
    }
  }

#ifdef DEBUG_OUTPUT
  stralloc_nul(&str);
  buffer_putm_internal(buffer_2, "Command: ", str.s, "\n", "Arguments: ", NULL);
  strlist_dump(buffer_2, &args);
  buffer_putnlflush(buffer_2);
#endif
  print_args(buffer_1, &args);
  return 0;
}

void
usage(char* errmsg_argv0) {
  buffer_putm_internal(buffer_1, "Usage: ", errmsg_argv0, " [-f type] [-t type] [-o " "output] [input or stdin]\n\n", "  -f type  Input type\n" "  -t type  Output type\n" "\n" "Supported types are: m3u pls " "xspf\n", NULL);
  buffer_flush(buffer_1);
  exit(0);
}

int
main(int argc, char* argv[]) {
  ssize_t p;
  int i;
  int in_fd = 0;
  ssize_t line = 0;
  stralloc sa;
  strarray v;
  strlist args;
  char** av;
  int opt;
  buffer* input = buffer_0;
  const char *pathstr, *in_type = NULL;
  const char* logfile = getenv("LOGFILE");

  errmsg_iam(argv[0]);

  strlist_init(&args, '\0');

  get_prog_name();

  while((opt = unix_getopt(argc, argv, "f:h")) != -1) {
    switch(opt) {
      case 'f': in_type = unix_optarg; break;

      case 'h': usage(str_basename(argv[0])); exit(EXIT_SUCCESS);
      default: /* '?' */ buffer_putm_internal(buffer_2, "Usage: ", argv[0], "[-t TYPE] [file]\n", NULL); exit(EXIT_FAILURE);
    }
  }

  {
    const char* in_file = argc > unix_optind ? argv[unix_optind] : "-";

    if(!str_diff(in_file, "-")) {
      in_fd = 0;
    } else if(in_file == NULL || (in_fd = open_read(in_file)) == -1) {
      buffer_putm_internal(buffer_2, "No such file: ", in_file ? in_file : "(null)", "\n", NULL);
      buffer_flush(buffer_2);
      exit(1);
    }
  }

  buffer_read_fd(input, in_fd);

  for(stralloc_init(&sa); buffer_getline_sa(input, &sa); stralloc_zero(&sa)) {
    ++line;
    stralloc_trimr(&sa, "\r\n", 2);

    if(sa.len > 0)
      parse_line(sa.s, sa.len);

    /*    buffer_putulong(buffer_1,
       sa.len); buffer_puts(buffer_1, ":
       "); buffer_put(buffer_1, sa.s,
       sa.len);

        buffer_puts(buffer_1, "\n");
        buffer_flush(buffer_1);
    */
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "errmsg_argv0: ");
  buffer_puts(buffer_2, argv[0]);
  buffer_puts(buffer_2, ", prog: ");
  buffer_putsa(buffer_2, &prog);
  buffer_puts(buffer_2, ", ext: ");
  buffer_puts(buffer_2, ext);
  buffer_putnlflush(buffer_2);
#endif
  return 0;
}
