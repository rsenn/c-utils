#include "buffer.h"
#include "strlist.h"
#include <getopt.h>
#include <stdlib.h>

typedef enum {
  MIXED = 0,
  UNIX = 1,
  WINDOWS = 2,
} path_format;

static strlist relative_to;
static char separator[2];
static stralloc delims;
static path_format format = MIXED;

void
strlist_from_path(strlist* sl, const char* p) {
  strlist_zero(sl);
  strlist_push(sl, "");
  strlist_push_tokens(sl, p, delims.s);
}

void
pathtool(const char* arg) {
  stralloc sa;
  strlist path;

  strlist_init(&path);
  strlist_from_path(&path, arg);

  stralloc_init(&sa);
  strlist_join(&path, &sa, separator[0]);

  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct option opts[] = {
    { "relative-to", 1, NULL, 'r' },
    { "separator", 1, NULL, 's' },
    { "mixed", 0, NULL, 'm' },
    { "unix", 0, NULL, 'u' },
    { "windows", 0, NULL, 'w' },
  };

  for(;;) {
    c = getopt_long(argc, argv, "r:s:muw", opts, &index);
    if(c == -1)
      break;

    switch(c) {
    case 'r':
      rel_to = optarg;
      break;
    case 's':
      separator[0] = optarg[0];
      break;
    case 'm':
      format = MIXED;
      break;
    case 'u':
      format = UNIX;
      break;
    case 'w':
      format = WINDOWS;
      break;
    }
  }

  stralloc_catb(&delims, "/\\", 2);
  stralloc_catb(&delims, separator, 1);
  stralloc_nul(&delims);

  if(separator[0] == '\0') {
    switch(format) {
    case UNIX:
    case MIXED:
      separator[0] = '/';
      break;
    case WINDOWS:
      separator[0] = '\\';
      break;
    }
  }

  if(rel_to) {
    strlist_from_path(&relative_to, rel_to);
  }

  while(optind < argc) {
    pathtool(argv[optind++]);
  }

  {
    stralloc test = { 0,0,0 };
    stralloc out = { 0,0,0 };
    stralloc_copys(&test, "NumberOfSections");
    stralloc_decamelize(&test, &out);

    buffer_putsa(buffer_1, &out);
    buffer_putnlflush(buffer_1);
  }


  return 0;
}
