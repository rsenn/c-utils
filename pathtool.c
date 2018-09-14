#include "lib/buffer.h"
#include "lib/errmsg.h"
#include "lib/getopt.h"
#include "lib/path.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum {
  MIXED = 0,
  UNIX = 1,
  WINDOWS = 2,
} path_format;

static strlist relative_to;
static char separator[2];
static stralloc delims;
static path_format format = MIXED;
static int absolute = 0;
static stralloc cwd;

void
strlist_from_path(strlist* sl, const char* p) {
  strlist_zero(sl);
  // strlist_push(sl, "");
  strlist_push_tokens(sl, p, delims.s);
}

#if defined(__CYGWIN__) || defined(__MSYS__)
#include <sys/cygwin.h>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
void
pathconv(const char* path, stralloc* sa) {
  stralloc_ready(sa, MAX_PATH);

  switch(format) {
    case MIXED:
    case WINDOWS:
      cygwin_conv_to_win32_path(path, sa->s);
      sa->len = str_len(sa->s);
      break;
    case UNIX:
      cygwin_conv_to_posix_path(path, sa->s);
      sa->len = str_len(sa->s);
      break;
  }

  if(format == MIXED) {
    size_t i;
    for(i = 0; i < sa->len; ++i) {
      if(sa->s[i] == '\\') sa->s[i] = '/';
    }
  }
}
#endif

int
pathtool(const char* arg, stralloc* sa) {
  strlist path;

  stralloc_init(sa);

  if(absolute) {
    path_realpath(arg, sa, 1, &cwd);
  } else {
    stralloc_copys(sa, arg);
  }
  stralloc_nul(sa);

#if defined(__CYGWIN__) || defined(__MSYS__)
  {
    stralloc tmp;
    stralloc_init(&tmp);
    pathconv(sa->s, &tmp);
    stralloc_copy(sa, &tmp);
    stralloc_nul(sa);
    stralloc_free(&tmp);
  }
#endif

  strlist_init(&path, '\0');
  strlist_from_path(&path, sa->s);

  stralloc_zero(sa);

  if(relative_to.sa.s) {
    strlist rel;
    size_t n1 = strlist_count(&path);
    size_t n2 = strlist_count(&relative_to);
    size_t i, n = max(n1, n2);

    for(i = 0; i < n; ++i) {
      size_t l1, l2;
      char* s1 = strlist_at_n(&path, i, &l1);
      char* s2 = strlist_at_n(&relative_to, i, &l2);

#if 1
      buffer_puts(buffer_2, "REL ");
      buffer_put(buffer_2, s1, l1);
      buffer_puts(buffer_2, " ");
      buffer_put(buffer_2, s2, l2);
      buffer_putnlflush(buffer_2);
#endif

      if(l1 != l2) break;
      if(byte_diff(s1, l1, s2)) break;
    }

    strlist_init(&rel, '\0');

    while(n2-- > i) {
      strlist_push(&rel, "..");
    }
    while(i < n1) {
      char* s = strlist_at_n(&path, i, &n);
      strlist_pushb(&rel, s, n);
      ++i;
    }

    strlist_join(&rel, sa, separator[0]);

  } else {
    strlist_join(&path, sa, separator[0]);
  }

  return 1;
}

void
usage(char* av0) {
  buffer_putm(buffer_1,
              "Usage: ",
              str_basename(av0),
              " [OPTIONS] <path...>\n",
              "\n",
              "Options:\n",
              "\n",
              "  --help                 Show this help\n",
              "  -r, --relative-to DIR  Print the resolved path relative to DIR\n",
              "  -s, --separator SEP    Use SEP as directory separator\n",
              "  -w, --windows          Print Windows form of path(s) (C:\\WINNT)\n",
              "  -m, --mixed            Like --windows, but with regular slashes (C:/WINNT)\n",
              "  -u, --unix   (default) Print Unix form of path(s) (/cygdrive/c/winnt)\n",
              "  -a, --absolute         Output absolute path\n",
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
      {"help", 0, NULL, 'h'},
      {"relative-to", 1, NULL, 'r'},
      {"separator", 1, NULL, 's'},
      {"mixed", 0, NULL, 'm'},
      {"unix", 0, NULL, 'u'},
      {"windows", 0, NULL, 'w'},
      {"absolute", 0, NULL, 'a'},
  };

  for(;;) {
    c = getopt_long(argc, argv, "ahr:s:muw", opts, &index);
    if(c == -1) break;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'r': rel_to = optarg; break;
      case 's': separator[0] = optarg[0]; break;
      case 'm': format = MIXED; break;
      case 'u': format = UNIX; break;
      case 'w': format = WINDOWS; break;
      case 'a': absolute = 1; break;
      default: usage(argv[0]); return 1;
    }
  }

  path_getcwd(&cwd, 64);

  stralloc_catb(&delims, "/\\", 2);
  stralloc_catb(&delims, separator, 1);
  stralloc_nul(&delims);

  if(separator[0] == '\0') {
    switch(format) {
      case UNIX:
      case MIXED: separator[0] = '/'; break;
      case WINDOWS: separator[0] = '\\'; break;
    }
  }

  if(rel_to) {
    stralloc rel;

    stralloc_init(&rel);
    absolute = 1;

    if(pathtool(rel_to, &rel)) {
      stralloc_copy(&relative_to.sa, &rel);
      relative_to.sep = separator[0];
    } else {
      errmsg_warnsys(str_basename(argv[0]), ": relative to", NULL);
    }
  }

  while(optind < argc) {
    stralloc sa;
    stralloc_init(&sa);

    if(pathtool(argv[optind++], &sa)) {
      buffer_putsa(buffer_1, &sa);
      buffer_putnlflush(buffer_1);
    }
  }

  return 0;
}
