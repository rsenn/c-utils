#include "lib/buffer.h"
#include "lib/getopt.h"
#include "lib/path.h"
#include "lib/rdir.h"
#include "lib/str.h"
#include "lib/strarray.h"

static strarray srcs;

char*
c_to_o(const char* in) {
  stralloc out;
  stralloc_init(&out);

  stralloc_copys(&out, str_basename(in));

  if(stralloc_endb(&out, ".c", 2))
    out.len -= 2;

  stralloc_cats(&out, ".o");

  stralloc_nul(&out);
  return out.s;
}

void
get_sources(const char* basedir, strarray* sources) {

  rdir_t rdir;
  if(!rdir_open(&rdir, basedir)) {
    const char* s;
    stralloc sa;
    stralloc_init(&sa);

    while((s = rdir_read(&rdir))) {
      stralloc_copys(&sa, s);

      if(stralloc_endb(&sa, ".c", 2)) strarray_push(sources, s);
    }

    stralloc_free(&sa);
  }
}

void
strarray_dump(buffer* b, const strarray* arr) {
  char **p = strarray_begin(arr), **e = strarray_end(arr);

  while(p < e) {

    buffer_puts(b, *p);
    buffer_putnlflush(b);
    ++p;
  }
}

void
usage(const char* argv0) {
  buffer_putm(buffer_1,
              "Usage: ",
              str_basename(argv0),
              " [sources...]\n",
              "\n",
              "Options\n",
              "  -h, --help                show this help\n");
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  stralloc sa;

  int argi = 1;
  int c;
  int index = 0;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
  };

  for(;;) {
    c = getopt_long(argc, argv, "h", opts, &index);
    if(c == -1) break;
    if(c == 0) continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      default: usage(argv[0]); return 1;
    }
  }

  strarray_init(&srcs);
  stralloc_init(&sa);

  while(optind < argc) {

    get_sources(argv[optind], &srcs);

    ++optind;
  }
  // strarray_dump(buffer_1, &srcs);

  strarray_transform(&srcs, c_to_o);
  /*strarray_removesuffixs(&srcs, ".c");
  strarray_appends(&srcs, ".o");*/

  strarray_joins(&srcs, &sa, " ");
  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);

  return 0;
}
