#include "lib/unix.h"
#include "lib/algorithm.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/dir.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/iterator.h"
#include "lib/path.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/wordexp.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/ndelay.h"
#include "lib/mmap.h"
#include "lib/array.h"
#include "lib/socket.h"
#include "lib/open.h"
#include "lib/wait.h"
#include "map.h"

#include <ctype.h>
#include <spawn.h>
#include <stdlib.h>

#define PKGCFG_EXISTS 1
#define PKGCFG_PRINT_ERR 2
#define PKGCFG_SHORT_ERR 4

typedef enum {
  PRINT_VERSION = 1,
  PRINT_CFLAGS = 2,
  PRINT_LIBS = 4,
  PRINT_REQUIRES = 8,
  PRINT_PATH = 16,
  LIST_ALL = 32,
  STATIC_LIBS = 64
} id;

typedef enum { LIBS_ONLY_L = 128, LIBS_ONLY_OTHER = 256 } libs_mode_t;

typedef enum { CFLAGS_ONLY_I = 512, CFLAGS_ONLY_OTHER = 1024 } cflags_mode_t;

static struct {
  id code;
  strlist path;
  stralloc self;
  stralloc host;
  stralloc prefix;
} cmd;

typedef struct pkg_s {
  stralloc name;
  MAP_T vars;
  MAP_T fields;
} pkg;

static const char* const field_names[] = {
    "Version",
    "Cflags",
    "Libs",
    "Requires",
};

static const char *sysroot = 0, *pkgcfg_path = 0;
static int libs_mode = 0;
static int static_libs = 0;
static int cflags_mode = 0;

int
get_field_index(int flags) {
  if(flags & (PRINT_LIBS | LIBS_ONLY_L | LIBS_ONLY_OTHER))
    return 2;
  if(flags & (PRINT_CFLAGS | CFLAGS_ONLY_I | CFLAGS_ONLY_OTHER))
    return 1;
  if(flags & (PRINT_REQUIRES))
    return 3;
  if(flags & (PRINT_VERSION))
    return 0;

  buffer_puts(buffer_2, "ERROR: field index flags = ");
  buffer_putxlong(buffer_2, flags);
  buffer_putnlflush(buffer_2);

  exit(2);

  return -1;
}

const char*
host_arch(const char* compiler, stralloc* out) {
  pid_t pid;
  posix_spawn_file_actions_t actions;
  posix_spawnattr_t attr;
  int p[2];
  char* const argv[] = {(char*)compiler, "-dumpmachine", 0};
  char* const envp[1] = {0};

  stralloc_zero(out);

  if(pipe(p) == -1)
    return NULL;
  posix_spawnattr_setflags(&attr, 0);
  posix_spawn_file_actions_init(&actions);
  posix_spawn_file_actions_adddup2(&actions, p[1], 1);

  if(posix_spawnp(&pid, compiler, &actions, &attr, argv, envp) == 0) {
    int ws = 0;
    close(p[1]);

    waitpid_nointr(pid, &ws, WNOHANG);

    readclose_append(p[0], out, 1024);

    stralloc_trimr(out, "\r\t\v\n", 4);
    stralloc_nul(out);
  }
  return out->len ? out->s : 0;
}

/**
 * @brief pkg_get Get a property
 * @param pf      Package structure
 * @param key     Key name
 * @return        1 on success, 0 on failure
 */
const char*
pkg_get(pkg* pf, const char* key) {
  char* v = NULL;
  size_t len;
  MAP_T map = isupper(key[0]) ? pf->fields : pf->vars;
  v = MAP_GET(map, (char*)key, str_len(key) + 1);
  return v;
}

/**
 * @brief wordexp_sa Expands a string
 * @param s          Input string
 * @param sa         Output value
 * @return           1 on sucess, 0 on failure
 */
int
wordexp_sa(const char* s, stralloc* sa, MAP_T vars) {
  const char *x, *v;
  size_t i, n;
  stralloc var;
  stralloc_init(&var);

  for(x = s; *x; x += n) {
    i = str_find(x, "${");
    n = i;

    stralloc_catb(sa, x, i);

    if(x[i]) {
      i += 2;
      x += i;

      n = str_chr(x, '}');

      stralloc_copyb(&var, x, n);
      stralloc_nul(&var);

      v = MAP_GET(vars, var.s, var.len + 1);

      stralloc_cats(sa, v);
      //      wordexp_sa(v, sa, vars);
      n += 1;
    }
  }

  return 1;
}

/**
 * @brief pkg_expand  Expand package property
 * @param pf        Package structure
 * @param key       Property name
 * @param out       Output for value
 * @return          1 on sucess, 0 on failure
 */
int
pkg_expand(pkg* pf, const char* key, stralloc* out) {
  stralloc k;
  const char* s;

  stralloc_init(&k);
  stralloc_copys(&k, key);
  stralloc_nul(&k);

  k.s[0] = toupper(k.s[0]);

  if((s = pkg_get(pf, key)) == NULL)
    return 0;

  {
    stralloc v;
    stralloc_init(&v);
    stralloc_copys(&v, s);

    for(;;) {
      stralloc_nul(&v);
      if(!wordexp_sa(v.s, out, pf->vars))
        return 0;

      if(stralloc_finds(out, "${") == out->len)
        break;

      stralloc_free(&v);
      v = *out;
      stralloc_init(out);
    }
  }

  return 1;
}

/**
 * @brief pkg_free Frees a package structure
 */
void
pkg_free(pkg* p) {
  MAP_DESTROY(p->fields);
  MAP_DESTROY(p->vars);
  stralloc_free(&p->name);
}

/**
 * @brief pkg_read  Read in a package structure
 * @param b         Input buffer
 * @param p         Package structure
 * @return          1 on success, 0 on failure
 */
int
pkg_read(buffer* b, pkg* p) {
  stralloc name, value;
  stralloc_init(&name);
  stralloc_init(&value);

  MAP_NEW(p->vars);
  MAP_NEW(p->fields);

  for(;;) {
    int ret;
    char sep, ch;

    while(buffer_peekc(b, &ch) && (ch == ' ' || ch == '\t')) buffer_skipc(b);

    if((ret = buffer_get_new_token_sa(b, &name, ":=\r\n", 4)) == -1)
      goto fail;
    stralloc_chomp(&name);
    if(name.len > 0 && name.s[0] == '#')
      continue;
    if(ret == 1 && isspace(name.s[0]))
      continue;

    if(ret == 0 || name.s[0] == '\0')
      break;
    if(name.len > 1) {
      sep = name.s[--name.len];
    }

    while(buffer_peekc(b, &ch) && (ch == ' ' || ch == '\t')) buffer_skipc(b);

    stralloc_zero(&value);
    if((ret = buffer_getline_sa(b, &value)) == -1)
      goto fail;

    if(value.s[0] == '\0')
      break;

    if(name.len) {
      stralloc_trimr(&value, "\r\n\t \0", 5);
      stralloc_nul(&value);
      stralloc_nul(&name);

      if(stralloc_starts(&value, "/"))
        stralloc_prepends(&value, sysroot);

#ifdef DEBUG_OUTPUT_
      buffer_putm_3(buffer_2, "Name: ", name.s, "\n");
      buffer_putm_3(buffer_2, "Value: ", value.s, "\n");
      buffer_flush(buffer_2);
#endif
      {
        MAP_T map = sep == '=' ? p->vars : p->fields;
        MAP_INSERT(map, name.s, name.len + 1, value.s, value.len + 1);
      }

      /*     if(islower(name.s[0]))
            setenv(name.s, value.s, 1); */
    }
  }

  buffer_close(b);
  return 1;

fail:
  stralloc_free(&name);
  stralloc_free(&value);
  buffer_close(b);
  return 0;
}

static int
visit_set(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  pkg* p = user_data;
  stralloc var, v;
  stralloc_init(&var);
  stralloc_copyb(&var, key, key_len - 1);
  stralloc_nul(&var);

  stralloc_init(&v);
  if(value_len && ((char*)value)[value_len - 1] == '\0')
    --value_len;
  stralloc_catb(&v, value, value_len);

  /* wordexp_sa(value, &v); */

#ifdef DEBUG_OUTPUT_
  buffer_putm_3(buffer_2, "ENV SET ", key, "=");
  buffer_putsa(buffer_2, &v);
  buffer_putnlflush(buffer_2);
#endif
  stralloc_nul(&v);
  env_set(key, v.s);
  /* setenv(key, v.s, 1); */

  return 1;
}

/**
 * @brief pkg_set  Set environmen for package
 * @param p        Package structure
 * @return         1 on success, 0 on failure
 */
int
pkg_set(pkg* p) {
  MAP_T map = p->vars;
  MAP_VISIT_ALL(map, visit_set, p);
  return 0;
}

static int
visit_unset(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  (void)key_len;
  (void)value;
  (void)value_len;
  (void)user_data;
  /* unsetenv(key); */
  env_unset(key);
  return 1;
}

/**
 * @brief pkg_unset  Unset environment for package
 * @param p        Package structure
 * @return         1 on success, 0 on failure
 */
int
pkg_unset(pkg* p) {
  MAP_VISIT_ALL(p->vars, visit_unset, p);
  return 0;
}

typedef struct {
  buffer* b;
  const char* m;
} dump_t;

static int
visit_dump(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  dump_t* ptr = user_data;
  buffer_put(ptr->b, ptr->m, str_len(ptr->m) - 3);
  buffer_puts(ptr->b, " ");
  buffer_put(ptr->b, key, key_len - 1);
  buffer_puts(ptr->b, isupper(((char*)key)[0]) ? ": " : "=\"");
  buffer_put(ptr->b, value, value_len - 1);
  buffer_puts(ptr->b, isupper(((char*)key)[0]) ? "\n" : "\"\n");
  buffer_flush(ptr->b);
  return 1;
}

/**
 * @brief pkg_dump Dumps a package
 * @param b        Output buffer
 * @param pf       Package structure
 */
void
pkg_dump(buffer* b, pkg* pf) {
  dump_t dump_st;
  dump_st.b = b;
  dump_st.m = str_basename(pf->name.s);
  buffer_putsa(b, &pf->name);
  buffer_putnlflush(b);

  MAP_VISIT_ALL(pf->vars, visit_dump, &dump_st);
  MAP_VISIT_ALL(pf->fields, visit_dump, &dump_st);
}

void
pkg_init(pkg* pf, const char* fn) {
  byte_zero(pf, sizeof(pkg));
  stralloc_copys(&pf->name, fn);
}

/**
 * @brief pkg_list List all packages
 */
void
pkg_list() {
  slink* pkgs;
  slink **it, *item;
  stralloc path, line;
  const char* s;
  size_t n, len;

  slist_init(&pkgs);

  stralloc_init(&path);
  stralloc_init(&line);

  strlist_foreach(&cmd.path, s, n) {
    const char* entry;
    dir_t d;
    stralloc_copyb(&path, s, n);
    len = path.len;
    stralloc_nul(&path);

    if(dir_open(&d, path.s))
      continue;

    while((entry = dir_read(&d))) {
      stralloc_catm_internal(&path, "/", entry, 0);

      if(stralloc_endb(&path, ".pc", 3)) {
        stralloc line;
        buffer pc;
        pkg pf;

        stralloc_init(&line);
        stralloc_nul(&path);
        pkg_init(&pf, path.s);

        if(!buffer_mmapread(&pc, path.s)) {
          path.len -= 3;
          stralloc_nul(&path);

          stralloc_copys(&line, str_basename(path.s));

          if(pkg_read(&pc, &pf)) {
            const char* desc;

            if((desc = pkg_get(&pf, "Description"))) {
              stralloc_cats(&line, " - ");
              stralloc_cats(&line, desc);
            }
          }

          stralloc_nul(&line);

          slink_foreach(&pkgs, it)
            if(str_diff(line.s, slink_data(it)) < 0)
              break;
        
          slist_unshifts(it, line.s);

          byte_zero(&line, sizeof(line));
        }

        pkg_free(&pf);
      }

      path.len = len;
    }
  }

  slist_foreach(pkgs, item) { 
    const char* x = slist_data(item);
    buffer_puts(buffer_1, x);
    buffer_putnlflush(buffer_1);

    free(x);
  }
}

/**
 * @brief pkg_open
 * @param pkgname   Name of the modlue
 * @param pf       Package file structure
 * @return         1 on success, 0 on failure
 */
int
pkg_open(const char* pkgname, pkg* pf) {
  buffer pc;
  const char* s;
  size_t n;
  int ret = 0;

  stralloc_init(&pf->name);

  strlist_foreach(&cmd.path, s, n) {

    stralloc_copyb(&pf->name, s, n);
    stralloc_catm_internal(&pf->name, "/", pkgname, ".pc", 0);
    stralloc_nul(&pf->name);

    if(!buffer_mmapread(&pc, pf->name.s))
      break;
  }

  if(pc.x)
    ret = pkg_read(&pc, pf);
  else
    stralloc_free(&pf->name);

  return ret;
}

/**
 * @brief pkg_conf Evaluates pkgcfg all commands
 * @param pkgname   Package name
 */
int
pkg_conf(strarray* modules, int mode) {
  int i;
  stralloc value;
  stralloc_init(&value);

  for(i = 0; i < strarray_size(modules); ++i) {
    const char* pkgname = strarray_at(modules, i);
    pkg pf;
    byte_zero(&pf, sizeof(pf));

    if(!pkg_open(pkgname, &pf)) {
      if(mode & PKGCFG_EXISTS)
        return 0;
      continue;
    }

    if(cmd.code == PRINT_PATH) {
      if(value.len)
        stralloc_catc(&value, '\n');
      stralloc_cat(&value, &pf.name);
    } else {
      const char* fn = field_names[get_field_index(cmd.code)];

      pkg_set(&pf);

#ifdef DEBUG_OUTPUT_
      pkg_dump(buffer_2, &pf);
#endif

      if(!pkg_expand(&pf, fn, &value)) {
        buffer_flush(buffer_1);
        buffer_flush(buffer_2);
        errmsg_warn("Expanding ", pkgname, "::", fn, NULL);
        pkg_unset(&pf);
        pkg_free(&pf);
        return 0;
      }

      if((cmd.code == PRINT_LIBS && libs_mode) || (cmd.code == PRINT_CFLAGS && cflags_mode)) {
        strlist sl;
        const char* s;
        strlist_init(&sl, '\0');
        stralloc_nul(&value);
        strlist_froms(&sl, value.s, ' ');
        stralloc_zero(&value);

        strlist_foreach_s(&sl, s) {
          if((cmd.code == PRINT_LIBS) && libs_mode) {
            int flag = (/* str_start(s, "-L") ||  */ str_start(s, "-l"));
            if((libs_mode == LIBS_ONLY_L) ^ flag != 0)
              continue;
          }
          if((cmd.code == PRINT_CFLAGS) && libs_mode) {
            int flag = !!(str_start(s, "-I"));
            if((libs_mode == CFLAGS_ONLY_I) ^ flag != 0)
              continue;
          }

          if(value.len > 0)
            stralloc_catc(&value, ' ');
          stralloc_cats(&value, s);
        }
      }
    }

    pkg_unset(&pf);
    pkg_free(&pf);
  }

  if(!(mode & PKGCFG_EXISTS)) {
    /*
     */
    buffer_putsa(buffer_1, &value);
    buffer_putnlflush(buffer_1);
  }
  return 1;
}

char*
search_path(const char* path, const char* what, stralloc* out) {
  const char* x;
  char* ret = 0;
  size_t pos;
  stralloc stra;

  stralloc_init(&stra);

  for(x = path; *x; x += pos) {
    pos = str_chr(x, ':');

    stralloc_copyb(&stra, x, pos);
    stralloc_catc(&stra, '/');
    stralloc_cats(&stra, what);
    stralloc_nul(&stra);

    if(path_exists(stra.s)) {
      stralloc_copy(out, &stra);
      stralloc_nul(out);
      ret = out->s;
      break;
    }

    pos++;
  }
  stralloc_free(&stra);
  return ret;
}

void
pkgcfg_init(const char* argv0) {
  size_t pos;
  const char* x;
  stralloc dir;
  stralloc_init(&dir);

  // path_readlink("/proc/self/exe", &cmd.self);
  stralloc_init(&cmd.self);
  stralloc_init(&cmd.host);
  stralloc_init(&cmd.prefix);

  if(!argv0[str_chr(argv0, '/')]) {

    if((x = search_path(env_get("PATH"), argv0, &dir)))
      argv0 = x;
  }

  if(!argv0[str_chr(argv0, '/')])
    argv0 = (const char*)path_readlink("/proc/self/exe", &dir);

  stralloc_copys(&cmd.self, path_basename(argv0));

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "argv0: ");
  buffer_puts(buffer_2, argv0);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "self: ");
  buffer_putsa(buffer_2, &cmd.self);
  buffer_putnlflush(buffer_2);
#endif

  path_dirname(argv0, &cmd.prefix);

  stralloc_trunc(&cmd.prefix, stralloc_finds(&cmd.prefix, "/bin"));

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "prefix: ");
  buffer_putsa(buffer_2, &cmd.prefix);
  buffer_putnlflush(buffer_2);
#endif

  pos = stralloc_finds(&cmd.self, "pkg");

  if(pos > 0 && cmd.self.s[pos - 1] == '-') {
    pos--;

    stralloc_copyb(&cmd.host, cmd.self.s, pos);
  }

  if(cmd.host.len == 0)
    if(!host_arch("cc", &cmd.host))
      if(!host_arch("gcc", &cmd.host))
        host_arch("clang", &cmd.host);

  stralloc_copy(&dir, &cmd.prefix);
  stralloc_cats(&dir, "/");
  stralloc_cat(&dir, &cmd.host);
  stralloc_cats(&dir, "/sysroot");
  stralloc_nul(&dir);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "dir: ");
  buffer_putsa(buffer_2, &dir);
  buffer_putnlflush(buffer_2);
#endif

  if(path_exists(dir.s) && sysroot == 0) {
    sysroot = str_dup(dir.s);
  }

#ifdef DEBUG_OUTPUT_
  if(sysroot) {
    buffer_puts(buffer_2, "sysroot: ");
    buffer_puts(buffer_2, sysroot);
    buffer_putnlflush(buffer_2);
  }
#endif

  strlist_init(&cmd.path, '\0');

  if(pkgcfg_path) {
    strlist_froms(&cmd.path, pkgcfg_path, ':');
    /*     for(x = pkgcfg_path; *x; x += pos) {
          pos = str_chr(x, ':');
          strlist_pushb(&cmd.path, x, pos);
          pos++;
        } */
  } else {
    stralloc_copy(&dir, &cmd.prefix);
    if(sysroot)
      stralloc_prepends(&dir, sysroot);

    pos = dir.len;

    stralloc_cats(&dir, "/lib/");
    stralloc_cat(&dir, &cmd.host);
    stralloc_cats(&dir, "/pkgconfig");
    stralloc_nul(&dir);

    if(path_exists(dir.s))
      strlist_push_sa(&cmd.path, &dir);

    dir.len = pos;
    stralloc_cats(&dir, "/lib/pkgconfig");
    stralloc_nul(&dir);

    if(path_exists(dir.s))
      strlist_push_sa(&cmd.path, &dir);

    dir.len = pos;
    stralloc_cats(&dir, "/share/pkgconfig");
    stralloc_nul(&dir);

    if(path_exists(dir.s))
      strlist_push_sa(&cmd.path, &dir);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "full qualified host: '");
  buffer_putsa(buffer_2, &cmd.host);
  buffer_putsflush(buffer_2, "'\n");
#endif

  stralloc_free(&dir);
}

void
usage(char* progname) {
  buffer_putm_3(buffer_1, "Usage: ", path_basename(progname), " [OPTIONS] [PACKAGES...]\n");
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1, "  --help, -h                        show this help\n");
  buffer_puts(buffer_1, "  --cflags                          print required CFLAGS to stdout\n");
  buffer_puts(buffer_1, "  --libs                            print required linker flags to stdout\n");
  buffer_puts(buffer_1, "  --path                            show the exact filenames for any matching .pc files\n");
  buffer_puts(buffer_1, "  --modversion                      print the specified module's version to stdout\n");
  buffer_putnlflush(buffer_1);
}

extern buffer* optbuf;

int
main(int argc, char* argv[]) {
  int c;
  int index = 0;
  int mode = 0;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"version", 0, NULL, 'v'},
      {"modversion", 0, NULL, PRINT_VERSION},
      // {"cflags", 0, NULL, PRINT_CFLAGS},
      {"path", 0, NULL, PRINT_PATH},
      {"variable", 1, NULL, 'V'},
      {"list-all", 0, NULL, 'l'},
      {"print-errors", 0, NULL, 'P'},
      {"short-errors", 0, NULL, 'S'},
      {"exists", 0, NULL, 'E'},
      {"libs-only-l", 0, &libs_mode, LIBS_ONLY_L},
      {"libs-only-other", 0, &libs_mode, LIBS_ONLY_OTHER},
      //  {"libs", 0, NULL, PRINT_LIBS},
      {"cflags-only-I", 0, &cflags_mode, CFLAGS_ONLY_I},
      {"cflags-only-other", 0, &cflags_mode, CFLAGS_ONLY_OTHER},
      {"static", 0, NULL, STATIC_LIBS},

      /*   {"atleast", 0, NULL, 0},
          {"atleast-pkgconfig-version", 0, NULL, 0},
          {"atleast-version", 0, NULL, 0},
            {"debug", 0, NULL, 0},
          {"define-prefix", 0, NULL, 0},
          {"define-variable", 0, NULL, 0},
          {"dont-define-prefix", 0, NULL, 0},
          {"errors-to-stdout", 0, NULL, 0},
          {"exact-version", 0, NULL, 0},

          {"max-version", 0, NULL, 0},
          {"prefix-variable", 0, NULL, 0},
          {"print-provides", 0, NULL, 0},
          {"print-requires", 0, NULL, 0},
          {"print-requires-private", 0, NULL, 0},
          {"print-variables", 0, NULL, 0},
          {"silence-errors", 0, NULL, 0},
          {"uninstalled", 0, NULL, 0},
          {"validate", 0, NULL, 0}, */

      {0, 0, 0, 0},
  };

  errmsg_iam(argv[0]);

#ifdef _MSC_VER
  optbuf = buffer_1;
#endif
  opterr = 0;

  for(;;) {
    c = getopt_long(argc, argv, "hmilpaPSvV:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {

      case 'h': usage(argv[0]); return 0;
      case 'V': break;
      case 'v': {
        buffer_putsflush(buffer_1, "1.0\n");
        exit(0);
      }
      case LIBS_ONLY_L:
      case LIBS_ONLY_OTHER:
        cmd.code |= PRINT_LIBS;
        libs_mode = c;
        break;
      case CFLAGS_ONLY_I:
      case CFLAGS_ONLY_OTHER:
        cmd.code |= PRINT_CFLAGS;
        cflags_mode = c;
        break;

      case STATIC_LIBS: static_libs = 1; break;
      case PRINT_VERSION:
      case PRINT_CFLAGS:
      case PRINT_LIBS:
      case PRINT_PATH:

        if(!cmd.code)
          cmd.code |= c;
        break;
      case 'l':
        if(!cmd.code)
          cmd.code = LIST_ALL;
        break;

      case 'P': mode = PKGCFG_PRINT_ERR; break;
      case 'S': mode = PKGCFG_SHORT_ERR; break;
      case 'E': mode = PKGCFG_EXISTS; break;
      case '?': {

        const char* arg = argv[optind];

        if(!str_diffn(arg, "--libs", 6) || !str_diffn(arg, "--cflags", 8)) {
          int i = arg[2] == 'l' ? PRINT_LIBS : PRINT_CFLAGS;
          cmd.code |= i;
          if(i == PRINT_LIBS)
            libs_mode = arg[str_find(arg, "only")] ? (arg[str_find(arg, "other")] ? LIBS_ONLY_OTHER : LIBS_ONLY_L) : 0;
          else
            cflags_mode =
                arg[str_find(arg, "only")] ? (arg[str_find(arg, "other")] ? CFLAGS_ONLY_OTHER : CFLAGS_ONLY_I) : 0;

          argv[optind] = "--static";
          /*           for(i = optind; argv[i]; i++)
                      argv[i] = argv[i+1];
           */
          continue;
        }

        goto getopt_end;
      }

      default:
        buffer_puts(buffer_2, "WARNING: Invalid argument -");
        buffer_putc(buffer_2, isprint(c) ? c : '?');
        buffer_putm_internal(buffer_2, " '", optarg ? optarg : argv[optind], "'", 0);
        buffer_putnlflush(buffer_2);
        usage(argv[0]);
        return 1;
        break;
    }
  }
getopt_end:

  if(libs_mode)
    cmd.code |= PRINT_LIBS;
  if(cflags_mode)
    cmd.code |= PRINT_CFLAGS;

  sysroot = env_get("PKG_CONFIG_SYSROOT");

  pkgcfg_path = env_get("PKG_CONFIG_PATH");

  pkgcfg_init(argv[0]);

  if(!sysroot)
    sysroot = "";

    // strlist_froms(&cmd.path, pkgcfg_path, ':');
    /*
      if(strlist_count(&cmd.path) == 0) {

        {
          size_t len = stralloc_finds(&prefix, "/bin");

          if(len == prefix.len) {
            stralloc_copys(&prefix, "/usr");
            len = prefix.len;
          }
          prefix.len = len;
          stralloc_cats(&prefix, "/lib/pkgconfig");
          strlist_push_sa(&cmd.path, &prefix);
          prefix.len = len;
          stralloc_cats(&prefix, "/share/pkgconfig");
          strlist_push_sa(&cmd.path, &prefix);
        }
      } */

#ifdef PKGCONF_DEBUG
  buffer_putm_2(buffer_2, path_basename(argv[0]), ": ");
  buffer_puts(buffer_2, "PKG_CONFIG_PATH is ");
  buffer_putsa(buffer_2, &cmd.path.sa);
  buffer_putnlflush(buffer_2);
#endif

  if(cmd.code == LIST_ALL) {
    pkg_list();
    return 0;
  } else if(optind < argc) {
    strarray modules;
    strarray_from_argv(argc - optind, (const char* const*)&argv[optind], &modules);
    return pkg_conf(&modules, mode);
  } else {
    buffer_puts(buffer_2, "Must specify package names on the command line");
    buffer_putnlflush(buffer_2);
    return 1;
  }
}
