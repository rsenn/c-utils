#include "lib/windoze.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/path.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/byte.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/open.h"
#include "ini.h"

#if WINDOWS_NATIVE
#define EXEEXT ".exe"
#include <process.h>
#else
#define EXEEXT ""
#include <unistd.h>
#endif

#if WINDOWS
#include <windows.h>
#endif

static stralloc cmd, realcmd, fullcmd, specs;
static const char* ext =
#if WINDOWS
    ".exe";
#else
    "";
#endif
static strlist path, pathext;
static stralloc prog, base;

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
      buffer_putm_internal(buffer_2, "path_find(\"", dir, "\", \"", name.s, "\", out);", NULL);
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
  stralloc_zero(&base);
  stralloc_cat(&base, &prog);
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

const char*
get_prog_name(void) {
  ssize_t len;
  char* name;

  if((name = process_executable())) {
    prog.s = name;
    prog.len = str_len(name);
    return name;
  }

  stralloc_ready(&prog, PATH_MAX);

  if(!path_is_absolute(errmsg_argv0)) {
    pathlist_get(&path, "PATH");
    pathlist_get(&pathext, "PATHEXT");

    if(strlist_count(&pathext) == 0)
      strlist_push(&pathext, "");

    pathlist_lookup(errmsg_argv0, &prog);
  } else {
    stralloc_copys(&prog, errmsg_argv0);
  }
  stralloc_nul(&prog);
  return prog.s;
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
main(int argc, char* argv[], char* envp[]) {
  size_t p;
  int i;
  stralloc sa;
  strarray v;
  strlist args;
  char** av;
  int ret;
  const char* pathstr;
  const char* logfile = getenv("LOGFILE");
  buffer inifile;

  errmsg_iam(argv[0]);

  strlist_init(&args, '\0');

  get_prog_name();

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "errmsg_argv0: ");
  buffer_puts(buffer_2, argv[0]);
  buffer_puts(buffer_2, ", prog: ");
  buffer_putsa(buffer_2, &prog);
  buffer_puts(buffer_2, ", ext: ");
  buffer_puts(buffer_2, ext);
  buffer_putnlflush(buffer_2);
#endif

  base_file(".ini");

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "base: ");
  buffer_putsa(buffer_2, &base);

  buffer_putnlflush(buffer_2);
#endif

  buffer_mmapread(&inifile, base.s);
  ini_section_t* ini = 0;

  ini_read(&inifile, &ini);

  const char *exec, *cwd;

  if((cwd = ini_get(ini, "cwd"))) {
    if(chdir(cwd)) {
      errmsg_warnsys("failed to chdir(): ", cwd, 0);
      return 2;
    }
  }

  if((exec = ini_get(ini, "exec"))) {
    stralloc_copys(&realcmd, exec);
    stralloc_nul(&realcmd);

    if(!stralloc_contains(&realcmd, PATHSEP_S)) {
      path_lookup(realcmd.s, &fullcmd);
    } else if(path_exists(realcmd.s)) {
      stralloc_copy(&fullcmd, &realcmd);
    }

    for(i = 1; i < argc; ++i) {
      strlist_push(&args, argv[i]);
    }

    strlist_unshift(&args, path_basename(realcmd.s));

    stralloc_init(&sa);
    strlist_joins(&args, &sa, "' '");
    stralloc_nul(&sa);

    if(!stralloc_endb(&realcmd, EXEEXT, str_len(EXEEXT)))
      stralloc_cats(&realcmd, EXEEXT);

    if(!path_exists(realcmd.s)) {
      errmsg_warnsys("doesn't exist: ", realcmd.s, " ('", sa.s, "''): ", 0);
    }

    if(logfile)
      write_log(&args, logfile);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "execve: '");
    buffer_putsa(buffer_2, &sa);
    buffer_puts(buffer_2, "'");
    buffer_putnlflush(buffer_2);
#endif

    av = strlist_to_argv(&args);
    ret = execve(realcmd.s, av, envp);

    if(ret == -1) {
      errmsg_warnsys("execve:", 0);
      return 1;
    }
  }

  return 0;
}
