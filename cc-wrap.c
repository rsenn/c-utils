#include "lib/windoze.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/path_internal.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/byte.h"
#include "lib/mmap.h"
#include "lib/scan.h"

#if WINDOWS_NATIVE
#include <process.h>
#else
#include <unistd.h>
#endif

static stralloc cmd, realcmd, fullcmd, specs;
static const char* ext = "";
static strlist path;
static stralloc base;

char*
base_file(const char* suffix) {
  stralloc_zero(&base);
  stralloc_copys(&base, argv0);
  if(stralloc_endb(&base, ".exe", 4)) {
    base.len -= 4;
    ext = ".exe";
  }
  stralloc_cats(&base, suffix);
  stralloc_nul(&base);
  return base.s;
}

ssize_t
read_env(const char* x, size_t n) {
  char* line;
  size_t len, skip, num_read = 0;
  byte_foreach_skip(x, n, line, skip) {

    skip = scan_lineskip(line, n - (line - x));
    len = scan_line(line, skip);

    env_putb(line, len);

    buffer_puts(buffer_2, "Line: ");
    buffer_put(buffer_2, line, len);
    buffer_putnlflush(buffer_2);

    num_read += len;
  }

  return len;
}

int
path_lookup(const char* cmd, stralloc* out) {
  char* s;

  strlist_foreach_s(&path, s) {
    stralloc_copys(out, s);
    stralloc_catc(out, PATHSEP_C);
    stralloc_cats(out, cmd);
    stralloc_nul(out);

    if(path_exists(out->s))
      return 1;
  }
  return 0;
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
  const char* pathstr;

  errmsg_iam(argv[0]);

  strlist_init(&args, '\0');

  pathstr = env_get("PATH");

  strlist_init(&path, '\0');
  strlist_froms(&path, pathstr, PATHLISTSEP_C);

  base_file(".real");
  stralloc_cats(&base, ext);
  stralloc_copy(&realcmd, &base);
  stralloc_nul(&realcmd);

  if(!stralloc_contains(&realcmd, PATHSEP_S)) {
    path_lookup(realcmd.s, &fullcmd);
  } else if(path_exists(realcmd.s)) {
    stralloc_copy(&fullcmd, &realcmd);
  }

  if(path_exists(base_file(".specs"))) {
    stralloc_copys(&specs, "-specs=");
    stralloc_cat(&specs, &base);
    stralloc_nul(&specs);

    strlist_unshift(&args, specs.s);
  }

  if(path_exists(base_file(".env"))) {
    size_t n;
    char* x;
    if((x = mmap_read(base.s, &n)))
      read_env(x, n);
    mmap_unmap(x, n);
  }

  for(i = 1; i < argc; ++i) {
    strlist_push(&args, argv[i]);
  }

  strlist_unshift(&args, path_basename(realcmd.s));

  stralloc_init(&sa);
  strlist_joins(&args, &sa, "' '");
  stralloc_nul(&sa);
  // strarray_joins(&v, &sa, "'\n'");

  if(!path_exists(realcmd.s)) {
    errmsg_warnsys("doesn't exist: ", realcmd.s, " ('", sa.s, "''): ", 0);
    //    return 127;
  }

#if 0
  buffer_puts(buffer_1, "cmd: '");
  buffer_putsa(buffer_1, &cmd);
  buffer_puts(buffer_1, ext);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "realcmd: '");
  buffer_putsa(buffer_1, &realcmd);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
#endif
#ifdef DEBUG
  buffer_puts(buffer_2, "execvp: '");
  buffer_putsa(buffer_2, &sa);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  av = strlist_to_argv(&args);
  ret = execvp(realcmd.s, av);

  if(ret == -1) {
    errmsg_warnsys("execvp:", 0);
    return 1;
  }

  return 0;
}

