#include "lib/windoze.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/strarray.h"
#include "lib/path.h"
#include "lib/path_internal.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/byte.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/open.h"
#include "lib/bool.h"
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
#include <io.h>
#endif

static stralloc cmd, realcmd, fullcmd, specs;
static const char* ext =
#if WINDOWS
    ".exe";
#else
    "";
#endif

static stralloc prog, base, progdir;

void
pathlist_get(strarray* list, const char* varname) {
  const char* p;

  strarray_init(list);

  if((p = env_get(varname))) {
    const char* e = p + str_len(p);
    char sep = ':';

    if(((p[0] != '/' && p[1] == ':') && p[byte_chr(p, e - p, ';')]) || varname[4] == 'E')
      sep = ';';

    while(p < e) {
      size_t i;
      i = byte_chr(p, e - p, sep);
      strarray_pushb(list, p, i);
      p += i + 1;
    }
  }
}

const char*
pathlist_lookup(const strarray* path, const strarray* pathext, const char* bin, stralloc* out) {
  char **dir_p, **ext_p;
  stralloc name;
  stralloc_init(&name);

  strarray_foreach(path, dir_p) {
    size_t len;
    bool exist;
    path_concat(*dir_p, bin, &name);
    len = name.len;

    if((exist = path_exists(name.s)))
      goto found;

    strarray_foreach(pathext, ext_p) {
      name.len = len;
      stralloc_cats(&name, *ext_p ? *ext_p : "");
      stralloc_nul(&name);

      exist = path_exists(name.s);

#ifdef DEBUG_OUTPUT
      buffer_putm_internal(buffer_2, "path_find(\"", *dir_p, "\", \"", name.s, "\", out) = ", exist ? "true" : "false", ";", NULL);
      buffer_putnlflush(buffer_2);
#endif

      if(exist)
        goto found;
    }
  }

  stralloc_free(&name);
  return NULL;

found:
  stralloc_copy(out, &name);
  stralloc_nul(out);
  stralloc_free(&name);
  return out->s;
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
/*
static int
path_lookup(const strarray* path, const strarray* pathext, const char* cmd,
stralloc* out) { char** ptr;

  strarray_foreach(path, ptr) {
    stralloc_copys(out, *ptr);
    stralloc_catc(out, PATHSEP_C);
    stralloc_cats(out, cmd);
    stralloc_nul(out);

    if(path_exists(out->s))
      return 1;
  }
  return 0;
}
*/
static stralloc
expand_env(const char* src) {
  size_t len = str_len(src);
  const char *s = src, *e = src + len;
  stralloc ret;
  stralloc_init(&ret);

  while(s < e) {
    size_t i, j;
    if((i = byte_chr(s, e - s, '%'))) {
      stralloc_catb(&ret, s, i);
      s += i;
      continue;
    }
    ++s;
    if((i = byte_chr(s, e - s, '%'))) {
      const char* value;
      if((value = env_get_b(s, i)))
        stralloc_cats(&ret, value);
      else
        stralloc_catb(&ret, s - 1, i + 2);
      s += i + 1;
    }
  }

  stralloc_nul(&ret);
  return ret;
}

static char*
search_path(stralloc* sa, const char* p) {
  stralloc_init(sa);
  stralloc_ready(sa, PATH_MAX);

  if(!path_is_absolute(p)) {
    strarray path, pathext;
    pathlist_get(&path, "PATH");
    pathlist_get(&pathext, "PATHEXT");

    if(strarray_size(&pathext) == 0)
      strarray_push(&pathext, "");

    pathlist_lookup(&path, &pathext, p, sa);
    strarray_free(&path);
    strarray_free(&pathext);
  } else {
    stralloc_copys(sa, p);
  }

  stralloc_nul(sa);
  return sa->s;
}

static const char*
get_prog_name(void) {
  ssize_t len;
  char* name;

  if((name = process_executable())) {
    prog.s = name;
    prog.len = str_len(name);
    return name;
  }

  return search_path(&prog, errmsg_argv0);
}

static int
write_log(const strarray* argv, const char* file) {
  stralloc sa;
  fd_type fd;

  if((fd = open_append(file)) != -1) {
    buffer b;
    buffer_write_fd(&b, fd);

    stralloc_init(&sa);
    strarray_joins(argv, &sa, " ");

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
  strarray args;
  char** av;
  int ret;
  const char* pathstr;
  const char* logfile = getenv("LOGFILE");
  buffer inifile;

  errmsg_iam(argv[0]);

  strarray_init(&args);

  get_prog_name();

  stralloc_copy(&progdir, &prog);
  path_dirname_sa(&progdir);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "errmsg_argv0: ");
  buffer_puts(buffer_2, argv[0]);
  buffer_puts(buffer_2, "\nprog: ");
  buffer_putsa(buffer_2, &prog);
  buffer_puts(buffer_2, "\nprogdir: ");
  buffer_putsa(buffer_2, &progdir);
  buffer_puts(buffer_2, "\next: ");
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
  {
    ini_section_t *ini = 0, *env = 0;
    const char *exec, *cwd, *argv0;

    ini_read(&inifile, &ini);

    if((cwd = ini_get(ini, "cwd"))) {
      if(chdir(cwd)) {
        errmsg_warnsys("failed to chdir(): ", cwd, 0);
        return 2;
      }
    }

    if((env = ini_section(ini, "env"))) {
      strarray keys = ini_keys(env);
      char** ptr;

      strarray_foreach(&keys, ptr) {
        const char* name = *ptr;
        const char* value = ini_get(env, *ptr);

        stralloc expanded = expand_env(value);

#ifdef DEBUG_OUTPUT
        buffer_putm_internal(buffer_2, "env_set(\"", name, "\", \"", expanded.s, "\"); (", value, ")", NULL);
        buffer_putnlflush(buffer_2);
#endif

        env_set(name, expanded.s);
        stralloc_free(&expanded);
      }
    }

    if((exec = ini_get(ini, "exec"))) {
      stralloc_copys(&realcmd, exec);
      stralloc_nul(&realcmd);

      if(!path_is_absolute(realcmd.s)) {
        stralloc tmp;
        stralloc_init(&tmp);
        path_concat_sa(&progdir, &realcmd, &tmp);

        if(str_len(PATHSEP_S_MIXED) > 1)
          stralloc_replacec(&tmp, PATHSEP_S_MIXED[1], PATHSEP_S_MIXED[0]);

        stralloc_nul(&tmp);
        if(path_exists(tmp.s)) {
          stralloc_free(&realcmd);
          stralloc_move(&realcmd, &tmp);
        }
      }

      if(realcmd.len == byte_chrs(realcmd.s, realcmd.len, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1)) {
        search_path(&fullcmd, realcmd.s);
      } else if(path_exists(realcmd.s)) {
        stralloc_copy(&fullcmd, &realcmd);
      }

      for(i = 1; i < argc; ++i) {
        strarray_push(&args, argv[i]);
      }

      if((argv0 = ini_get(ini, "name"))) {
        strarray_unshift(&args, argv0);
      } else {
        strarray_unshift(&args, path_basename(fullcmd.s));
      }
    }

    stralloc_init(&sa);
    strarray_joins(&args, &sa, "' '");
    stralloc_nul(&sa);

    if(!stralloc_endb(&fullcmd, EXEEXT, str_len(EXEEXT)))
      stralloc_cats(&fullcmd, EXEEXT);

    if(!path_exists(fullcmd.s)) {
      errmsg_warnsys("doesn't exist: ", fullcmd.s, " ('", sa.s, "''): ", 0);
      return 127;
    }

    if(logfile)
      write_log(&args, logfile);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "args: '");
    buffer_putstra(buffer_2, &args, "' '");
    buffer_puts(buffer_2, "'");
    buffer_putnlflush(buffer_2);
#endif

    stralloc_replacec(&fullcmd, PATHSEP_S_MIXED[0], '/');

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "fullcmd: ");
    buffer_putsa(buffer_2, &fullcmd);
    buffer_putnlflush(buffer_2);
#endif
    av = strarray_BEGIN(&args);
    if(-1 == (ret = process_create(fullcmd.s, av, NULL, NULL))) {
      errmsg_warnsys("process_create:", 0);
      return 1;
    }

    // ret = execve(realcmd.s, av, envp);
    {
      int status = process_wait(ret);
#ifdef DEBUG_OUTPUT
      buffer_puts(buffer_2, "status: ");
      buffer_putlong(buffer_2, status);
      buffer_putnlflush(buffer_2);
#endif

      if((status & 0xff) == 0)
        return status >> 8;
    }
    /*strarray_free(&args);*/
  }

  return 0;
}
