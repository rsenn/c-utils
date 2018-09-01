#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/cbmap.h"
#include "lib/dir.h"
#include "lib/errmsg.h"
#include "lib/getopt.h"
#include "lib/path.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/iterator.h"
#include "lib/algorithm.h"
#include <ctype.h>
#include <stdlib.h>
#include <wordexp.h>

typedef enum {
  PRINT_VERSION = 1,
  PRINT_CFLAGS,
  PRINT_LIBS,
  PRINT_REQUIRES,
  PRINT_PATH,
  LIST_ALL,
} id;

static struct {
  id code;
  strlist path;
  stralloc self;
} cmd;

typedef struct pkg_s {
  stralloc name;
  cbmap_t vars;
  cbmap_t fields;
} pkg;

static const char* const field_names[] = {
    "Version",
    "Cflags",
    "Libs",
    "Requires",
};

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
  cbmap_get(isupper(key[0]) ? pf->fields : pf->vars, (char*)key, str_len(key) + 1, (void**)&v, &len);
  return v;
}

/**
 * @brief wordexp_sa Expands a string
 * @param s          Input string
 * @param sa         Output value
 * @return           1 on sucess, 0 on failure
 */
int
wordexp_sa(const char* s, stralloc* sa) {
  wordexp_t wx;
  char** w;
  int i;

  if(wordexp(s, &wx, WRDE_NOCMD | WRDE_UNDEF)) return 0;

  w = wx.we_wordv;

  for(i = 0; i < wx.we_wordc; ++i) {
    if(sa->len) stralloc_catb(sa, " ", 1);
    stralloc_cats(sa, w[i]);
  }

  wordfree(&wx);

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

  if((s = pkg_get(pf, key)) == NULL) return 0;

  stralloc v;
  stralloc_init(&v);
  stralloc_copys(&v, s);

  for(;;) {
    stralloc_nul(&v);
    if(!wordexp_sa(v.s, out)) return 0;
    if(stralloc_finds(out, "${") == out->len) break;

    stralloc_free(&v);
    v = *out;
    stralloc_init(out);
  }

  return 1;
}

/**
 * @brief pkg_free Frees a package structure
 */
void
pkg_free(pkg* p) {
  cbmap_destroy(&p->fields);
  cbmap_destroy(&p->vars);
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

  p->vars = cbmap_new();
  p->fields = cbmap_new();

  for(;;) {
    int ret;
    char sep, ch;

    while(buffer_peekc(b, &ch) && (ch == ' ' || ch == '\t')) buffer_skipc(b);

    if((ret = buffer_get_new_token_sa(b, &name, ":=\r\n", 4)) == -1) goto fail;
    stralloc_chomp(&name);
    if(name.len > 0 && name.s[0] == '#') continue;
    if(ret == 1 && isspace(name.s[0])) continue;

    if(ret == 0 || name.s[0] == '\0') break;
    if(name.len > 1) {
      sep = name.s[--name.len];
    }

    while(buffer_peekc(b, &ch) && (ch == ' ' || ch == '\t')) buffer_skipc(b);

    stralloc_zero(&value);
    if((ret = buffer_getline_sa(b, &value)) == -1) goto fail;

    if(value.s[0] == '\0') break;

    if(name.len) {
      stralloc_trim(&value, "\r\n\t \0", 5);
      stralloc_nul(&value);
      stralloc_nul(&name);

      cbmap_insert(sep == '=' ? p->vars : p->fields, name.s, name.len + 1, value.s, value.len + 1);
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
  if(value_len && ((char*)value)[value_len - 1] == '\0') --value_len;
  stralloc_catb(&v, value, value_len);
  //  wordexp_sa(value, &v);
  stralloc_nul(&v);

  setenv(key, v.s, 1);

  return 1;
}

/**
 * @brief pkg_set  Set environmen for package
 * @param p        Package structure
 * @return         1 on success, 0 on failure
 */
int
pkg_set(pkg* p) {
  return cbmap_visit_all(p->vars, &visit_set, p);
}

static int
visit_unset(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  unsetenv(key);
  return 1;
}

/**
 * @brief pkg_unset  Unset environment for package
 * @param p        Package structure
 * @return         1 on success, 0 on failure
 */
int
pkg_unset(pkg* p) {
  return cbmap_visit_all(p->vars, &visit_unset, p);
}

static int
visit_dump(const void* key, size_t key_len, const void* value, size_t value_len, void* user_data) {
  buffer_put(user_data, key, key_len - 1);
  buffer_puts(user_data, isupper(((char*)key)[0]) ? ": " : "=\"");
  buffer_put(user_data, value, value_len - 1);
  buffer_puts(user_data, isupper(((char*)key)[0]) ? "" : "\"");
  buffer_putnlflush(user_data);
  return 1;
}

/**
 * @brief pkg_dump Dumps a package
 * @param b        Output buffer
 * @param pf       Package structure
 */
void
pkg_dump(buffer* b, pkg* pf) {
  buffer_putsa(b, &pf->name);
  buffer_putnlflush(b);

  cbmap_visit_all(pf->vars, visit_dump, b);
  cbmap_visit_all(pf->fields, visit_dump, b);
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
  slist pkgs;
  stralloc path, line;
  int i, n = strlist_count(&cmd.path);

  slist_init(&pkgs);

  stralloc_init(&path);
        stralloc_init(&line);

  for(i = 0; i < n; ++i) {
    const char* entry;
    dir_t d;

    path = strlist_at_sa(&cmd.path, i);
    size_t len = path.len;
    stralloc_nul(&path);
    dir_open(&d, path.s);

    while((entry = dir_read(&d))) {

      stralloc_catm(&path, "/", entry);

      if(stralloc_endb(&path, ".pc", 3)) {

        stralloc line;
        buffer pc;
        pkg pf;

        stralloc_nul(&path);
        pkg_init(&pf, path.s);

        if(!buffer_mmapread(&pc, path.s)) {
          path.len -= 3;
          stralloc_nul(&path);

          stralloc_copys(&line, path_basename(path.s));

          if(pkg_read(&pc, &pf)) {
            const char* desc;

            if((desc = pkg_get(&pf, "Description"))) {
              stralloc_cats(&line, " - ");
              stralloc_cats(&line, desc);
            }
          }

          stralloc_nul(&line);

          buffer_putsa(buffer_1, &line);
          buffer_putnlflush(buffer_1);
          
          slist_pushs(&pkgs, line.s);
          line.s = NULL;
          line.a = 0;

        }

        pkg_free(&pf);
      }

      path.len = len;
    }
  }

  for_each(&pkgs) {

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
  int i, n = strlist_count(&cmd.path);

  stralloc_init(&pf->name);

  for(i = 0; i < n; ++i) {

    stralloc path = strlist_at_sa(&cmd.path, i);

    stralloc_catm(&path, "/", pkgname, ".pc");
    stralloc_nul(&path);

    stralloc_copy(&pf->name, &path);

    if(!buffer_mmapread(&pc, path.s)) break;
  }

  if(pc.x == NULL) return 0;

  pkg_read(&pc, pf);
  return 1;
}

/**
 * @brief pkg_conf Evaluates pkgconf all commands
 * @param pkgname   Package name
 */
int
pkg_conf(strarray* modules) {
  stralloc value;
  stralloc_init(&value);

  for(int i = 0; i < strarray_size(modules); ++i) {
    const char* pkgname = strarray_at(modules, i);
    pkg pf;
    byte_zero(&pf, sizeof(pf));

    if(!pkg_open(pkgname, &pf)) continue;

    if(cmd.code == PRINT_PATH) {
      if(value.len) stralloc_catc(&value, '\n');
      stralloc_cat(&value, &pf.name);
    } else {
      const char* fn = field_names[cmd.code - 1];

      pkg_set(&pf);

      if(!pkg_expand(&pf, fn, &value)) {
        errmsg_warn("Expanding ", pkgname, "::", fn);
        buffer_flush(buffer_1);
        pkg_unset(&pf);
        pkg_free(&pf);
        return 0;
      }
    }

    pkg_unset(&pf);
    pkg_free(&pf);
  }

  buffer_putsa(buffer_1, &value);
  buffer_putnlflush(buffer_1);
  return 1;
}

void
usage(char* progname) {
  buffer_putm(buffer_1, "Usage: ", path_basename(progname), " [OPTIONS] [PACKAGES...]\n");
  buffer_puts(buffer_1, "Options\n");
  buffer_puts(buffer_1, "  --help, -h                        show this help\n");
  buffer_puts(buffer_1, "  --cflags                          print required CFLAGS to stdout\n");
  buffer_puts(buffer_1, "  --libs                            print required linker flags to stdout\n");
  buffer_puts(buffer_1, "  --path                            show the exact filenames for any matching .pc files\n");
  buffer_puts(buffer_1, "  --modversion                      print the specified module's version to stdout\n");
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int c;
  int index = 0;
  struct option opts[] = {
      {"help", 0, NULL, 'h'},
      {"modversion", 0, NULL, PRINT_VERSION},
      {"cflags", 0, NULL, PRINT_CFLAGS},
      {"libs", 0, NULL, PRINT_LIBS},
      {"path", 0, NULL, PRINT_PATH},
      {"list-all", 0, NULL, 'l'},
  };

  for(;;) {
    c = getopt_long(argc, argv, "hmilpa", opts, &index);
    if(c == -1) break;

    switch(c) {
      case 'h':
        usage(argv[0]);
        return 0;
      case PRINT_VERSION:
      case PRINT_CFLAGS:
      case PRINT_LIBS:
      case PRINT_PATH:
        if(!cmd.code) cmd.code = c;
        break;
      case 'l':
        if(!cmd.code) cmd.code = LIST_ALL;
        break;
      default:
        usage(argv[0]);
        return 1;
    }
  }

  path_readlink("/proc/self/exe", &cmd.self);

  strlist_froms(&cmd.path, getenv("PKG_CONFIG_PATH"), ':');

  if(strlist_count(&cmd.path) == 0) {
    stralloc prefix;
    stralloc_init(&prefix);
    stralloc_copy(&prefix, &cmd.self);
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

#ifdef PKGCONF_DEBUG
  buffer_putm(buffer_2, path_basename(argv[0]), ": ");
  buffer_puts(buffer_2, "PKG_CONFIG_PATH is ");
  buffer_putsa(buffer_2, &cmd.path.sa);
  buffer_putnlflush(buffer_2);
#endif

  if(cmd.code == LIST_ALL) {
    pkg_list();
    return 0;

  } else {
    strarray modules;
    strarray_from_argv(argc - optind, &argv[optind], &modules);

    return !pkg_conf(&modules);
  }
}
