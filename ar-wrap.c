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
#include "lib/wait.h"

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
#include <errno.h>

static stralloc cmd, realcmd, fullcmd, specs;
static const char* ext = "";
static strlist path, pathext;
static stralloc prog, real, base, cwd;

void
debug_strarray(const char* name, strarray* stra) {
  stralloc sa;
  stralloc_init(&sa);
  strarray_joins(stra, &sa, ",\n  ");
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ":\n  ");
  buffer_putsa(buffer_2, &sa);
  buffer_putnlflush(buffer_2);
  stralloc_free(&sa);
}

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
    if((pos = byte_rchr(x, n, '/')) < n || (pos = byte_rchr(x, n, '\\')) < n)
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

void
pathlist_get(strlist* list, const char* varname) {
  const char* p;

  if((p = env_get(varname))) {
    strlist_init(list, '\0');
    strlist_froms(list, p, p[0] == '/' ? ':' : ';');
  }
}

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
      buffer_putm_internal(buffer_2, "path_find(\"", dir, "\", \"", name.s, "\", out);", 0);
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

const char*
get_prog_name(stralloc* prog) {
  ssize_t len;

  stralloc_ready(prog, PATH_MAX);

#if !WINDOWS_NATIVE
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

  if(!path_is_absolute(argv0)) {
    pathlist_get(&path, "PATH");
    pathlist_get(&pathext, "PATHEXT");

    if(strlist_count(&pathext) == 0)
      strlist_push(&pathext, "");

    pathlist_lookup(argv0, prog);
  } else {
    stralloc_copys(prog, argv0);
  }
  stralloc_nul(prog);
  return prog->s;
}

int
write_log(const strlist* argv, const char* file) {
  stralloc sa;
  fd_t fd;

  if((fd = open_append(file)) != -1) {
    buffer b;
    buffer_write_fd(&b, fd);

    stralloc_init(&sa);
    strlist_join(argv, &sa, ' ');

    buffer_putsa(&b, &sa);
    buffer_putnlflush(&b);
    buffer_close(&b);

    return 1;
  }
  return 0;
}

int
main(int argc, char* argv[]) {
  size_t p;
  int i, st;
  stralloc sa, lib, arg;
  strarray v;
  strlist opts, objs, dirs;
  char** av;
  int ret;
  const char* pathstr;
  const char* logfile = getenv("LOGFILE");
  static int tlib = 0;

  errmsg_iam(argv[0]);

  path_getcwd(&cwd);
  debug_sa("cwd", &cwd);

  strlist_init(&opts, '\0');
  strlist_init(&objs, '\0');
  strlist_init(&dirs, '\0');

  stralloc_init(&arg);
  stralloc_init(&lib);

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
  stralloc_lower(&base);

  if(stralloc_starts(&base, "tlib"))
    tlib = 1;

  debug_sa("base", &base);

  base_file(".real");
  if(!stralloc_endb(&real, ext, str_len(ext)))
    stralloc_cats(&real, ext);

  stralloc_copy(&realcmd, &real);
  stralloc_nul(&realcmd);

  if(!stralloc_contains(&realcmd, PATHSEP_S)) {
    path_lookup(realcmd.s, &fullcmd);
  } else if(path_exists(realcmd.s)) {
    stralloc_copy(&fullcmd, &realcmd);
  }

  if(path_exists(base_file(".specs"))) {
    stralloc_copys(&specs, "-specs=");
    stralloc_cat(&specs, &real);
    stralloc_nul(&specs);

    strlist_unshift(&opts, specs.s);
  }

  if(path_exists(base_file(".env"))) {
    size_t n;
    char* x;
    if((x = (char*)mmap_read(real.s, &n)))
      read_env(x, n);
    mmap_unmap(x, n);
  }

  strarray_init(&v);
  strarray_from_argv(argc, (const char* const*)argv, &v);

  for(i = 1; i < strarray_size(&v); ++i) {
    size_t pos;
    stralloc_copys(&arg, strarray_at(&v, i));
    stralloc_nul(&arg);

    if(i == 1 && !tlib) {
      strlist_push_sa(&opts, &arg);
      continue;
    }

    if(stralloc_starts(&arg, "@")) {
      long exist = 0;
      stralloc sa;
      stralloc_init(&sa);
      debug_sa("@", &arg);

      if(openreadclose(arg.s + 1, &sa, 4096) > 0) {
        stralloc_nul(&arg);
        char *it, *end;
        array a;
        array_init(&a);

        it = stralloc_begin(&sa);
        end = stralloc_end(&sa);

        exist = 1;

        while(it < end) {
          size_t n;
          it += scan_whitenskip(it, end - it);
          if(it == end)
            break;
          n = scan_nonwhitenskip(it, end - it);
          it = byte_trim(it, &n, "\"'", 2);

          it[n] = '\0';

          array_catb(&a, &it, sizeof(char*));
          it += n + 1;
        }

        strarray_splice(&v, i, 1, array_length(&a, sizeof(char*)), array_start(&a));

        // debug_strarray("new args", &v);

        --i;
        continue;
      }
    }

    pos = stralloc_findb(&arg, "/", 1);

#if WINDOWS_NATIVE
    if(pos > 0 && pos < arg.len)
      stralloc_replacec(&arg, '/', '\\');
#endif

    if(stralloc_endb(&arg, ".a", 2) || stralloc_endb(&arg, ".lib", 4)) {
      stralloc_copy(&lib, &arg);
    } else {
      int is_obj = stralloc_endb(&arg, ".o", 2) || stralloc_endb(&arg, ".obj", 4);

      if(is_obj) {
        stralloc dir;
        stralloc_init(&dir);
        path_dirname(arg.s, &dir);
        strlist_push_unique_sa(&dirs, &dir);
        stralloc_free(&dir);

        if(tlib)
          stralloc_prepends(&arg, "+-");
      }

      if(tlib) {
        if(stralloc_equals(&arg, "/a") || stralloc_equals(&arg, "/u"))
          continue;
      }

      strlist_push_sa(is_obj ? &objs : &opts, &arg);
    }
  }

  strlist_unshift(&opts, path_basename(realcmd.s));
  strlist_push_sa(&opts, &lib);
  strlist_cat(&opts, &objs);

  stralloc_init(&sa);
  // strlist_joins(&dirs, &sa, " , ");
  strarray_joins(&v, &sa, "'\n'");
  stralloc_nul(&sa);
  //

  if(!stralloc_endb(&realcmd, EXEEXT, str_len(EXEEXT)))
    stralloc_cats(&realcmd, EXEEXT);

  if(!path_exists(realcmd.s)) {
    errmsg_warnsys("doesn't exist: ", realcmd.s, " \"", sa.s, "\" : ", 0);
    //    return 127;
  }

#if DEBUG
//  buffer_puts(buffer_1, "dirs: ");
//  buffer_putsa(buffer_1, &sa);
//  buffer_putnlflush(buffer_1);
//  buffer_puts(buffer_1, "cmd: '");
//  buffer_putsa(buffer_1, &cmd);
//  buffer_puts(buffer_1, ext);
//  buffer_puts(buffer_1, "'");
//  buffer_putnlflush(buffer_1);
//  buffer_puts(buffer_1, "realcmd: '");
//  buffer_putsa(buffer_1, &realcmd);
//  buffer_puts(buffer_1, "'");
//  buffer_putnlflush(buffer_1);
#endif
#ifdef DEBUG
  buffer_puts(buffer_2, "cmd: '");
  buffer_putsa(buffer_2, &sa);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif
  if(logfile)
    write_log(&opts, logfile);

  av = strlist_to_argv(&opts);

  errno = 0;
  ret = process_create(realcmd.s, (const char**)av, 0, 0);
  // ret = execvp(realcmd.s, av);

  if(ret == -1) {
    errmsg_warnsys("process_create:", 0);
    return 1;
  }

  if((i = wait_pid(ret, &st)) != -1) {
    buffer_puts(buffer_2, "waitpid = ");
    buffer_putlong(buffer_2, i);
    buffer_puts(buffer_2, " status = ");
    buffer_putlong(buffer_2, st);
    buffer_puts(buffer_2, " ");
    buffer_flush(buffer_2);
    errmsg_warnsys("child terminated:", 0);
  }

  return 0;
}
