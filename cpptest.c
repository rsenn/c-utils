#include "lib/cpp.h"
#include "lib/cpp_internal.h"
#include "lib/buffer.h"
#include "lib/unix.h"
#include "lib/open.h"
#include "lib/errmsg.h"
#include "lib/str.h"
#include "lib/path.h"

static bool opt_dump = false;

static int
usage(char* a0) {
  buffer_putm_internal(buffer_2,
                       "example preprocessor\n"
                       "usage: ",
                       path_basename(a0),
                       " [-d] [-I includedir...] [-D define] [-o output] file\n"
                       "if no filename or '-' is passed, stdin is used.\n",
                       NULL);
  buffer_putnlflush(buffer_2);
  return 1;
}

static buffer *out, tmpbuf;

int
main(int argc, char** argv) {
  int ret, c, index = 0;
  char *tmp, *fn;
  int no_process = 0, no_line = 0;
  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"debug", 0, NULL, 'd'},
      {"output", 0, NULL, 'o'},
      {"no-process", 1, &no_process, 'P'},
      {"no-line", 1, &no_line, 'L'},
      {0, 0, 0, 0},
  };

  out = buffer_1;

  errmsg_iam(str_basename(argv[0]));

  while((c = unix_getopt_long(argc, argv, "D:I:o:dhPL", opts, &index)) != -1) {
    switch(c) {
      case 'I': {
        strarray_push(&cpp_include_paths, unix_optarg);
        break;
      }

      case 'D': {
        size_t namelen = str_chr(unix_optarg, '=');
        char* name = str_ndup(unix_optarg, namelen);
        char* value = unix_optarg[namelen] ? &unix_optarg[namelen + 1] : "";

        cpp_macro_define(name, value);

        alloc_free(name);
        break;
      }

      case 'o': {
        if(out == &tmpbuf) {
          buffer_close(&tmpbuf);
          buffer_free(&tmpbuf);
        }

        buffer_truncfile(&tmpbuf, unix_optarg);
        out = &tmpbuf;
        break;
      }

      case 'd': {
        opt_dump = true;
        break;
      }

      case 'P': {
        no_process = 1;
        break;
      }

      case 'L': {
        no_line = 1;
        break;
      }

      case 'h':
      default: {
        return usage(argv[0]);
      }
    }
  }

  fn = "/dev/stdin";
  /*fd = STDIN_FILENO;*/

  if(argv[unix_optind] && str_diff(argv[unix_optind], "-")) {
    fn = argv[unix_optind];
  }

  cpp_token* tok;

  cpp_base_file = fn;

  if(!(tok = cpp_tokenize_file(fn))) {
    errmsg_warnsys("open/read", 0);
    return 1;
  }

  cpp_init_macros();

  cpp_token* tok2 = no_process ? tok : cpp_preprocess(tok);

  if(opt_dump)
    for(cpp_token* t = tok2; t; t = t->next)
      cpp_token_dump(buffer_2, t);

  cpp_print_tokens(out, tok2, !(no_line || no_process));

#ifndef test_x
#define test_x(a,b,c) ((a) && (b) && (c))
#endif

#if test_x(1,1,1)
  printf("blah\n");
#endif

  __UINT32_C(102);

  return !tok2;
}
