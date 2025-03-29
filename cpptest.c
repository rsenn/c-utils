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
  int no_process = 0, no_line = 0, dump_defines = 0, show_deps = 0;
  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"debug", 0, NULL, 'd'},
      {"output", 0, NULL, 'o'},
      {"no-process", 1, &no_process, 'P'},
      {"no-line", 1, &no_line, 'L'},
      {"dump-defines", 1, &dump_defines, 'x'},
      {"MM", 1, &show_deps, 64},
      {0, 0, 0, 0},
  };

  out = buffer_1;

  errmsg_iam(str_basename(argv[0]));

  while((c = unix_getopt_long(argc, argv, "D:I:o:dhPLx", opts, &index)) != -1) {
    switch(c) {
      case 'I': {
        strarray_push(&cpp_include_paths, unix_optarg);
        break;
      }

      case 'D': {
        size_t namelen = str_chr(unix_optarg, '=');
        char* name = str_ndup(unix_optarg, namelen);
        char* value = unix_optarg[namelen] ? &unix_optarg[namelen + 1] : "";

        cpp_define(name, value);

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

      case 'x': {
        dump_defines = 1;
        break;
      }
      case 64: {
        show_deps = 1;
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

  cpp_macro** ptr = cpp_macro_ptr;
  cpp_token *t, *u;

  for(t = tok; t->kind != TK_EOF; t = t->next) {
    if(!t->at_bol)
      continue;

    if(cpp_is_hash(t)) {

      t = t->next;

      if(cpp_equal(t, "include"))
        continue;

      if(cpp_equal(t, "define"))
        continue;

      if(cpp_equal(t, "endif"))
        continue;

      for(u = t->next; u->kind != TK_EOF; u = u->next) {
        if(u->at_bol)
          break;

        if(u->kind == TK_IDENT)
          if(!cpp_equal(u, "defined"))
            // if(!cpp_is_keyword(u))
            cpp_token_dump(buffer_2, u);
      }
    }
  }

  cpp_token* tok2 = no_process ? tok : cpp_preprocess(tok);

  if(opt_dump)
    for(cpp_token* t = tok2; t; t = t->next)
      cpp_token_dump(buffer_2, t);

  cpp_print_tokens(out, tok2, !(no_line || no_process));

  if(dump_defines)
    for(cpp_macro* m = *ptr; m; m = m->next) {
      cpp_print_macro(buffer_2, m);

      buffer_flush(buffer_2);

      if(&m->next == cpp_macro_ptr)
        break;
    }

  char** inc;

  buffer_put(buffer_2, fn, str_rchr(fn, '.'));
  buffer_puts(buffer_2, ".o:");

  strarray_foreach(&include_array, inc) {
    if(path_is_absolute(*inc))
      continue;

    size_t n = path_is_dotslash(*inc);

    buffer_putc(buffer_2, ' ');
    buffer_puts(buffer_2, *inc + n);
  }
  
  buffer_putnlflush(buffer_2);

#ifndef test_x
#define test_x(a, b, c) ((a) && (b) && (c))
#endif

  /*#if test_x(1, 1, 1)
    printf("blah\n");
  #endif

    __UINT32_C(102);*/

  return !tok2;
}
