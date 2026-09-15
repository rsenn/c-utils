/*
 * exec-intercept.c -- LD_PRELOAD shim that redirects exec*() calls by
 * program basename, e.g. "bash" -> "/usr/local/bin/shish". Built for
 * destructive compatibility testing: many tools and scripts silently
 * assume real bash and lean on bashisms. Preloading this over a stricter
 * POSIX shell makes every such assumption fail loudly at the exact
 * command that isn't portable, instead of staying silently bash-locked.
 *
 * glibc's execv/execl/execvp/... call their own internal aliases of
 * execve(), not the exported symbol -- overriding only execve() here
 * would NOT catch calls made through those other entry points. Each one
 * is therefore overridden individually and funnelled through the same
 * remap+libc_execve path.
 *
 * fexecve() only gets an fd, no path -- but /proc/self/fd/<fd> still
 * resolves to whatever path was open()'d, so no separate open() hook is
 * needed to catch the common open()-then-fexecve() pattern.
 *
 * posix_spawn(p)() bypass execve() internally via a clone() trampoline on
 * modern glibc, so hooking execve() would miss them -- but posix_spawn(p)
 * itself is the public, interposable entry point applications call, so
 * overriding it directly and delegating to the real libc implementation
 * (with the path/file argument swapped) works without reimplementing any
 * spawn logic.
 *
 * system()/popen() always run the command through /bin/sh -- they don't
 * take a shell path, so there's nothing to swap the way remap()+libc_execve
 * works for the exec*()/posix_spawn(p)() family above. Instead, whenever a
 * remap entry matches "sh" (either key "sh" or "/bin/sh" -- lookup()
 * basenames the map's own "from" field, so both spellings resolve the same
 * way), these are reimplemented from scratch: fork() + execve() the target
 * shell with "-c" and the command, replacing what would have been an
 * internal /bin/sh -c invocation. pclose() has to know which FILE* streams
 * came out of this reimplemented popen() (they carry a pid libc's own
 * pclose() never learned about), so successful streams are recorded in
 * `popens` and pclose() checks there before falling back to libc.
 */
#define _GNU_SOURCE 1

#include <dlfcn.h>
#include <limits.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "lib/buffer.h"
#include "lib/cas.h"
#include "lib/env.h"
#include "lib/fmt.h"
#include "lib/open.h"
#include "lib/str.h"
#include "lib/util.h"

typedef int execve_function(const char*, char* const[], char* const[]);
typedef int execvp_function(const char*, char* const[]);
typedef int fexecve_function(int, char* const[], char* const[]);
typedef int posix_spawn_function(
    pid_t*, const char*, const posix_spawn_file_actions_t*, const posix_spawnattr_t*, char* const[], char* const[]);
typedef int system_function(const char*);
typedef FILE* popen_function(const char*, const char*);
typedef int pclose_function(FILE*);

static execve_function* libc_execve;
static execvp_function* libc_execvp;
static fexecve_function* libc_fexecve;
static posix_spawn_function* libc_posix_spawn;
static posix_spawn_function* libc_posix_spawnp;
static system_function* libc_system;
static popen_function* libc_popen;
static pclose_function* libc_pclose;
static long initialized;

typedef struct {
  const char* from;
  const char* to;
} remap_t;

static remap_t remaps[16];
static int nremaps;

typedef struct {
  FILE* fp;
  pid_t pid;
} popen_entry_t;

static popen_entry_t popens[16];
static int npopens;

static buffer o;
static char procname[64];
static const char* procbase = "";

static void
init(void) {
  if(__CAS(&initialized, 0, 1))
    return;

  libc_execve = dlsym(RTLD_NEXT, "execve");
  libc_execvp = dlsym(RTLD_NEXT, "execvp");
  libc_fexecve = dlsym(RTLD_NEXT, "fexecve");
  libc_posix_spawn = dlsym(RTLD_NEXT, "posix_spawn");
  libc_posix_spawnp = dlsym(RTLD_NEXT, "posix_spawnp");
  libc_system = dlsym(RTLD_NEXT, "system");
  libc_popen = dlsym(RTLD_NEXT, "popen");
  libc_pclose = dlsym(RTLD_NEXT, "pclose");

  const char* map = env_get("EXEC_INTERCEPT_MAP");

  if(!map)
    map = "bash=/usr/local/bin/shish";

  /* intentional leak: lives for the process lifetime, freed by exit() */
  char* copy = str_dup(map);
  char* save = NULL;

  for(char* tok = str_tok(copy, ",", &save); tok && nremaps < (int)countof(remaps); tok = str_tok(NULL, ",", &save)) {
    size_t eq = str_chr(tok, '=');

    /* no "=" at all (or nothing after it, e.g. "bash=") means log-only:
       record the call but let it run unredirected */
    if(tok[eq] == '=') {
      tok[eq] = 0;
      remaps[nremaps].from = tok;
      remaps[nremaps].to = tok + eq + 1;
    } else {
      remaps[nremaps].from = tok;
      remaps[nremaps].to = NULL;
    }

    nremaps++;
  }

  {
    const char* file = env_get("EXEC_INTERCEPT_LOG");
    int append = 0;
    int fd;

    if(file && file[0] == '+') {
      ++file;
      append = 1;
    }

    if((fd = (append ? open_append : open_trunc)(file && file[0] ? file : "exec-intercept.log")) == -1)
      fd = STDERR_FILENO;

    buffer_write_fd(&o, fd);
  }

  {
    int fd;
    ssize_t n = 0;

    if((fd = open_read("/proc/self/cmdline")) != -1) {
      n = read(fd, procname, sizeof(procname) - 1);
      close(fd);
    }

    /* /proc/self/cmdline is NUL-separated -- procname already reads as a
     * clean C string up to argv[0]'s own terminator regardless of what
     * follows in the buffer, so no extra scanning is needed here */
    procname[n < 0 ? 0 : n] = 0;
    procbase = str_basename(procname);
  }
}

static const remap_t*
lookup(const char* base) {
  /* basename the map's own "from" too, so a key given as a full path (e.g.
   * "/bin/sh=...") matches the same way a bare basename key ("sh=...")
   * would */
  for(int i = 0; i < nremaps; i++)
    if(str_equal(base, str_basename(remaps[i].from)))
      return &remaps[i];

  return NULL;
}

static const remap_t*
remap(const char* path) {
  char target[PATH_MAX];
  ssize_t n;
  const remap_t* r;

  if((r = lookup(str_basename(path))))
    return r;

  /* path may be a symlink whose target is what actually matters, e.g.
   * /bin/sh -> bash on many distros: the caller invokes "sh", but the
   * binary that would run is bash */
  if((n = readlink(path, target, sizeof(target) - 1)) > 0) {
    target[n] = 0;
    return lookup(str_basename(target));
  }

  return NULL;
}

static void
log_prefix(void) {
  char ts[32];

  buffer_put(&o, ts, fmt_iso8601(ts, time(NULL)));
  buffer_puts(&o, " [");
  buffer_puts(&o, procbase);
  buffer_puts(&o, ":");
  buffer_putlong(&o, (signed long)getpid());
  buffer_puts(&o, "] ");
}

/* emits s as a double-quoted string, escaping backslash/quote/newline/CR so
 * a multi-line script still logs as exactly one line */
static void
log_quoted(const char* s) {
  buffer_puts(&o, "\"");

  for(const char* p = s; *p; p++) {
    switch(*p) {
      case '\n': buffer_puts(&o, "\\n"); break;
      case '\r': buffer_puts(&o, "\\r"); break;
      case '\\': buffer_put(&o, "\\\\", 2); break;
      case '"': buffer_put(&o, "\\\"", 2); break;
      default: buffer_put(&o, p, 1); break;
    }
  }

  buffer_puts(&o, "\"");
}

static void
log_shell(const char* fn, const char* command, const char* shell) {
  log_prefix();
  buffer_puts(&o, fn);
  buffer_puts(&o, " ");
  log_quoted(command);

  if(shell && *shell) {
    buffer_puts(&o, " -> ");
    buffer_puts(&o, shell);
  } else {
    buffer_puts(&o, " (logged only, not redirected)");
  }

  buffer_putnlflush(&o);
}

static void
log_remap(const char* orig, const char* to, char* const argv[]) {
  log_prefix();
  buffer_puts(&o, orig);

  if(to && *to) {
    buffer_puts(&o, " -> ");
    buffer_puts(&o, to);
  } else {
    buffer_puts(&o, " (logged only, not redirected)");
  }

  if(argv) {
    buffer_puts(&o, " argv:");

    for(int i = 0; argv[i]; i++) {
      buffer_puts(&o, " ");
      buffer_puts(&o, argv[i]);
    }
  }

  buffer_putnlflush(&o);
}

VISIBLE int
execve(const char* path, char* const argv[], char* const envp[]) {
  const remap_t* r;

  init();

  if((r = remap(path))) {
    log_remap(path, r->to, argv);

    if(r->to && *r->to)
      return libc_execve(r->to, argv, envp);
  }

  return libc_execve(path, argv, envp);
}

VISIBLE int
execv(const char* path, char* const argv[]) {
  return execve(path, argv, environ);
}

VISIBLE int
execvp(const char* file, char* const argv[]) {
  const remap_t* r;

  init();

  if((r = remap(file))) {
    log_remap(file, r->to, argv);

    if(r->to && *r->to)
      return libc_execve(r->to, argv, environ);
  }

  return libc_execvp(file, argv);
}

VISIBLE int
execl(const char* path, const char* argv0, ...) {
  va_list ap;
  int argc = 1;

  va_start(ap, argv0);
  while(va_arg(ap, const char*))
    argc++;
  va_end(ap);

  char* argv[argc + 1];

  va_start(ap, argv0);
  argv[0] = (char*)argv0;

  for(int i = 1; i < argc; i++)
    argv[i] = va_arg(ap, char*);

  argv[argc] = NULL;
  va_end(ap);

  return execv(path, argv);
}

VISIBLE int
execlp(const char* file, const char* argv0, ...) {
  va_list ap;
  int argc = 1;

  va_start(ap, argv0);
  while(va_arg(ap, const char*))
    argc++;
  va_end(ap);

  char* argv[argc + 1];

  va_start(ap, argv0);
  argv[0] = (char*)argv0;

  for(int i = 1; i < argc; i++)
    argv[i] = va_arg(ap, char*);

  argv[argc] = NULL;
  va_end(ap);

  return execvp(file, argv);
}

VISIBLE int
execle(const char* path, const char* argv0, ...) {
  va_list ap;
  int argc = 1;

  va_start(ap, argv0);
  while(va_arg(ap, const char*))
    argc++;
  va_end(ap);

  char* argv[argc + 1];
  char** envp;

  va_start(ap, argv0);
  argv[0] = (char*)argv0;

  for(int i = 1; i < argc; i++)
    argv[i] = va_arg(ap, char*);

  argv[argc] = NULL;
  (void)va_arg(ap, char*); /* consume the (char*)NULL sentinel recorded above */
  envp = va_arg(ap, char**);
  va_end(ap);

  return execve(path, argv, envp);
}

VISIBLE int
fexecve(int fd, char* const argv[], char* const envp[]) {
  char linkpath[32], path[PATH_MAX];
  ssize_t n;
  const remap_t* r;
  size_t p;

  init();

  p = str_copy(linkpath, "/proc/self/fd/");
  p += fmt_ulong(linkpath + p, (unsigned long)fd);
  linkpath[p] = 0;

  if((n = readlink(linkpath, path, sizeof(path) - 1)) > 0) {
    path[n] = 0;

    if((r = remap(path))) {
      log_remap(path, r->to, argv);

      if(r->to && *r->to)
        return libc_execve(r->to, argv, envp);
    }
  }

  return libc_fexecve(fd, argv, envp);
}

VISIBLE int
posix_spawn(pid_t* pid,
            const char* path,
            const posix_spawn_file_actions_t* file_actions,
            const posix_spawnattr_t* attrp,
            char* const argv[],
            char* const envp[]) {
  const remap_t* r;
  const char* to = path;

  init();

  if((r = remap(path))) {
    log_remap(path, r->to, argv);

    if(r->to && *r->to)
      to = r->to;
  }

  return libc_posix_spawn(pid, to, file_actions, attrp, argv, envp);
}

VISIBLE int
posix_spawnp(pid_t* pid,
             const char* file,
             const posix_spawn_file_actions_t* file_actions,
             const posix_spawnattr_t* attrp,
             char* const argv[],
             char* const envp[]) {
  const remap_t* r;

  init();

  if((r = remap(file))) {
    log_remap(file, r->to, argv);

    /* remap target is an absolute path -- go through posix_spawn (path
       form) instead of posix_spawnp (which would PATH-search it again) */
    if(r->to && *r->to)
      return libc_posix_spawn(pid, r->to, file_actions, attrp, argv, envp);
  }

  return libc_posix_spawnp(pid, file, file_actions, attrp, argv, envp);
}

VISIBLE int
system(const char* command) {
  const remap_t* r;
  pid_t pid;
  int status;

  init();

  if(!command)
    return libc_system(command);

  if((r = remap("/bin/sh"))) {
    log_shell("system", command, r->to);

    if(r->to && *r->to) {
      char* const argv[] = {(char*)r->to, "-c", (char*)command, NULL};

      if((pid = fork()) == 0) {
        libc_execve(r->to, argv, environ);
        _exit(127);
      }

      if(pid < 0)
        return -1;

      if(waitpid(pid, &status, 0) < 0)
        return -1;

      return status;
    }
  }

  return libc_system(command);
}

VISIBLE FILE*
popen(const char* command, const char* type) {
  const remap_t* r;
  int pipefd[2];
  pid_t pid;
  FILE* fp;
  int read_mode;

  init();

  if(!command || !type || !(r = remap("/bin/sh")))
    return libc_popen(command, type);

  log_shell("popen", command, r->to);

  if(!(r->to && *r->to))
    return libc_popen(command, type);

  read_mode = type[0] == 'r';

  if(pipe(pipefd) == -1)
    return NULL;

  if((pid = fork()) == 0) {
    char* const argv[] = {(char*)r->to, "-c", (char*)command, NULL};

    if(read_mode) {
      close(pipefd[0]);
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[1]);
    } else {
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);
    }

    libc_execve(r->to, argv, environ);
    _exit(127);
  }

  if(pid < 0) {
    close(pipefd[0]);
    close(pipefd[1]);
    return NULL;
  }

  if(read_mode) {
    close(pipefd[1]);
    fp = fdopen(pipefd[0], type);
  } else {
    close(pipefd[0]);
    fp = fdopen(pipefd[1], type);
  }

  if(!fp) {
    close(read_mode ? pipefd[0] : pipefd[1]);
    return NULL;
  }

  if(npopens < (int)countof(popens)) {
    popens[npopens].fp = fp;
    popens[npopens].pid = pid;
    npopens++;
  }

  return fp;
}

VISIBLE int
pclose(FILE* fp) {
  int status;

  for(int i = 0; i < npopens; i++) {
    if(popens[i].fp == fp) {
      pid_t pid = popens[i].pid;

      popens[i] = popens[--npopens];
      fclose(fp);

      if(waitpid(pid, &status, 0) < 0)
        return -1;

      return status;
    }
  }

  return libc_pclose(fp);
}
