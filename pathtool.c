#include "lib/unix.h"
#include "lib/buffer.h"
#include "lib/errmsg.h"
#include "lib/path_internal.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/windoze.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/array.h"
#include "lib/bool.h"
#include "debug.h"
#include <string.h>

#define MAP_USE_HMAP 1
#include "lib/map.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
/*
typedef enum {
  MIXED, UNIX, WIN
} path_format;*/
typedef int path_format;
#define MIXED 0
#define UNIX 1
#define WIN 2

struct mount_entry {
  const char* device;
  const char* mountpoint;
};

static strlist relative_to;
static char separator[2];
static stralloc delims;
static path_format format;
static int absolute = 0, canonical = 0;
static stralloc cwd;
#if defined(__MINGW32__) || defined(__MSYS__)
static stralloc mingw, msys;
#endif
static MAP_T mtab;

#define tolower(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 0x20 : (c))
#define toupper(c) ((c) >= 'a' && (c) <= 'z' ? (c)-0x20 : (c))
#define toslash(c) ((c) == '\\' ? '/' : (c))

int
path_diffb(const char* s, size_t len, const char* t) {
  unsigned char x, y;

  while(len > 0) {
    --len;
    x = tolower(*s);
    x = toslash(x);
    y = tolower(*t);
    y = toslash(y);

    if(x != y)
      return ((int)(unsigned int)x) - ((int)(unsigned int)y);

    ++s;
    ++t;
  }
  return 0;
}

void
strlist_from_path(strlist* sl, const char* p) {
  char* dp = delims.s;

  if(*dp == sl->sep && delims.len > 1)
    ++dp;

  stralloc_copys(&sl->sa, p);
  /*strlist_zero(sl);
  strlist_froms(sl, p, *dp);*/
}

#if defined(__CYGWIN__) || defined(__MSYS__)
#include <sys/cygwin.h>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#if(!defined(__MSYS__) && !defined(HAVE_CYGWIN_CONV_PATH)) || (defined(__MSYS__) && defined(__x86_64__))
#define HAVE_CYGWIN_CONV_PATH 1
#endif

#ifdef HAVE_CYGWIN_CONV_PATH
#define cygwin_conv_to_win32_path(from, to) cygwin_conv_path(CCP_POSIX_TO_WIN_A, (from), (to), MAX_PATH)
#define cygwin_conv_to_posix_path(from, to) cygwin_conv_path(CCP_WIN_A_TO_POSIX, (from), (to), MAX_PATH)
#endif

void
pathconv(const char* path, stralloc* sa) {
  stralloc_ready(sa, MAX_PATH);

  switch(format) {
    case MIXED:
    case WIN:
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
      if(sa->s[i] == '\\')
        sa->s[i] = '/';
    }
  }
}
#endif

typedef struct {
  char* s;
  size_t n;
} blob_t;

static int
mounts_read(MAP_T map) {
  buffer in;
  stralloc line;
  int ret = 0;
  blob_t dev = {0, 0}, mnt = {0, 0};

  if(buffer_readfile(&in, "/proc/mounts"))
    return -1;

  stralloc_init(&line);

  while(buffer_getnewline_sa(&in, &line) > 0) {
    stralloc_nul(&line);
    dev.s = line.s;
    dev.n = str_find(dev.s, " /");
    dev.s[dev.n] = '\0';
    mnt.s = dev.s + dev.n + 1;

    while(*mnt.s && *mnt.s == ' ')
      ++mnt.s;

    mnt.n = str_chr(mnt.s, ' ');
    mnt.s[mnt.n] = '\0';

    if(str_start(mnt.s, "/cygdrive/") /*&& mnt.n == 10*/) {
      blob_t tmp = {mnt.s, mnt.n};
      mnt = dev;

      tmp.s[0] = (tmp.s[10]);
      tmp.s[1] = ':';
      tmp.s[2] = '\0';
      tmp.n = 2;

      dev = tmp;
    }

    MAP_INSERT(map, dev.s, dev.n + 1, mnt.s, mnt.n + 1);

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "mounts_read() device: ", dev.s ? dev.s : "(null)", " ", 0);
    buffer_putm_internal(buffer_2, "mountpoint: ", mnt.s ? mnt.s : "(null)", "\n", 0);
    buffer_flush(buffer_2);
#endif

    ++ret;
  }

  return ret;
}

typedef struct {
  const char* s;
  size_t n;
} column_t;

#define KEY(t) \
  (column_t) { MAP_ITER_KEY(t), MAP_ITER_KEY_LEN(t) - 1 }
#define VAL(t) \
  (column_t) { MAP_ITER_VALUE(t), MAP_ITER_VALUE_LEN(t) - 1 }

static const char*
mounts_match(MAP_T map, const char* path, size_t pathlen, size_t* matchlen, int col, bool first) {
  MAP_PAIR_T t;
  column_t cols[2], ret = {0, 0}, *search = &cols[!!col], *replacement = &cols[!col];

  MAP_FOREACH(map, t) {
    cols[0] = KEY(t);
    cols[1] = VAL(t);

    bool matched = search->n >= ret.n && search->n <= pathlen && !path_diffb(path, search->n, search->s) &&
                   (search->n == pathlen || (search->n < pathlen && path_issep(path[search->n])));

#ifdef DEBUG_OUTPUT
    buffer_putulong(buffer_2, matched);
    buffer_puts(buffer_2, " mounts_match(map, ");
    buffer_put(buffer_2, path, pathlen);
    buffer_puts(buffer_2, "\", ");

    buffer_putulong(buffer_2, col);
    buffer_puts(buffer_2, ", ");
    buffer_puts(buffer_2, first ? "true" : "false");
    buffer_puts(buffer_2, ") device: ");
    buffer_put(buffer_2, cols[0].s, cols[0].n);
    buffer_puts(buffer_2, " mountpoint: ");
    buffer_put(buffer_2, cols[1].s, cols[1].n);
    buffer_putnlflush(buffer_2);
#endif

    if(matched) {
      if(matchlen)
        *matchlen = search->n;
      ret.s = replacement->s;
      ret.n = search->n;

      if(first)
        break;
    }
  }
  return ret.s;
}

static const char*
mounts_replace(MAP_T map, stralloc* sa, int col, bool first) {
  const char* mount;
  size_t len;

#ifdef DEBUG_OUTPUT
  debug_sa("before replace", sa);
#endif

  if((mount = mounts_match(mtab, sa->s, sa->len, &len, col, first))) {
    size_t mountlen = str_len(mount);

#ifdef DEBUG_OUTPUT
    debug_str("found mount", mount);
#endif

    while(mountlen && path_issep(mount[mountlen - 1]))
      --mountlen;

    stralloc_replace(sa, 0, len, mount, mountlen);
    stralloc_nul(sa);

#ifdef DEBUG_OUTPUT
    debug_sa("after replace", sa);
#endif
  }
  return mount;
}

static void
mounts_add(MAP_T map, const char* dev, const char* mnt) {

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "mounts_add(map, \"");
  buffer_puts(buffer_2, dev);
  buffer_puts(buffer_2, "\",  \"");

  buffer_puts(buffer_2, mnt);
  buffer_puts(buffer_2, "\")");
  buffer_putnlflush(buffer_2);
#endif

  MAP_INSERT(map, dev, str_len(dev) + 1, (void*)mnt, str_len(mnt) + 1);
}

#if defined(__MINGW32__) || defined(__MSYS__)
static int
msys_root(stralloc* sa) {
  char buf[PATH_MAX + 1];
  int ret = 0;
  const char* s;
  stralloc_zero(sa);
#if defined(__MSYS__) || defined(__CYGWIN__)
  if(cygwin_conv_path(CCP_POSIX_TO_WIN_A, "/", buf, sizeof(buf)) == 0) {
    path_dirname(buf, sa);
    ret = 1;
  }
#endif
  if(!ret && (s = getenv("MSYS_PREFIX"))) {
    path_dirname(s, sa);
    ret = 1;
  }

#if defined(__MSYS__) || defined(__CYGWIN__)
  if(!ret && (s = getenv("MSYSTEM_PREFIX"))) {
    cygwin_conv_to_win32_path(s, buf);
    path_dirname(buf, sa);
    ret = 1;
  }
#endif
  if(ret)
    stralloc_replacec(sa, '\\', '/');
  return ret;
}

static int
mingw_prefix(stralloc* sa) {
  const char* s;
  stralloc root;
  stralloc_init(&root);
  msys_root(&root);

  stralloc_zero(sa);
  if((s = getenv("MINGW_PREFIX"))) {
    path_relative_to(s, root.s, sa);
    stralloc_inserts(sa, "/", 0);
    return 1;
  }
  return 0;
}
#endif

int
pathtool(const char* arg, stralloc* sa) {
  strlist path;
  size_t len;
  char* str = 0;

  stralloc_init(sa);

#if 1 // def DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, "pathtool(\"", arg, "\")", NULL);
  buffer_putnlflush(buffer_2);
#endif

  /*#if defined(__MINGW32__) || defined(__MSYS__)
    len = str_len(arg);
    if(len >= mingw.len && stralloc_equalb(&mingw, arg, mingw.len)) {
    }
  #endif*/

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "arg: ");
  buffer_puts(buffer_2, arg);
  buffer_putnlflush(buffer_2);
#endif

  if(absolute) {
    path_absolute(arg, sa);
    stralloc_nul(sa);
    path_canonical_sa(sa);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "absolute: ");
    buffer_putsa(buffer_2, sa);
    buffer_putnlflush(buffer_2);
#endif
  } else if(canonical) {
    path_canonicalize(arg, sa, 1);
#ifdef DEBUG_OUTPUT
    buffer_putsflush(buffer_2, "path_canonicalize");
#endif
  } else {
    stralloc_copys(sa, arg);
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, ": ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
#endif

  stralloc_nul(sa);

#if defined(__CYGWIN__) || defined(__MSYS__)
  if(path_is_absolute(sa->s)) {
    stralloc tmp;
    stralloc_init(&tmp);
    pathconv(sa->s, &tmp);
    mounts_replace(mtab, &tmp, 1, false);

    stralloc_copy(sa, &tmp);
    stralloc_nul(sa);
    stralloc_free(&tmp);
  }
#endif

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "sa: ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
#endif

#if defined(__MINGW32__) || defined(__MSYS__)
  mounts_replace(mtab, sa, format != UNIX, false);

#if defined(__MINGW32__)
  if(format == UNIX) {
    if(sa->len >= 2 && sa->s[1] == ':') {
      sa->s[1] = tolower(sa->s[0]);
      sa->s[0] = '/';
    }
  }
#endif
#endif

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "sa2: ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
#endif

  strlist_init(&path, relative_to.sa.s ? PATHSEP_C : separator[0]);
  strlist_from_path(&path, sa->s);

  stralloc_zero(sa);

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, "strlist path = ", 0);
  buffer_putsl(buffer_2, &path, " / ");
  buffer_putnlflush(buffer_2);
#endif

  if(relative_to.sa.s) {

    stralloc_nul(&path.sa);
    stralloc_nul(&relative_to.sa);

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "relative(\"", path.sa.s, "\", \"", relative_to.sa.s, "\")", NULL);
    buffer_putsa(buffer_2, sa);
    buffer_putnlflush(buffer_2);
#endif

    path_relative_to_b(path.sa.s, path.sa.len, relative_to.sa.s, relative_to.sa.len, sa);

  } else {

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "PATHSEP_S_MIXED: ");
    buffer_puts(buffer_2, PATHSEP_S_MIXED);
    buffer_putnlflush(buffer_2);
#endif
    strlist_join(&path, sa, separator[0]);
  }

  stralloc_nul(sa);

  if(str)
    free(str);

  return 1;
}

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] <path...>\n",
                       "\n",
                       "Options:\n",
                       "\n",
                       "  -h, --help             Show this help\n",
                       "  -r, --relative-to DIR  Print the resolved path relative to DIR\n",
                       "  -s, --separator SEP    Use SEP as directory separator\n",
                       "  -w, --windows          Print Windows form of path(s) (C:\\WINNT)\n",
                       "  -m, --mixed            Like --windows, but with regular slashes (C:/WINNT)\n",
                       "  -u, --unix   (default) Print Unix form of path(s) (/cygdrive/c/winnt)\n",
                       "  -a, --absolute         Output absolute path\n",
                       "  -f, --canonicalize     Canonicalize by following every symlink in\n"
                       "                         every component of the given name recursively;\n"
                       "                         all but the last component must exist\n",
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {
  stralloc tmp;
  int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'},
                                {"relative-to", 1, NULL, 'r'},
                                {"separator", 1, NULL, 's'},
                                {"mixed", 0, NULL, 'm'},
                                {"unix", 0, NULL, 'u'},
                                {"windows", 0, NULL, 'w'},
                                {"absolute", 0, NULL, 'a'},
                                {"canonicalize", 0, NULL, 'f'},
                                {0, 0, 0, 0}};

  MAP_NEW(mtab);

  errmsg_iam(argv[0]);
  strlist_init(&relative_to, PATHSEP_C);

  /*#if WINDOWS_NATIVE
    format = WIN;
  #elif defined(WINDOWS) && !(defined(__CYGWIN__) || defined(__MSYS__))
    format = MIXED;
  #else*/
  format = UNIX;
  /*#endif*/

  for(;;) {
    c = unix_getopt_long(argc, argv, "afhr:s:muw", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'r': rel_to = unix_optarg; break;
      case 's': separator[0] = unix_optarg[0]; break;
      case 'm': format = MIXED; break;
      case 'u': format = UNIX; break;
      case 'w': format = WIN; break;
      case 'a': absolute = 1; break;
      case 'f': canonical = 1; break;
      default: usage(argv[0]); return 1;
    }
  }

  stralloc_init(&tmp);
  path_getcwd(&tmp);
  stralloc_nul(&tmp);

#if defined(__CYGWIN__) || defined(__MSYS__)
  pathconv(tmp.s, &cwd);
#else
  stralloc_copy(&cwd, &tmp);
#endif

  if(separator[0] == '\0') {
    switch(format) {
      case UNIX:
      case MIXED: separator[0] = '/'; break;
      case WIN: separator[0] = '\\'; break;
    }
  }

  stralloc_catb(&delims, "/\\", 2);
  stralloc_catb(&delims, separator, 1);
  stralloc_nul(&delims);

#if 1 // def DEBUG_OUTPUT
  buffer_puts(buffer_2, "format: ");
  buffer_puts(buffer_2, ((const char*[]){"MIXED", "UNIX", "WIN"})[format]);
  buffer_putnlflush(buffer_2);
#endif

  mounts_read(mtab);

#if defined(__MINGW32__) || defined(__MSYS__)
  mingw_prefix(&mingw);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "mingw prefix: ");
  buffer_putsa(buffer_2, &mingw);
  buffer_putnlflush(buffer_2);
#endif

  msys_root(&msys);
  if(!mounts_replace(mtab, &msys, 1, false)) {
  }

  // mounts_add(mtab, "/", msys.s);
  mounts_add(mtab, msys.s, "/");

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "msys root: ");
  buffer_putsa(buffer_2, &msys);
  buffer_putnlflush(buffer_2);
#endif
#endif

  if(rel_to) {

    stralloc_copys(&relative_to.sa, rel_to);

    //    char tmpsep = separator[0];
    //    stralloc rel;

    //    stralloc_init(&rel);
    //    absolute = 1;

    //    separator[0] = PATHSEP_C;

    //    if(pathtool(rel_to, &rel)) {
    //      stralloc_copy(&relative_to.sa, &rel);
    //      relative_to.sep = separator[0];
    //    } else {
    //      errmsg_warnsys(str_basename(argv[0]), ": relative to", NULL);
    //    }

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "relative-to: ");
    buffer_putsa(buffer_2, &relative_to.sa);
    buffer_putnlflush(buffer_2);
#endif
    // separator[0] = tmpsep;
  }

  while(unix_optind < argc) {
    stralloc sa;
    stralloc_init(&sa);

    if(pathtool(argv[unix_optind++], &sa)) {

      if(format == WIN)
        stralloc_replacec(&sa, '/', '\\');
      else
        stralloc_replacec(&sa, '\\', '/');

      buffer_putsa(buffer_1, &sa);
      buffer_putnlflush(buffer_1);
    }
  }

  return 0;
}
