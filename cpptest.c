#include "lib/cpp.h"
#include "lib/unix.h"
#include "lib/open.h"
#include "lib/errmsg.h"
#include "lib/str.h"

cpp* pp;

static int
usage(char* a0) {
  buffer_putm_internal(buffer_2,
                       "example preprocessor\n"
                       "usage: ",
                       a0,
                       " [-I includedir...] [-D define] file\n"
                       "if no filename or '-' is passed, stdin is used.",
                       NULL);
  buffer_putnlflush(buffer_2);
  return 1;
}

int
main(int argc, char** argv) {
  int ret, c;
  char *tmp, *fn;
  buffer in;
  fd_t fd;

  errmsg_iam(str_basename(argv[0]));

  pp = cpp_new();

  while((c = unix_getopt(argc, argv, "D:I:")) != -1) {
    switch(c) {
      case 'I': cpp_add_includedir(pp, unix_optarg); break;
      case 'D':
        if(*(tmp = unix_optarg + str_chr(unix_optarg, '=')) == '=')
          *tmp = ' ';
        cpp_add_define(pp, unix_optarg);
        break;
      default: return usage(argv[0]);
    }
  }

  fn = "<stdin>";
  fd = STDIN_FILENO;

  if(argv[unix_optind] && str_diff(argv[unix_optind], "-")) {
    fn = argv[unix_optind];

    if((fd = open_read(fn)) == -1) {
      errmsg_warnsys("open_read", 0);
      return 1;
    }
  }

  buffer_read_fd(&in, fd);
  ret = cpp_run(pp, &in, buffer_1, fn);
  cpp_free(pp);

  if(in.fd != STDIN_FILENO)
    buffer_close(&in);

  return !ret;
}
