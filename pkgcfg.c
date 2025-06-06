#define _GNU_SOURCE

#include "lib/unix.h"
#include "lib/algorithm.h"
#include "lib/uint64.h"
#include "lib/uint16.h"
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
#include "lib/case.h"
#include "lib/scan.h"
#include "lib/fnmatch.h"
#include "lib/process.h"
#define MAP_USE_HMAP 1
#include "lib/map.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <ctype.h>
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
  LIST_PATH = 64,
  LIST_FILE = 128,
  ATLEAST_PKGCONFIG_VERSION = 4096,
  CHECK_EXISTS = 8192,
  MATCH = 0x10000,
  VARIABLE = 0x20000
} id;
typedef enum { OP_EQ = 0, OP_NE, OP_GT, OP_GE, OP_LT, OP_LE } op_code;

typedef enum { LIBS_ONLY_L = 64, LIBS_ONLY_LIBPATH = 128, LIBS_ONLY_OTHER = 256 } libs_mode_t;

typedef enum { CFLAGS_ONLY_I = 512, CFLAGS_ONLY_OTHER = 1024 } cflags_mode_t;

typedef struct cmd_s {
  strlist path;
  stralloc self;
  stralloc host;
  stralloc prefix;
} cmd_t;

static void
cmd_nul(cmd_t* c) {
  stralloc_nul(&c->path.sa);
  stralloc_nul(&c->self);
  stralloc_nul(&c->host);
  stralloc_nul(&c->prefix);
}

static void
cmd_dump(cmd_t* c, buffer* b) {
  buffer_puts(b, "path: ");
  buffer_putsl(b, &c->path, ":");
  buffer_puts(b, "\nself: ");
  buffer_putsa(b, &c->self);
  buffer_puts(b, "\nhost: ");
  buffer_putsa(b, &c->host);
  buffer_puts(b, "\nprefix: ");
  buffer_putsa(b, &c->prefix);
  buffer_putnlflush(b);
}

typedef struct pkg_s {
  stralloc name;
  MAP_T vars;
  MAP_T fields;
  uint64 version;
} pkg;

typedef struct cond_s {
  stralloc op_str;
  op_code op_val;
  stralloc version_str;
  uint64 version_val;
} cond;

static array cmds;
static cmd_t cmd;
static const char* const field_names[] = {
    "Version",
    "Cflags",
    "Libs",
    "Requires",
};
static const char* const op_strings[] = {"==", "!=", ">", ">=", "<", "<="};

#ifndef CONFIG_PREFIX
#define CONFIG_PREFIX "/usr"
#endif

static const char* default_prefix = CONFIG_PREFIX;
static const char* sysroot = 0;
static const char* match_pattern = 0;
static const char* variable = 0;
static int libs_mode = 0;
static int static_libs = 0;
static int cflags_mode = 0;
static int sorted = 0;
static int verbose = 0;
static int debug = 0;
static int show_version = 0;
static int atleast_version = 0;
const char* pkgcfg_path = 0;
extern buffer* unix_optbuf;
static strlist args, output;
static strarray modules = {0, 0, 0};

static void
add_cmd(id cmd) {
  if(array_indexof(&cmds, sizeof(id), &cmd) == -1)
    array_catb(&cmds, &cmd, sizeof(id));
}

static int
get_field_index(int flags) {
  if(flags & (PRINT_LIBS | LIBS_ONLY_L | LIBS_ONLY_LIBPATH | LIBS_ONLY_OTHER))
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

static const char*
get_field_name(int flags) {
  if(flags == -1)
    return NULL;

  if(flags & PRINT_PATH)
    return 0;

  if(flags & VARIABLE)
    return variable;
  return field_names[get_field_index(flags)];
}

static int
get_op_index(const stralloc* op) {
  int i;

  for(i = 0; op_strings[i]; i++) {
    if(stralloc_equals(op, op_strings[i]))
      return i;
  }
  return -1;
}

static char*
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

static void
extract_paths(const stralloc* sa, strlist* paths) {
  const char *s, *e;
  size_t n;
  s = sa->s;
  e = sa->s + sa->len;

  while(s < e) {
    n = scan_whitenskip(s, e - s);

    if((s += n) == e)
      break;
    n = scan_nonwhitenskip(s, e - s);

    if(*s == PATHSEP_C) {
      strlist_pushb(paths, s, n);
    }
    s += n;
  }
}

const char*
exec_program(const char* compiler, const char* arg, stralloc* out) {
  int pid;
  int ws = 0;
#if 0 // def POSIX_SPAWN
  posix_spawn_file_actions_t actions;
  posix_spawnattr_t attr;
#endif
  int p[2];
  int stdio[3];
  char* argv[3];
  char* envp[1];
  const char* bin;
  stralloc dir;
  argv[0] = (char*)compiler;
  argv[1] = (char*)arg;
  argv[2] = NULL;
  envp[0] = NULL;

  stralloc_init(&dir);
  stralloc_zero(out);

#if WINDOWS_NATIVE
  if(_pipe(p, 2048, 1) == -1)
    return NULL;
#else
  if(pipe(p) == -1)
    return NULL;
#endif

  if(path_is_absolute(compiler)) {
    bin = compiler;
  } else if((bin = search_path(env_get("PATH"), compiler, &dir)) == 0) {
    errmsg_warnsys(bin, " not found: ", 0);
    exit(127);
  }

  stdio[0] = 0;
  stdio[1] = p[1];
  stdio[2] = p[1];

  if((pid = process_create(bin, argv, stdio, 0)) < 0) {
    errmsg_warnsys("process_create error ", bin, ": ", 0);
    return 0;
  }

  close(p[1]);
  ws = process_wait(pid);

  readclose_append(p[0], out, 1024);

  stralloc_trimr(out, "\r\t\v\n", 4);
  stralloc_nul(out);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "host_arch out='");
  buffer_putsa(buffer_2, out);
  buffer_putsflush(buffer_2, "'\n");
#endif

  return out->len ? out->s : 0;
}

const char*
host_arch(const char* compiler, stralloc* out) {
  const char* s;
  int tcc = str_contains(compiler, "tcc");

  s = exec_program(compiler, tcc ? "-vv" : "-dumpmachine", out);

  if(tcc) {
    const char* p;
    strlist paths;
    strlist_init(&paths, '\0');
    extract_paths(out, &paths);
    strlist_foreach_s(&paths, p) {
      if(path_exists(p)) {
        size_t pos = str_find(p, PATHSEP_S "lib" PATHSEP_S);

        if(p[pos]) {
          const char* h = &p[pos + 5];
          size_t hlen = str_chr(h, PATHSEP_C);

          if(byte_count(h, hlen, '-') >= 2) {
            stralloc_copyb(out, h, hlen);
            stralloc_nul(out);
            s = out->s;

#ifdef DEBUG_OUTPUT_
            buffer_puts(buffer_2, "host_arch h='");
            buffer_put(buffer_2, h, hlen);
            buffer_putsflush(buffer_2, "'\n");
#endif
            goto end;
          }
        }
      }
    }
  }
end:
  return s;
}

/**
 * @brief pkg_get Get a property
 * @param pf      Package structure
 * @param key     Key name
 * @return        1 on success, 0 on
 * failure
 */
const char*
pkg_get(pkg* pf, const char* key) {
  char* v = NULL;
  size_t len;
  MAP_T map = isupper(key[0]) ? pf->fields : pf->vars;

#ifdef DEBUG_OUTPUT_
  if(str_diff(key, "Description")) {
    buffer_puts(buffer_2, "Get: ");
    buffer_putsa(buffer_2, &pf->name);
    buffer_puts(buffer_2, " key: ");
    buffer_puts(buffer_2, key);
    buffer_putnlflush(buffer_2);
  }
#endif
  v = MAP_GET(map, (char*)key, str_len(key) + 1);
  return v;
}

/**
 * @brief wordexp_sa Expands a string
 * @param s          Input string
 * @param sa         Output value
 * @return           1 on sucess, 0 on
 * failure
 */
static int
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
 * @brief pkg_expand  Expand package
 * property
 * @param pf        Package structure
 * @param key       Property name
 * @param out       Output for value
 * @return          1 on sucess, 0 on
 * failure
 */
int
pkg_expand(pkg* pf, const char* key, stralloc* out) {
  stralloc k, v;
  const char* s;
  stralloc_init(&k);
  stralloc_init(&v);
  stralloc_copys(&k, key);
  stralloc_nul(&k);
  k.s[0] = toupper(k.s[0]);

  if((s = pkg_get(pf, key)) == NULL)
    return 0;
  stralloc_copys(&v, s);

  for(;;) {
    stralloc_nul(&v);

    if(!wordexp_sa(v.s, out, pf->vars))
      return 0;

    if(stralloc_finds(out, "${") == out->len)
      break;
    stralloc_copy(&v, out);
    stralloc_zero(out);
  }
  return 1;
}

/**
 * @brief pkg_free Frees a package
 * structure
 */
void
pkg_free(pkg* p) {
  MAP_DESTROY(p->fields);
  MAP_DESTROY(p->vars);

  stralloc_free(&p->name);
}

void
pkg_parse_version(uint64* v, const char* x, size_t n) {
  int s;
  *v = 0;

  for(s = 48; s >= 0; s -= 16) {
    uint16 num = 0xffff;

    if(n) {
      size_t i, j;
      i = scan_ushort(x, &num);
      j = scan_noncharsetnskip(&x[i], "0123456789", n - i);

      if(i + j > 0) {
        x += i + j;
        n -= i + j;
      } else {
        n = 0;
      }
    }
    *v |= (num & 0xffff) << s;
  }
}

size_t
pkg_format_version(uint64 v, stralloc* sa) {
  int s;
  stralloc_zero(sa);

  for(s = 48; s >= 0; s -= 16) {
    uint16 num = (v >> s) & 0xffff;

    if(num == 0xffff)
      break;

    if(sa->len)
      stralloc_catb(sa, ".", 1);
    stralloc_catlong(sa, num);
  }
  return sa->len;
}

/**
 * @brief pkg_read  Read in a package
 * structure
 * @param b         Input buffer
 * @param p         Package structure
 * @return          1 on success, 0 on
 * failure
 */
int
pkg_read(buffer* b, pkg* p) {
  stralloc name, value;
  stralloc_init(&name);
  stralloc_init(&value);

  MAP_NEW(p->vars);
  MAP_NEW(p->fields);
  p->version = 0;

  for(;;) {
    int ret;
    char sep, ch;

    while(buffer_peekc(b, &ch) && (ch == ' ' || ch == '\t'))
      buffer_skipc(b);

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

    while(buffer_peekc(b, &ch) && (ch == ' ' || ch == '\t'))
      buffer_skipc(b);

    stralloc_zero(&value);

    if((ret = buffer_getline_sa(b, &value)) == -1)
      goto fail;

    if(value.s[0] == '\0')
      break;

    if(name.len) {
      stralloc_trimr(&value, "\r\n\t \0", 5);

      if(stralloc_starts(&value, "/"))
        stralloc_prepends(&value, sysroot);

      stralloc_remove_all(&value, "\"'", 2);

      stralloc_nul(&name);
      stralloc_nul(&value);

#ifdef DEBUG_OUTPUT
      buffer_putm_internal(buffer_2, "Name: ", name.s, "\n", NULL);
      buffer_putm_internal(buffer_2, "Value: ", value.s, "\n", NULL);
      buffer_flush(buffer_2);
#endif

      if(stralloc_equals(&name, "Version"))
        pkg_parse_version(&p->version, value.s, value.len);

      {
        MAP_T map = sep == '=' ? p->vars : p->fields;
        MAP_INSERT2(map, name.s, name.len + 1, value.s, value.len + 1);
      }
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
  buffer_putm_internal(buffer_2, "ENV SET ", key, "=", NULL);
  buffer_putsa(buffer_2, &v);
  buffer_putnlflush(buffer_2);
#endif
  stralloc_nul(&v);
  env_set(key, v.s);
  /* setenv(key, v.s, 1); */

  return 1;
}

/**
 * @brief pkg_set  Set environment for package
 * @param p        Package structure
 * @return         1 on success, 0 on
 * failure
 */
int
pkg_set(pkg* p) {
  MAP_VISIT_ALL(p->vars, visit_set, p);
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
 * @brief pkg_unset  Unset environment
 * for package
 * @param p        Package structure
 * @return         1 on success, 0 on
 * failure
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

int
pkgcfg_sort_compare(const char** a, const char** b) {
  return case_diffs(*a, *b);
}

/**
 * @brief pkg_list List all packages
 */
void
pkg_list(id code) {
  slink *pkgs, **it, *item;
  stralloc path, line;
  const char* s;
  size_t n, i, len;
  int found;

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
      path.len = len;
      stralloc_catm_internal(&path, "/", entry, NULL);

      if(stralloc_endb(&path, ".pc", 3)) {
        stralloc line;
        buffer pc;
        pkg pf;

        stralloc_init(&line);
        stralloc_nul(&path);

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "file: ");
        buffer_putsa(buffer_2, &path);
        buffer_putnlflush(buffer_2);
#endif

        if(match_pattern && path_fnmatch(match_pattern, str_len(match_pattern), path.s, path.len, FNM_CASEFOLD) == FNM_NOMATCH)
          continue;

        pkg_init(&pf, path.s);

        if(!buffer_mmapread(&pc, path.s)) {
          stralloc_zero(&line);
          i = byte_rchr(path.s, path.len, '/');

          if(i == path.len)
            i = 0;
          else
            i++;
          n = path.len - i - 3;

          if(code == LIST_FILE)
            stralloc_cat(&line, &path);
          else
            stralloc_catb(&line, path.s + i, n);

          if(code == LIST_PATH) {
            if(verbose == 1)
              stralloc_zero(&line);
            else if(line.len > 0)
              stralloc_cats(&line, " ");
            stralloc_catb(&line, path.s, verbose == 0 ? i - 1 : path.len);
          } else if(pkg_read(&pc, &pf)) {
            const char* desc;

            if((desc = pkg_get(&pf, "Description"))) {
              stralloc_cats(&line, " - ");
              stralloc_cats(&line, desc);
            }
          }
          stralloc_catc(&line, '\n');
          stralloc_nul(&line);
          n = byte_chr(line.s, line.len, ' ');

          if(line.s[n])
            n++;
          i = 0;
          found = 0;
          slist_foreach(pkgs, item) {
            char* s = *(char**)slist_data(item);

            if(!str_diffn(s, line.s, n)) {
              found = 1;
              break;
            }
            i++;
          }

          if(!found) {
            if(sorted) {
              slink_foreach(&pkgs, it) if(case_diffs(line.s, *(char**)slink_data(it)) < 0) break;
              slist_unshifts(it, line.s);
            } else {
              slist_pushs(&pkgs, line.s);
            }
          }
          stralloc_free(&line);
        }

        pkg_free(&pf);
      }

      path.len = len;
    }
  }

  while(pkgs) {
    char* x = *(char**)slist_data(pkgs);
    buffer_putsflush(buffer_1, x);
    slist_removes(&pkgs);
  }
}

/**
 * @brief pkg_open
 * @param pkgname   Name of the modlue
 * @param pf       Package file
 * structure
 * @return         1 on success, 0 on
 * failure
 */
int
pkg_open(const char* pkgname, pkg* pf) {
  buffer pc = {0};
  const char* s;
  size_t n;
  int ret = 0;

  stralloc_init(&pf->name);
  strlist_foreach(&cmd.path, s, n) {
    stralloc_copyb(&pf->name, s, n);
    stralloc_catm_internal(&pf->name, "/", pkgname, ".pc", NULL);
    stralloc_nul(&pf->name);

    if(!buffer_mmapread(&pc, pf->name.s))
      break;
  }

  if(pc.x) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "opened: ");
    buffer_puts(buffer_2, pf->name.s);
    buffer_putnlflush(buffer_2);
#endif
    ret = pkg_read(&pc, pf);
  } else
    stralloc_free(&pf->name);
  return ret;
}

int
pkg_parse_cond(cond* c, const char* x, size_t n) {
  int op = -1;
  size_t i = scan_nonwhitenskip(x, n);
  stralloc_init(&c->op_str);
  stralloc_init(&c->version_str);
  stralloc_copyb(&c->op_str, x, i);
  op = get_op_index(&c->op_str);
  c->op_val = op;
  x += i;
  n -= i;
  i = scan_whitenskip(x, n);
  x += i;
  n -= i;
  stralloc_copyb(&c->version_str, x, n);
  pkg_parse_version(&c->version_val, x, n);
  return 0;
}

int
pkg_check_cond(const cond* c, const pkg* p) {
  switch(c->op_val) {
    case OP_EQ: return p->version == c->version_val;
    case OP_NE: return p->version != c->version_val;
    case OP_GT: return p->version > c->version_val;
    case OP_GE: return p->version >= c->version_val;
    case OP_LT: return p->version < c->version_val;
    case OP_LE: return p->version <= c->version_val;
  }
  return -1;
}

void
pkg_dump_cond(const cond* c) {
  buffer_puts(buffer_2, "op_str='");
  buffer_putsa(buffer_2, &c->op_str);

  buffer_puts(buffer_2, "', op-val = ");
  buffer_putlong(buffer_2, c->op_val);
  buffer_puts(buffer_2, ", version_str = '");
  buffer_putsa(buffer_2, &c->version_str);
  buffer_puts(buffer_2, "'");
  buffer_puts(buffer_2, ", version_val = ");
  buffer_putxlonglong0(buffer_2, c->version_val, 16);
  {
    stralloc ver;
    stralloc_init(&ver);
    pkg_format_version(c->version_val, &ver);
    buffer_puts(buffer_2, ", version_fmt = ");
    buffer_putsa(buffer_2, &ver);
  }

  buffer_putnlflush(buffer_2);
}

void
pkg_output(const char* x, size_t n) {
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "pkg_output: '");
  buffer_put(buffer_2, x, n);
  buffer_putsflush(buffer_2, "'\n");
#endif
  strlist_pushb_unique(&output, x, n);
}

static cond condition;

/**
 * @brief pkg_conf Evaluates pkgcfg all
 * commands
 * @param pkgname   Package name
 */
int
pkg_conf(strarray* modules, id code, int mode) {
  int ret = 0, i, do_cond;
  size_t n, pos;
  const char* x;
  size_t len;
  strlist output, require;
  stralloc name, cond, value;
  stralloc_init(&name);
  stralloc_init(&cond);
  stralloc_init(&value);
  strlist_init(&output, '\0');
  strlist_init(&require, ' ');

  for(i = 0; i < strarray_size(modules); ++i) {
    const char* pkgname = strarray_AT(modules, i);
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "pkgname: ");
    buffer_puts(buffer_2, pkgname);
    buffer_puts(buffer_2, " (");
    buffer_putlong(buffer_2, i);
    buffer_puts(buffer_2, ")");
    buffer_putnlflush(buffer_2);
#endif
    pkg pf;
    byte_zero(&pf, sizeof(pf));
    stralloc_copys(&name, pkgname);
    stralloc_zero(&cond);
    n = byte_chr(name.s, name.len, ' ');

    do_cond = 0;

    if(n < name.len) {
      pos = n;

      if(pos < name.len)
        pos += scan_whitenskip(&name.s[pos], name.len - pos);
      pkg_parse_cond(&condition, &name.s[pos], name.len - pos);

      do_cond = 1;
      name.len = n;
    }
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "name: ");
    buffer_putsa(buffer_2, &name);
    buffer_puts(buffer_2, " cond: ");
    buffer_putsa(buffer_2, &cond);
    buffer_putnlflush(buffer_2);
#endif
    stralloc_nul(&name);

    if(!pkg_open(name.s, &pf)) {
      buffer_putm_internal(buffer_2, "No package '", name.s, "' found", NULL);
      buffer_putnlflush(buffer_2);
      return 1;
    }

    if(do_cond) {
      int r = pkg_check_cond(&condition, &pf);

      if(r == -1) {
      } else if(r == 0) {
        continue;
      }
    }

    if(mode & PKGCFG_EXISTS)
      return 0;
#ifdef DEBUG_OUTPUT
    pkg_dump(buffer_2, &pf);
#endif
    stralloc_zero(&value);
    pkg_expand(&pf, "Requires", &value);
    strlist_zero(&require);
    strlist_fromb(&require, value.s, value.len, " ,\r\n\t");
    strlist_foreach(&require, x, len) {
#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "Require: ");
      buffer_put(buffer_2, x, len);
      buffer_putnlflush(buffer_2);
#endif
      strarray_pushb_unique(modules, x, len);
    }
    {
      const char* fn = 0;

      if(code & PRINT_PATH) {
        if(value.len)
          stralloc_catc(&value, '\n');
        stralloc_cat(&value, &pf.name);
      }

      if(code & VARIABLE) {
#ifdef DEBUG_OUTPUT_
        buffer_putm_internal(buffer_2, "Variable '", variable, "': ", NULL);
        buffer_putsa(buffer_2, &value);
        buffer_putnlflush(buffer_2);
#endif
        fn = variable;
      } else {
        fn = get_field_name(code);
      }
      pkg_set(&pf);

      if(fn) {
        stralloc_zero(&value);

        if(!pkg_expand(&pf, fn, &value)) {
          buffer_flush(buffer_1);
          buffer_flush(buffer_2);
          errmsg_warn("Expanding ", pkgname, "::", fn, NULL);
          pkg_unset(&pf);
          pkg_free(&pf);
          return 0;
        }
      }

      if((code == PRINT_LIBS && libs_mode) || (code == PRINT_CFLAGS && cflags_mode)) {
        strlist sl;
        const char* s;
        strlist_init(&sl, '\0');
        stralloc_nul(&value);
        strlist_froms(&sl, value.s, ' ');
        stralloc_zero(&value);
        strlist_foreach_s(&sl, s) {
          if((code == PRINT_LIBS) && libs_mode) {
            if((libs_mode == LIBS_ONLY_L) && !str_start(s, "-l"))
              continue;

            if((libs_mode == LIBS_ONLY_LIBPATH) && !str_start(s, "-L"))
              continue;

            if((libs_mode == LIBS_ONLY_OTHER) && str_case_start(s, "-l"))
              continue;
          }

          if((code == PRINT_CFLAGS) && libs_mode) {
            int flag = !!(str_start(s, "-I"));

            if((libs_mode == CFLAGS_ONLY_I) ^ (flag != 0))
              continue;
          }

          if(value.len > 0)
            stralloc_catc(&value, ' ');
          stralloc_cats(&value, s);
        }
      }
    }

    if(!(mode & PKGCFG_EXISTS)) {
      char *x, *end;
      size_t n, i;

      for(x = value.s, n = value.len, end = x + n; x < end; x += i + 1) {
        i = byte_chr(x, end - x, ' ');
        strlist_pushb_unique(&output, x, i);
      }

      if(value.len) {
        pkg_output(value.s, value.len);
      }
    }
    stralloc_zero(&name);
    stralloc_zero(&cond);
    stralloc_zero(&value);
    pkg_unset(&pf);
    pkg_free(&pf);
  }

  if(!(mode & PKGCFG_EXISTS)) {
    if(value.len) {
      buffer_putsa(buffer_1, &value);
      buffer_putnlflush(buffer_1);
    }
  } else {
    ret = 1;
  }
  stralloc_free(&name);
  stralloc_free(&cond);
  stralloc_free(&value);
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "pkg_conf output: ");
  buffer_putsl(buffer_2, &output, ", ");
  buffer_putnlflush(buffer_2);
#endif
  strlist_free(&output);
  strlist_free(&require);
  return ret;
}

static int
add_path(const stralloc* dir) {
  if(path_exists(dir->s)) {
    strlist_push_sa(&cmd.path, dir);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "add_path exists: ");
    buffer_putsa(buffer_2, dir);
    buffer_putnlflush(buffer_2);
#endif

    return 1;
  }
  return 0;
}

static int
add_paths(const stralloc* prefix) {
  size_t pos;
  int n = 0;
  stralloc dir;
  stralloc_init(&dir);
  stralloc_copy(&dir, prefix);

  pos = dir.len;

  stralloc_cats(&dir, PATHSEP_S "lib" PATHSEP_S);
  stralloc_cat(&dir, &cmd.host);
  stralloc_cats(&dir, PATHSEP_S "pkgconfig");
  stralloc_nul(&dir);

  n += add_path(&dir);

  dir.len = pos;

  stralloc_cats(&dir, PATHSEP_S "lib" PATHSEP_S "pkgconfig");
  stralloc_nul(&dir);

  n += add_path(&dir);

  dir.len = pos;

  stralloc_cats(&dir, PATHSEP_S "share" PATHSEP_S "pkgconfig");
  stralloc_nul(&dir);

  n += add_path(&dir);

  /* dir.len = byte_rchr(dir.s, pos, PATHSEP_C);

  if(dir.len < pos)
  n += add_paths(&dir); */

  return n;
}

void
pkgcfg_init(const char* self) {
  size_t pos;
  const char* x;
  stralloc dir;
  stralloc_init(&dir);
  // path_readlink("/proc/self/exe",
  // &cmd.self);
  stralloc_init(&cmd.self);
  stralloc_init(&cmd.host);
  stralloc_init(&cmd.prefix);

  if(!self[str_chr(self, '/')]) {
    if((x = search_path(env_get("PATH"), self, &dir)))
      self = x;
  }

  if(!self[str_chr(self, '/')]) {
    path_readlink("/proc/self/exe", &dir);
    stralloc_nul(&dir);
    self = dir.s;
  }
  stralloc_copys(&cmd.self, path_basename(self));
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "self: ");
  buffer_puts(buffer_2, self);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "cmd.self: ");
  buffer_putsa(buffer_2, &cmd.self);
  buffer_putnlflush(buffer_2);
#endif

  if(str_contains(self, "/bin/")) {
    path_dirname(self, &cmd.prefix);
    stralloc_trunc(&cmd.prefix, stralloc_finds(&cmd.prefix, "/bin"));
  } else {
    stralloc_copys(&cmd.prefix, default_prefix);
  }
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "cmd.prefix: ");
  buffer_putsa(buffer_2, &cmd.prefix);
  buffer_putnlflush(buffer_2);
#endif
  pos = stralloc_finds(&cmd.self, "pkg");

  if(cmd.host.len == 0) {
    if((pos = stralloc_finds(&cmd.self, "pkg")) > 0 && byte_count(cmd.self.s, pos, '-') >= 2) {
      stralloc_copyb(&cmd.host, cmd.self.s, pos);
    } else {
      const char* compiler = env_get("CC");

      if(compiler && compiler[0]) {
        if(!host_arch(compiler, &cmd.host)) {
          errmsg_warnsys("Failed executing compiler '", compiler, "': ", 0);
        }
      } else {
        if(!host_arch("cc", &cmd.host))

          if(!host_arch("gcc", &cmd.host))
            host_arch("clang", &cmd.host);
      }
    }
  }
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "cmd.host: ");
  buffer_putsa(buffer_2, &cmd.host);
  buffer_putnlflush(buffer_2);
#endif
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
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "full qualified host: '");
  buffer_putsa(buffer_2, &cmd.host);
  buffer_putsflush(buffer_2, "'\n");
#endif
  stralloc_free(&dir);
  stralloc_nul(&cmd.prefix);
  stralloc_nul(&cmd.host);
  stralloc_nul(&cmd.self);
}

void
pkgcfg_setpath(const char* path) {
  stralloc dir;
  stralloc_init(&dir);
  strlist_init(&cmd.path, ':');

  if(path) {
    strlist_froms(&cmd.path, path, ':');
  } else {
    const char* local = PATHSEP_S "local";
    /*   if(sysroot)
          stralloc_copys(&dir, sysroot);
        else */
    stralloc_copy(&dir, &cmd.prefix);
    stralloc_nul(&dir);
    add_paths(&dir);

    if(str_end(dir.s, local)) {
      dir.len -= str_len(local);
      stralloc_nul(&dir);
      add_paths(&dir);
    }

    if(str_end(dir.s, PATHSEP_S "usr")) {
      dir.len -= 4;
      stralloc_nul(&dir);
      add_paths(&dir);
    }
  }
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "cmd.path: ");
  buffer_putsa(buffer_2, &cmd.path.sa);
  buffer_putnlflush(buffer_2);
#endif
  stralloc_nul(&cmd.path.sa);
  stralloc_free(&dir);
}

static void
error_exit(int exitCode) {
  buffer_puts(buffer_2, "The arguments leading up to this were:\n  ");
  buffer_putsl(buffer_2, &args, " ");
  buffer_putnlflush(buffer_2);
  exit(exitCode);
}

void
usage(char* progname) {
  buffer_putm_internal(buffer_1, "Usage: ", path_basename(progname), " [OPTIONS] [PACKAGES...]\n", NULL);
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1, "  --help, -h                        show this help\n");
  buffer_puts(buffer_1, "  --prefix, -p DIRECTORY            Set prefix directory\n");
  buffer_puts(buffer_1, "  --host, -m MACHINE                Set canonical host\n");
  buffer_puts(buffer_1, "  --cflags                          print required CFLAGS               o stdout\n");
  buffer_puts(buffer_1, "  --libs                            print required linker               lags to stdout\n");
  buffer_puts(buffer_1, "  --path                            show the exact               ilenames for any matching .pc files\n");
  buffer_puts(buffer_1, "  --modversion                      print the specified               odule's version to stdout\n");
  buffer_puts(buffer_1, "  --list-all                        list all known packages\n");
  buffer_puts(buffer_1, "  --list-path                       list path of all known               ackages\n");
  buffer_puts(buffer_1, "  --verbose                         increase verbosity\n");
  buffer_puts(buffer_1, "  --debug                           show verbose debug               nformation\n");
  buffer_puts(buffer_1, "  --sorted                          sorted list output\n");
  buffer_puts(buffer_1, "  --unsorted                        unsorted list output               default)\n\n");
  buffer_putm_internal(buffer_1, "Default prefix: ", cmd.prefix.s, "\n", NULL);
  buffer_putm_internal(buffer_1, "Default host: ", cmd.host.s, "\n", NULL);
  buffer_puts(buffer_1, "Default search path:\n  ");

  buffer_putsl(buffer_1, &cmd.path, "\n  ");
  buffer_putnlflush(buffer_1);
}

void
pkgcfg_dumpenv(char* envp[]) {
  strarray stra;
  strarray_init(&stra);
  strarray_from_vec(&stra, envp);
  strarray_sort(&stra, 0);

  buffer_puts(buffer_2, "Environment:\n  ");

  buffer_putstra(buffer_2, &stra, "\n  ");
  buffer_putnlflush(buffer_2);
}

int
main(int argc, char* argv[], char* envp[]) {
  int c;
  id* code;
  int index = 0;
  int mode = 0;
  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"version", 0, &show_version, 1},
      {"modversion", 0, NULL, PRINT_VERSION},
      {"path", 0, NULL, PRINT_PATH},
      {"verbose", 0, &verbose, 1},
      {"match", 0, NULL, MATCH},
      {"variable", 1, NULL, VARIABLE},
      {"list-all", 0, NULL, 'l'},
      {"list-path", 0, NULL, 'L'},
      {"list-file", 0, NULL, 'F'},
      {"print-errors", 0, NULL, 'P'},
      {"short-errors", 0, NULL, 'S'},
      {"exists", 0, NULL, 'E'},
      {"libs-only-l", 0, &libs_mode, LIBS_ONLY_L},
      {"libs-only-L", 0, &libs_mode, LIBS_ONLY_LIBPATH},
      {"libs-only-other", 0, &libs_mode, LIBS_ONLY_OTHER},
      {"libs", 0, NULL, PRINT_LIBS},
      {"cflags-only-I", 0, &cflags_mode, CFLAGS_ONLY_I},
      {"cflags-only-other", 0, &cflags_mode, CFLAGS_ONLY_OTHER},
      {"cflags", 0, NULL, PRINT_CFLAGS},
      {"static", 0, &static_libs, 1},
      {"sorted", 0, &sorted, 1},
      {"unsorted", 0, &sorted, 0},
      {"atleast-pkgconfig-version", 1, NULL, ATLEAST_PKGCONFIG_VERSION},
      /* {"host", 1, NULL, 'm'},
       {"prefix", 1, NULL, 'p'},*/
      {"debug", 0, NULL, 'd'},
      {0, 0, 0, 0},
  };

  errmsg_iam(path_basename(argv[0]));
  strlist_init(&output, ' ');
  strlist_init(&args, '\0');
  strlist_fromv(&args, (const char**)argv, argc);

#if DEBUG_OUTPUT
  buffer_puts(buffer_2, "Arguments: ");
  buffer_putsl(buffer_2, &args, ", ");
  buffer_putnlflush(buffer_2);
#endif

  byte_zero(&cmd, sizeof(cmd));

  pkgcfg_init(argv[0]);

  if((pkgcfg_path = env_get("PKG_CONFIG_PATH")))
    pkgcfg_setpath(pkgcfg_path);
  unix_opterr = 0;
  unix_optbuf = buffer_1;

  for(;;) {
    c = unix_getopt_long(argc, argv, "dhm:ilLp:aPSvV:F", opts, &index);

    if(c == -1 || unix_opterr /* || argv[unix_optind] == 0 */)
      break;

    if(c == 0)
      continue;

    if(tolower(opts[index].val) == 'l')
      c = opts[index].val;

    switch(c) {
      case ATLEAST_PKGCONFIG_VERSION: atleast_version = 1; break;
      case 'm': {
        stralloc_copys(&cmd.host, unix_optarg);
        break;
      }
      case 'p': {
        stralloc_copys(&cmd.prefix, unix_optarg);
        break;
      }
      case 'd': debug++; break;
      case 'h': usage(argv[0]); return 0;
      case 'v': verbose++; break;
      case 'V': {
        show_version = 1;
        goto getopt_end;
      }
      case VARIABLE: {
        add_cmd(VARIABLE);
        variable = unix_optarg;
        break;
      }
      case 'F':
      case 'L':
      case 'l': add_cmd(c == 'F' ? LIST_FILE : c == 'l' ? LIST_ALL : LIST_PATH); break;
      case LIBS_ONLY_L:
      case LIBS_ONLY_LIBPATH:
      case LIBS_ONLY_OTHER:
        add_cmd(PRINT_LIBS);
        libs_mode = c;
        break;
      case CFLAGS_ONLY_I:
      case CFLAGS_ONLY_OTHER:
        add_cmd(PRINT_CFLAGS);
        cflags_mode = c;
        break;

      case PRINT_VERSION:
      case PRINT_CFLAGS:
      case PRINT_LIBS:
      case PRINT_PATH: add_cmd(c); break;

      case MATCH: match_pattern = argv[unix_optind]; break;

      case 'P': mode |= PKGCFG_PRINT_ERR; break;
      case 'S': mode |= PKGCFG_SHORT_ERR; break;
      case 'E': mode |= PKGCFG_EXISTS; break;
      case '?': {
        const char* arg = argv[unix_optind];

        if(!str_diffn(arg, "--libs", 6) || !str_diffn(arg, "--cflags", 8)) {
          int i = arg[2] == 'l' ? PRINT_LIBS : PRINT_CFLAGS;
          add_cmd(i);

          if(i == PRINT_LIBS)
            libs_mode = arg[str_find(arg, "only")] ? (arg[str_find(arg, "other")] ? LIBS_ONLY_OTHER : (arg[str_find(arg, "L")] ? LIBS_ONLY_LIBPATH : LIBS_ONLY_L)) : 0;
          else
            cflags_mode = arg[str_find(arg, "only")] ? (arg[str_find(arg, "other")] ? CFLAGS_ONLY_OTHER : CFLAGS_ONLY_I) : 0;
          //   argv[unix_optind] = "-";      for(i = unix_optind; argv[i];
          //   i++) argv[i] = argv[i+1];
          continue;
        }
        // unix_optind++;

        goto getopt_end;
      }

      default:
        buffer_puts(buffer_2, "WARNING: Invalid argument -");
        buffer_putc(buffer_2, isprint(c) ? c : '?');
        buffer_putm_internal(buffer_2, " '", unix_optarg ? unix_optarg : argv[unix_optind], "'", NULL);
        buffer_putnlflush(buffer_2);
        usage(argv[0]);
        return 1;
        break;
    }
  }
getopt_end:

  if(pkgcfg_path == 0) {
    stralloc_nul(&cmd.prefix);
    pkgcfg_setpath(0);
  }

  if(argv[unix_optind - 1] && str_equal(argv[unix_optind - 1], "--list-all")) {
    add_cmd(LIST_ALL);
  }

  if(show_version) {
    buffer_puts(buffer_1, "1.0");
    buffer_putnlflush(buffer_1);
    return 0;
  }

  if(atleast_version) {
    return 0;
  }

  if(libs_mode)
    add_cmd(PRINT_LIBS);

  if(cflags_mode)
    add_cmd(PRINT_CFLAGS);

  sysroot = env_get("PKG_CONFIG_SYSROOT");

  if(!sysroot) {
    size_t pos;
    stralloc_nul(&cmd.prefix);

    if((pos = stralloc_finds(&cmd.prefix, "/sys-root/")) < cmd.prefix.len || (pos = stralloc_finds(&cmd.prefix, "/sysroot/")) < cmd.prefix.len) {
      sysroot = str_ndup(cmd.prefix.s, pos + 9);
    } else {
      sysroot = "";
    }
  }

  if(verbose) {
    buffer_putm_internal(buffer_2, path_basename(argv[0]), ": ", NULL);
    buffer_puts(buffer_2, "PKG_CONFIG_PATH is\n  ");
    strlist_dump(buffer_2, &cmd.path);
    buffer_putnlflush(buffer_2);
  }

  if(mode & PKGCFG_EXISTS) {
    add_cmd(CHECK_EXISTS);
  } else if(array_empty(&cmds) && !(mode & PKGCFG_EXISTS)) {

    buffer_puts(buffer_2, "Must specify package names on the command line");
    buffer_putnlflush(buffer_2);
    error_exit(1);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "argc: ");
  buffer_putulong(buffer_2, argc);
  buffer_puts(buffer_2, "\nunix_optind: ");
  buffer_putulong(buffer_2, unix_optind);
  buffer_puts(buffer_2, "\nargv[unix_optind]: ");
  buffer_puts(buffer_2, argv[unix_optind]);
  buffer_putnlflush(buffer_2);
#endif

#ifdef DEBUG_OUTPUT_
  cmd_nul(&cmd);
  buffer_puts(buffer_2, "Dump cmd:\n");
  cmd_dump(&cmd, buffer_2);
#endif

  strarray_from_argv(argc - unix_optind, (const char* const*)argv + unix_optind, &modules);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "Dump cmd:\n");
  cmd_dump(&cmd, buffer_2);
#endif

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Modules(");
  buffer_putulong(buffer_2, strarray_size(&modules));
  buffer_puts(buffer_2, "): ");
  buffer_putstra(buffer_2, &modules, " ");
  buffer_putnlflush(buffer_2);
#endif

  array_foreach_t(&cmds, code) {
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Command: ");
    buffer_putulong(buffer_2, *code);
    buffer_putnlflush(buffer_2);
#endif

    if(*code == LIST_ALL || *code == LIST_PATH || *code == LIST_FILE) {
      pkg_list(*code);
    } else if(unix_optind < argc) {
      int exitCode;

      exitCode = pkg_conf(&modules, *code, mode);

      if(exitCode)
        error_exit(exitCode);
    }
  }

  if(output.sa.s) {
    buffer_putsa(buffer_1, &output.sa);
    buffer_putnlflush(buffer_1);
  }
  return 0;
}
