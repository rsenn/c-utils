#include "lib/windoze.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/open.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/path.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/byte.h"
#include "lib/mmap.h"
#include "lib/scan.h"

#if WINDOWS
#define EXEEXT ".exe"
#else
#define EXEEXT ""
#endif

#if WINDOWS_NATIVE
#include <process.h>
#else
#include <unistd.h>
#endif

#if WINDOWS
#include <windows.h>
#endif

static stralloc cmd;
static strlist builddir, srcdir;
static const char* ext = "";
static strlist path, pathext;
static stralloc prog, real, base, cwd;

#ifdef DEBUG
/**
 * @brief debug_sa
 * @param name
 * @param sa
 */
void
debug_sa(const char* name, stralloc* sa) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
}

/**
 * @brief debug_s
 * @param name
 * @param s
 */
void
debug_s(const char* name, const char* s) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_puts(buffer_2, s);
  buffer_putnlflush(buffer_2);
}

/**
 * @brief debug_sl
 * @param name
 * @param l
 */
void
debug_sl(const char* name, const strlist* l) {
  size_t pos, n;
  const char* x;
  stralloc tmp;
  stralloc_init(&tmp);
  strlist_foreach(l, x, n) {
    if(tmp.len)
      stralloc_catc(&tmp, ' ');
    if((pos = byte_rchr(x, n, '/')) < n ||
       (pos = byte_rchr(x, n, '\\')) < n)
      stralloc_catb(&tmp, x + pos + 1, n - pos - 1);
    else
      stralloc_catb(&tmp, x, n);
  }
  // debug_sa(name, &tmp);
  stralloc_free(&tmp);
}

/**
 * @brief debug_int
 * @param name
 * @param i
 */
void
debug_int(const char* name, int i) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putlong(buffer_2, i);
  buffer_putnlflush(buffer_2);
}
#else
#define debug_sa(x, y)
#define debug_sl(x, y)
#define debug_s(x, y)
#define debug_int(x, y)
#endif

/**
 * @brief pathlist_get
 * @param list
 * @param varname
 */
void
pathlist_get(strlist* list, const char* varname) {
  const char* p;

  if((p = env_get(varname))) {
    strlist_init(list, '\0');
    strlist_froms(list, p, p[0] == '/' ? ':' : ';');
  }
}

/**
 * @brief pathlist_lookup
 * @param bin
 * @param out
 * @return
 */
const char*
pathlist_lookup(const char* bin, stralloc* out) {
  const char *dir, *ext;
  stralloc name;
  stralloc_init(&name);

  strlist_foreach_s(&path, dir) {
    strlist_foreach_s(&pathext, ext) {
      stralloc_copys(&name, bin);
      stralloc_cats(&name, ext);
      stralloc_nul(&name);

#ifdef DEBUG_OUTPUT
      buffer_putm_internal(buffer_2,
                           "path_find(\"",
                           dir,
                           "\", \"",
                           name.s,
                           "\", out);",
                           NULL);
      buffer_putnlflush(buffer_2);
#endif

      if(path_find(dir, name.s, out)) {
        stralloc_nul(out);
        stralloc_free(&name);
        return out->s;
      }
    }
  }
  stralloc_free(&name);
  return NULL;
}

/**
 * @brief base_file
 * @param suffix
 * @return
 */
char*
base_file(const char* suffix) {
  stralloc_zero(&real);
  stralloc_cat(&real, &prog);
  if(stralloc_endb(&real, EXEEXT, str_len(EXEEXT))) {
    real.len -= str_len(EXEEXT);
    ext = EXEEXT;
  }
  stralloc_cats(&real, suffix);
  stralloc_nul(&real);
  return real.s;
}

/**
 * @brief read_env
 * @param x
 * @param n
 * @return
 */
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

/**
 * @brief path_lookup
 * @param cmd
 * @param out
 * @return
 */
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

/**
 * @brief get_prog_name
 * @param prog
 * @return
 */
const char*
get_prog_name(stralloc* prog) {
  ssize_t len;

  stralloc_ready(prog, PATH_MAX);

#ifndef WINDOWS_NATIVE
  if((len = readlink("/proc/self/exe", prog->s, prog->a)) > 0) {
    prog->len = len;
    return prog->s;
  }
#endif

#if WINDOWS
  if((len = GetModuleFileNameA(0, prog->s, prog->a)) > 0) {
    prog->len = len;
    return prog->s;
  }
#endif

  if(!path_is_absolute(errmsg_argv0)) {
    pathlist_get(&path, "PATH");
    pathlist_get(&pathext, "PATHEXT");

    if(strlist_count(&pathext) == 0)
      strlist_push(&pathext, "");

    pathlist_lookup(errmsg_argv0, prog);
  } else {
    stralloc_copys(prog, errmsg_argv0);
  }
  stralloc_nul(prog);
  return prog->s;
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int
main(int argc, char* argv[], char* envp[]) {
  size_t p;
  int i;
  stralloc sa;
  strarray v;
  strlist args;
  char** av;
  int ret;
  const char* pathstr;

  errmsg_iam(argv[0]);

  path_getcwd(&cwd);
  debug_sa("cwd", &cwd);

  strlist_init(&args, '\0');

  get_prog_name(&prog);
  if(stralloc_endb(&prog, EXEEXT, str_len(EXEEXT)))
    prog.len -= str_len(EXEEXT);
  stralloc_nul(&prog);

#ifdef DEBUG
  buffer_puts(buffer_2, "argv0: ");
  buffer_puts(buffer_2, argv[0]);
  buffer_puts(buffer_2, ", prog: ");
  buffer_putsa(buffer_2, &prog);
  buffer_puts(buffer_2, ", ext: ");
  buffer_puts(buffer_2, ext);
  buffer_putnlflush(buffer_2);
#endif

  stralloc_copys(&base, path_basename(prog.s));

#if DEBUG
  buffer_puts(buffer_1, "cmd: '");
  buffer_putsa(buffer_1, &cmd);
  buffer_puts(buffer_1, ext);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
#endif
#ifdef DEBUG
  buffer_puts(buffer_2, "execve: '");
  buffer_putsa(buffer_2, &sa);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  av = strlist_to_argv(&args);
  ret = execve(cmd.s, av, envp);

  if(ret == -1) {
    errmsg_warnsys("execve:", 0);
    return 1;
  }

  return 0;
}
