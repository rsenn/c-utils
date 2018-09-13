#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/strlist.h"
#include "lib/getopt.h"

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

  strlist_init(&path, '\0');
  strlist_from_path(&path, arg);

  stralloc_init(&sa);
  strlist_join(&path, &sa, separator[0]);

  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);
}

void
usage(const char* av0) {
  buffer_putm(buffer_1,
              "Usage: ",
              av0,
              " <path...>\n",
              "\n",
              "Options:\n",
              "\n",
              "  --help                     Show this help\n",
              "  -r DIR, --relative-to=DIR  Print the resolved path relative to DIR\n",
              "  -s SEP, --separator=SEP    Use SEP as directory separator\n",
              "  -w, --windows              Print Windows form of path(s) (C:\\WINNT)\n",
              "  -m, --mixed                Like --windows, but with regular slashes (C:/WINNT)\n",
              "  -u, --unix       (default) Print Unix form of path(s) (/cygdrive/c/winnt)\n",
              "\n");
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct option opts[] = {
    { "help", 0, NULL, 'h' },
    { "relative-to", 1, NULL, 'r' },
    { "separator", 1, NULL, 's' },
    { "mixed", 0, NULL, 'm' },
    { "unix", 0, NULL, 'u' },
    { "windows", 0, NULL, 'w' },
  };

  for(;;) {
    c = getopt_long(argc, argv, "hr:s:muw", opts, &index);
    if(c == -1)
      break;

    switch(c) {
    case 'h':
      usage(argv[0]);
      return 0;
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
    default:
      usage(argv[0]);
      return 1;
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
