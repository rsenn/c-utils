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
#include "map.h"

#include <ctype.h>
#include <spawn.h>
#include <stdlib.h>
#include <sys/wait.h>

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
} id;
typedef enum { OP_EQ = 0, OP_NE, OP_GT, OP_GE, OP_LT, OP_LE } op_code;

typedef enum { LIBS_ONLY_L = 64, LIBS_ONLY_LIBPATH = 128, LIBS_ONLY_OTHER = 256 } libs_mode_t;

typedef enum { CFLAGS_ONLY_I = 512, CFLAGS_ONLY_OTHER = 1024 } cflags_mode_t;

static array cmds;

static struct cmd_s {
  strlist path;
  stralloc self;
  stralloc host;
  stralloc prefix;
} cmd;

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

static const char* const field_names[] = {
    "Version",
    "Cflags",
    "Libs",
    "Requires",
};
static const char* const op_strings[] = {"==", "!=", ">", ">=", "<", "<="};

static const char* sysroot = 0;
static int libs_mode = 0;
static int static_libs = 0;
static int cflags_mode = 0;
static int sorted = 0;
static int verbose = 0;
static int show_version = 0;
static int atleast_version = 0;

void
add_cmd(id cmd) {
  array_catb(&cmds, &cmd, sizeof(id));
}

int
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

int
get_op_index(const stralloc* op) {
  int i;
  for(i = 0; op_strings[i]; i++) {
    if(stralloc_equals(op, op_strings[i]))
      return i;
  }
  return -1;
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

const char*
host_arch(const char* compiler, stralloc* out) {
  pid_t pid;
  int ws = 0;
#ifdef POSIX_SPAWN
  posix_spawn_file_actions_t actions;
  posix_spawnattr_t attr;
#endif
  int p[2];
  char* const argv[] = {(char*)compiler, "-dumpmachine", 0};
  char* const envp[1] = {0};
  const char* bin;
  stralloc dir;
  stralloc_init(&dir);

  stralloc_zero(out);

  if(pipe(p) == -1)
    return NULL;

#ifdef POSIX_SPAWN

  posix_spawnattr_setflags(&attr, 0);
  posix_spawn_file_actions_init(&actions);
  posix_spawn_file_actions_adddup2(&actions, p[1], 1);

  if(posix_spawnp(&pid, compiler, &actions, &attr, argv, envp)) {
    errmsg_warnsys("execvpe error: ", 0);
    exit(1);
  }

#else
  if((pid = fork()) == 0) {
    close(p[0]);

    dup2(p[1], 1);

    if((bin = search_path(env_get("PATH"), compiler, &dir)) == 0) {
      errmsg_warnsys(bin, " not found: ", 0);
      exit(127);
    }

    if(execve(bin, argv, envp)) {
      errmsg_warnsys("execvpe error: ", 0);
      exit(1);
    }
  }
#endif

  close(p[1]);

  wait_pid(pid, &ws);

  readclose_append(p[0], out, 1024);

  stralloc_trimr(out, "\r\t\v\n", 4);
  stralloc_nul(out);
  return out->len ? out->s : 0;
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
    *v |= (num & 0xffffll) << s;
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
  p->version = 0LL;

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
      buffer_putm_internal(buffer_2, "Name: ", name.s, "\n", 0);
      buffer_putm_internal(buffer_2, "Value: ", value.s, "\n", 0);
      buffer_flush(buffer_2);
#endif

      if(stralloc_equals(&name, "Version"))
        pkg_parse_version(&p->version, value.s, value.len);

      {
        MAP_T map = sep == '=' ? p->vars : p->fields;
        MAP_INSERT(map, name.s, name.len + 1, value.s, value.len + 1);
      }

      /*     if(islower(name.s[0]))
            setenv(name.s, value.s, 1);
       */
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
  buffer_putm_internal(buffer_2, "ENV SET ", key, "=", 0);
  buffer_putsa(buffer_2, &v);
  buffer_putnlflush(buffer_2);
#endif
  stralloc_nul(&v);
  env_set(key, v.s);
  /* setenv(key, v.s, 1); */

  return 1;
}

/**
 * @brief pkg_set  Set environmen for
 * package
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
      stralloc_catm_internal(&path, "/", entry, 0);

      if(stralloc_endb(&path, ".pc", 3)) {
        stralloc line;
        buffer pc;
        pkg pf;

        stralloc_init(&line);
        stralloc_nul(&path);
        pkg_init(&pf, path.s);

        if(!buffer_mmapread(&pc, path.s)) {
          stralloc_zero(&line);

#ifdef DEBUG_OUTPUT_
          buffer_puts(buffer_2, "file: ");
          buffer_putsa(buffer_2, &path);
          buffer_putnlflush(buffer_2);
#endif
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

static cond condition;

/**
 * @brief pkg_conf Evaluates pkgcfg all
 * commands
 * @param pkgname   Package name
 */
int
pkg_conf(strarray* modules, id code, int mode) {
  int i, do_cond;
  size_t n, pos;
  const char* x;
  size_t len;
  stralloc name, cond, value;
  strlist require;
  stralloc_init(&name);
  stralloc_init(&cond);
  stralloc_init(&value);
  strlist_init(&require, ' ');

  for(i = 0; i < strarray_size(modules); ++i) {
    const char* pkgname = strarray_at(modules, i);
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "pkgname: ");
    buffer_puts(buffer_2, pkgname);

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
      /*   pkg_dump_cond(&condition);

        stralloc_copyb(&cond,
        &name.s[pos], name.len - pos);
      */
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

    if(!pkg_open(name.s, &pf))
      continue;

    if(do_cond) {
      int r = pkg_check_cond(&condition, &pf);
      if(r == -1) {
      } else if(r == 0) {
        continue;
      }
    }
    if(mode & PKGCFG_EXISTS)
      return 0;

#ifdef DEBUG_OUTPUT_
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

    if(code == PRINT_PATH) {
      if(value.len)
        stralloc_catc(&value, '\n');
      stralloc_cat(&value, &pf.name);
    } else {
      int fi = code != 0 ? get_field_index(code) : -1;
      const char* fn = fi == -1 ? NULL : field_names[fi];

      pkg_set(&pf);
      stralloc_zero(&value);

      if(fn) {

        if(!pkg_expand(&pf, fn, &value)) {
          /*   buffer_flush(buffer_1);
             buffer_flush(buffer_2);
             errmsg_warn("Expanding ",
             pkgname, "::", fn, NULL);*/
          /*     pkg_unset(&pf);
               pkg_free(&pf);
               return 0;*/
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

    pkg_unset(&pf);
    pkg_free(&pf);
  }

  if(!(mode & PKGCFG_EXISTS)) {
    buffer_putsa(buffer_1, &value);
    buffer_putnlflush(buffer_1);
  } else {
    return 1;
  }
  return 0;
}

void
pkgcfg_init(const char* errmsg_argv0, const char* pkgcfg_path) {
  size_t pos;
  const char* x;
  stralloc dir;
  stralloc_init(&dir);

  // path_readlink("/proc/self/exe",
  // &cmd.self);
  stralloc_init(&cmd.self);
  stralloc_init(&cmd.host);
  stralloc_init(&cmd.prefix);

  if(!errmsg_argv0[str_chr(errmsg_argv0, '/')]) {

    if((x = search_path(env_get("PATH"), errmsg_argv0, &dir)))
      errmsg_argv0 = x;
  }

  if(!errmsg_argv0[str_chr(errmsg_argv0, '/')]) {
    path_readlink("/proc/self/exe", &dir);
    stralloc_nul(&dir);
    errmsg_argv0 = dir.s;
  }

  stralloc_copys(&cmd.self, path_basename(errmsg_argv0));

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "errmsg_argv0: ");
  buffer_puts(buffer_2, errmsg_argv0);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "self: ");
  buffer_putsa(buffer_2, &cmd.self);
  buffer_putnlflush(buffer_2);
#endif

  path_dirname(errmsg_argv0, &cmd.prefix);

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

  strlist_init(&cmd.path, ':');

  if(pkgcfg_path) {
    strlist_froms(&cmd.path, pkgcfg_path, ':');

  } else {
    if(sysroot)
      stralloc_copys(&dir, sysroot);
    else
      stralloc_copy(&dir, &cmd.prefix);

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
  buffer_putm_internal(buffer_1, "Usage: ", path_basename(progname), " [OPTIONS] [PACKAGES...]\n", 0);
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1,
              "  --help, -h                    "
              "    show this help\n");
  buffer_puts(buffer_1,
              "  --cflags                      "
              "    print required CFLAGS to "
              "stdout\n");
  buffer_puts(buffer_1,
              "  --libs                        "
              "    print required linker flags "
              "to stdout\n");
  buffer_puts(buffer_1,
              "  --path                        "
              "    show the exact filenames "
              "for any matching .pc files\n");
  buffer_puts(buffer_1,
              "  --modversion                  "
              "    print the specified "
              "module's version to stdout\n");
  buffer_puts(buffer_1,
              "  --list-all                    "
              "    list all known packages\n");
  buffer_puts(buffer_1,
              "  --list-path                   "
              "    list path of all known "
              "packages\n");
  buffer_puts(buffer_1,
              "  --verbose                     "
              "    increase verbosity\n");
  buffer_puts(buffer_1,
              "  --debug                       "
              "    show verbose debug "
              "information\n");
  buffer_puts(buffer_1,
              "  --sorted                      "
              "    sorted list output\n");
  buffer_puts(buffer_1,
              "  --unsorted                    "
              "    unsorted list output "
              "(default)\n");
  buffer_putnlflush(buffer_1);
}

extern buffer* optbuf;
static strlist args;

int
main(int argc, char* argv[]) {
  int c;
  id* code;
  int index = 0;
  int mode = 0;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"version", 0, &show_version, 1},
      {"modversion", 0, NULL, PRINT_VERSION},
      {"cflags", 0, NULL, PRINT_CFLAGS},
      {"path", 0, NULL, PRINT_PATH},
      {"verbose", 0, NULL, 'v'},
      {"debug", 0, &verbose, 2},
      {"variable", 1, NULL, 'V'},
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
      {"static", 0, &static_libs, 1},
      {"sorted", 0, &sorted, 1},
      {"unsorted", 0, &sorted, 0},
      {"atleast-pkgconfig-version", 1, NULL, ATLEAST_PKGCONFIG_VERSION},

      /*   {"atleast", 0, NULL, 0},
          {"atleast-pkgconfig-version",
         0, NULL, 0},
          {"atleast-version", 0, NULL,
         0},
            {"debug", 0, NULL, 0},
          {"define-prefix", 0, NULL, 0},
          {"define-variable", 0, NULL,
         0},
          {"dont-define-prefix", 0,
         NULL, 0},
          {"errors-to-stdout", 0, NULL,
         0},
          {"exact-version", 0, NULL, 0},

          {"max-version", 0, NULL, 0},
          {"prefix-variable", 0, NULL,
         0},
          {"print-provides", 0, NULL,
         0},
          {"print-requires", 0, NULL,
         0},
          {"print-requires-private", 0,
         NULL, 0},
          {"print-variables", 0, NULL,
         0},
          {"silence-errors", 0, NULL,
         0},
          {"uninstalled", 0, NULL, 0},
          {"validate", 0, NULL, 0}, */

      {0, 0, 0, 0},
  };

  errmsg_iam(argv[0]);
  strlist_init(&args, '\t');
  strlist_fromv(&args, (const char**)argv, argc);

#if DEBUG_OUTPUT_
  buffer_puts(buffer_2, "pkgcfg args = ");
  buffer_putsa(buffer_2, &args.sa);
  buffer_putnlflush(buffer_2);
#endif

#ifdef _MSC_VER
  optbuf = buffer_1;
#endif
  opterr = 0;

  for(;;) {
    c = getopt_long(argc, argv, "hmilLpaPSvV:F", opts, &index);
    if(c == -1 || opterr /* || argv[optind] == 0 */)
      break;
    if(c == 0)
      continue;

    if(tolower(opts[index].val) == 'l')
      c = opts[index].val;

    switch(c) {

      case ATLEAST_PKGCONFIG_VERSION: atleast_version = 1; break;

      case 'h': usage(argv[0]); return 0;
      case 'v': verbose++; break;
      case 'V': {
        show_version = 1;
        goto getopt_end;
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

      case 'P': mode |= PKGCFG_PRINT_ERR; break;
      case 'S': mode |= PKGCFG_SHORT_ERR; break;
      case 'E': mode |= PKGCFG_EXISTS; break;
      case '?': {

        const char* arg = argv[optind];

        if(!str_diffn(arg, "--libs", 6) || !str_diffn(arg, "--cflags", 8)) {
          int i = arg[2] == 'l' ? PRINT_LIBS : PRINT_CFLAGS;
          add_cmd(i);
          if(i == PRINT_LIBS)
            libs_mode = arg[str_find(arg, "only")] ? (arg[str_find(arg, "other")] ? LIBS_ONLY_OTHER : (arg[str_find(arg, "L")] ? LIBS_ONLY_LIBPATH : LIBS_ONLY_L)) : 0;
          else
            cflags_mode = arg[str_find(arg, "only")] ? (arg[str_find(arg, "other")] ? CFLAGS_ONLY_OTHER : CFLAGS_ONLY_I) : 0;

          argv[optind] = "-";
          /*           for(i = optind;
             argv[i]; i++) argv[i] =
             argv[i+1];
           */
          continue;
        }

        goto getopt_end;
      }

      default:
        buffer_puts(buffer_2,
                    "WARNING: Invalid "
                    "argument -");
        buffer_putc(buffer_2, isprint(c) ? c : '?');
        buffer_putm_internal(buffer_2, " '", optarg ? optarg : argv[optind], "'", NULL);
        buffer_putnlflush(buffer_2);
        usage(argv[0]);
        return 1;
        break;
    }
  }
getopt_end:

  if(argv[optind - 1] && str_equal(argv[optind - 1], "--list-all")) {
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

  pkgcfg_init(argv[0], env_get("PKG_CONFIG_PATH"));

  if(!sysroot)
    sysroot = "";

  // strlist_froms(&cmd.path,
  // pkgcfg_path, ':');
  /*
    if(strlist_count(&cmd.path) == 0) {

      {
        size_t len =
    stralloc_finds(&prefix, "/bin");

        if(len == prefix.len) {
          stralloc_copys(&prefix,
    "/usr"); len = prefix.len;
        }
        prefix.len = len;
        stralloc_cats(&prefix,
    "/lib/pkgconfig");
        strlist_push_sa(&cmd.path,
    &prefix); prefix.len = len;
        stralloc_cats(&prefix,
    "/share/pkgconfig");
        strlist_push_sa(&cmd.path,
    &prefix);
      }
    } */

  if(verbose) {
    buffer_putm_internal(buffer_2, path_basename(argv[0]), ": ", 0);
    buffer_puts(buffer_2, "PKG_CONFIG_PATH is\n  ");
    strlist_dump(buffer_2, &cmd.path);
    buffer_putnlflush(buffer_2);
  }
  if(mode & PKGCFG_EXISTS) {
    add_cmd(CHECK_EXISTS);
  } else if(array_empty(&cmds) && !(mode & PKGCFG_EXISTS)) {
    buffer_puts(buffer_2,
                "Must specify package names on "
                "the command line");
    buffer_putnlflush(buffer_2);
    return 1;
  }

  array_foreach_t(&cmds, code) {

    if(*code == LIST_ALL || *code == LIST_PATH || *code == LIST_FILE) {
      pkg_list(*code);
    } else if(optind < argc) {
      strarray modules;
      strarray_from_argv(argc - optind, (const char* const*)&argv[optind], &modules);
      return pkg_conf(&modules, *code, mode);
    }
  }
}
