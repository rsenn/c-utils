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

    stralloc_copys(&prog, argv0);
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

void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
              "Usage: ",
              argv0,
              " [-f type] [-t type] [-o output] [input or stdin]\n\n",
              "  -f type  Input type\n"
              "  -t type  Output type\n"
              "\n"
              "Supported types are: m3u pls xspf\n", 0);
  buffer_flush(buffer_1);
  exit(0);
}


int
main(int argc, char* argv[]) {
  size_t p;
  int i;
  stralloc sa;
  strarray v;
  strlist args;
  char** av;
  int ret;
  int opt;
  const char* pathstr, *in_type = NULL;
  const char* logfile = getenv("LOGFILE");

  errmsg_iam(argv[0]);

  strlist_init(&args, '\0');

  get_prog_name();


  while((opt = getopt(argc, argv, "f:h")) != -1) {
    switch(opt) {
      case 'f': in_type = optarg; break;
    
      case 'h': usage(str_basename(argv[0])); exit(EXIT_SUCCESS);
      default: /* '?' */ buffer_putm_3(buffer_2, "Usage: ", argv[0], "[-t TYPE] [file]\n"); exit(EXIT_FAILURE);
    }
  }

  {
    const char* in_file = argc > optind ? argv[optind] : "-";
    int in_fd;

    if(!str_diff(in_file, "-")) {
      in_fd = 0;
    } else if(in_file == NULL || (in_fd = open_read(in_file)) == -1) {
      buffer_putm_3(buffer_2, "No such file: ", in_file ? in_file : "(null)", "\n");
      buffer_flush(buffer_2);
      exit(1);
    }
  }


#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "argv0: ");
  buffer_puts(buffer_2, argv[0]);
  buffer_puts(buffer_2, ", prog: ");
  buffer_putsa(buffer_2, &prog);
  buffer_puts(buffer_2, ", ext: ");
  buffer_puts(buffer_2, ext);
  buffer_putnlflush(buffer_2);
#endif
  return 0;
}
