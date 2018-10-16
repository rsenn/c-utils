#include "lib/windoze.h"
#include "lib/buffer.h"
#include "lib/getopt.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/path.h"
#include "lib/rdir.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/strarray.h"
#include "lib/strlist.h"

#if WINDOWS
#define OBJEXT_DEFAULT ".obj"
#define LIBEXT_DEFAULT ".lib"
#define EXEEXT_DEFAULT ".exe"
#else
#define OBJEXT_DEFAULT ".o"
#define LIBEXT_DEFAULT ".a"
#define EXEEXT_DEFAULT ""
#endif

typedef struct {
  struct slink link;
  const char* name;
} sourcefile_t;

typedef struct {
  int n_sources;
  struct slink* sources;
} sourcedir_t;

typedef struct {
  const char* name;
  strlist deps;
  stralloc* cmd;
} rule_t;

static strarray srcs;
static stralloc compile_command, lib_command, link_command;
static const char* objext = OBJEXT_DEFAULT;
static const char* libext = LIBEXT_DEFAULT;
static const char* binext = EXEEXT_DEFAULT;

static HMAP_DB* rules;

/**
 * Checks if the given source file contains a main() function
 */
int
has_main(const char* filename) {
  char* m;
  size_t i, n;
  if((m = mmap_read(filename, &n))) {
    size_t i = 0;
    int ret = 0;
    do {
      i += byte_finds(m + i, n - i, "main");
      if(i == n) break;
      i += 4;
      if(!isspace(m[i - 5])) continue;
      i += scan_whitenskip(m + i, n - i);
      if(i == n) break;
      if(m[i] == '(') {
        ret = 1;
        break;
      }
    } while(i < n);
    mmap_unmap(m, n);
    return ret;
  }
  return -1;
}

/**
 * Get rule command with substitutions
 */
void
rule_command(rule_t* rule, stralloc* out) {
  size_t i;
  stralloc* in = rule->cmd;

  for(i = 0; i < in->len; ++i) {
    const char* p = &in->s[i];

    if(i + 2 <= in->len && *p == '$' && str_chr("@^<", p[1]) < 3) {
      switch(p[1]) {
      case '@': {
        stralloc_cats(out, rule->name);
        break;
      }
      case '^': {
        stralloc_cat(out, &rule->deps.sa);
        break;
      }
      case '<': {
        size_t n;
        const char* s = strlist_at_n(&rule->deps, 0, &n);
        stralloc_catb(out, s, n);
        break;
      }
      }
      ++i;
    } else {
      if(!stralloc_append(out, p)) return 0;
    }
  }
}

/**
 * Find or create rule
 */
rule_t*
get_rule(const char* name) {
  rule_t* ret = NULL;
  TUPLE* t;

  if(rules == NULL) hmap_init(1024, &rules);

  if(hmap_search(rules, name, str_len(name) + 1, &t) == HMAP_SUCCESS) {
    ret = t->vals.val_custom;
  } else {
    ret = malloc(sizeof(rule_t));
    byte_zero(ret, sizeof(rule_t));

    hmap_add(&rules, name, str_len(name) + 1, 0, HMAP_DATA_TYPE_CUSTOM, ret);
    hmap_search(rules, name, str_len(name) + 1, &t);

    ret->name = t->key;

    strlist_init(&ret->deps, ' ');
  }

  return ret;
}

/**
 * Output rule to buffer
 */
void
output_rule(buffer* b, rule_t* rule) {
  buffer_puts(b, rule->name);
  buffer_puts(b, ": ");
  buffer_putsa(b, &rule->deps.sa);

  if(rule->cmd) {
    stralloc cmd;
    stralloc_init(&cmd);

    rule_command(rule, &cmd);

    buffer_puts(b, "\n\t");
    buffer_putsa(b, &cmd);
    buffer_putc(b, '\n');

    stralloc_free(&cmd);
  }

  buffer_putnlflush(b);
}

/**
 * Create new source file entry.
 */
sourcefile_t*
new_source(const char* name) {
  sourcefile_t* ret;

  if((ret = malloc(sizeof(sourcefile_t)))) {
    byte_zero(ret, sizeof(sourcefile_t));
    ret->name = str_dup(name);
    return ret;
  }
  return 0;
}

char*
c_to_o(const char* in, stralloc* out) {
  stralloc_zero(out);

  stralloc_copys(out, str_basename(in));

  if(stralloc_endb(out, ".c", 2)) out->len -= 2;

  stralloc_cats(out, objext);
  stralloc_nul(out);

  return out->s;
}

char*
dirname_alloc(const char* p) {
  size_t len = str_len(p);
  size_t pos = str_rchrs(p, "\\/", 2);

  if(pos < len) return str_ndup(p, pos);

  return str_dup(".");
}

/**
 * Adds a source file to the given list.
 */
void
add_source(const char* filename, strarray* sources) {
  if(str_end(filename, ".c")) {
    strarray_push(sources, filename);
  }
}

/**
 * Searches all source files in the given directory and creates a string-array.
 */
void
get_sources(const char* basedir, strarray* sources) {
  rdir_t rdir;

  if(!rdir_open(&rdir, basedir)) {
    const char* s;

    while((s = rdir_read(&rdir)))
      add_source(s, sources);
  }
}

/**
  * Creates a hash-map of all source directories
  */
void
populate_sourcedirs(strarray* sources, HMAP_DB* sourcedirs) {
  char** srcfile;
  stralloc dir;
  stralloc_init(&dir);

  strarray_foreach(sources, srcfile) {
    sourcedir_t* srcdir;
    sourcefile_t* pfile = new_source(*srcfile);

    path_dirname(*srcfile, &dir);

    if((srcdir = hmap_get(sourcedirs, dir.s, dir.len + 1))) {
      slist_add_before(&srcdir->sources, &pfile->link, 0);

      ++srcdir->n_sources;
    } else {
      sourcedir_t newdir;
      byte_zero(&newdir, sizeof(newdir));

      newdir.n_sources = 1;
      newdir.sources = &pfile->link;

      hmap_set(&sourcedirs, dir.s, dir.len + 1, &newdir, sizeof(newdir));
    }
  }

  stralloc_free(&dir);
}

void
dump_sourcedirs(buffer* b, HMAP_DB* sourcedirs) {
  TUPLE* t = sourcedirs->list_tuple;

  while(t) {
    sourcedir_t* srcdir = t->vals.val_chars;

    buffer_puts(b, "source dir(");
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): ");
    buffer_put(b, t->key, t->key_len);

    {
      sourcefile_t** pfile;
      slist_foreach(&srcdir->sources, pfile) {
        buffer_putspace(b);
        buffer_puts(b, (*pfile)->name);
      }
    }

    buffer_putnlflush(b);

    t = hmap_next(sourcedirs, t);
  }
}

/**
 * Generate compile rules for every source file given
 */
void
compile_rules(HMAP_DB* rules, strarray* sources) {
  char** srcfile;
  stralloc obj;
  stralloc_init(&obj);

  strarray_foreach(sources, srcfile) {
    rule_t* rule;

    c_to_o(*srcfile, &obj);

    if((rule = get_rule(obj.s))) {
      strlist_push(&rule->deps, *srcfile);

      rule->cmd = &compile_command;
    }
  }

  stralloc_free(&obj);
}

/**
 * Generate compile rules for every source file given
 */
void
link_rules(HMAP_DB* rules, strarray* sources) {
  char** srcfile;
  stralloc obj, bin;
  stralloc_init(&obj);
  stralloc_init(&bin);

   strarray_foreach(sources, srcfile) {
    rule_t* rule;

    if(has_main(*srcfile) != 1) continue;

    c_to_o(*srcfile, &obj);
    stralloc_copy(&bin, &obj);
    if(stralloc_endb(&bin, objext, str_len(objext))) {
      bin.len -= str_len(objext);
      stralloc_cats(&bin, binext);
      stralloc_nul(&bin);
    }

    rule = get_rule("all");
    strlist_push(&rule->deps, bin.s);

    if((rule = get_rule(bin.s))) {
      strlist_push(&rule->deps, obj.s);

      rule->cmd = &link_command;
    }
  }

  stralloc_free(&bin);
  stralloc_free(&obj);
}

/**
 * Generate compile rules for every library given
 */
void
lib_rules(HMAP_DB* rules, HMAP_DB* srcdirs) {
  TUPLE* t;
  stralloc libname, objname;
  stralloc_init(&libname);
  stralloc_init(&objname);

  for(t = hmap_begin(srcdirs); t; t = hmap_next(srcdirs, t)) {
    sourcedir_t* srcdir = t->vals.val_chars;
    const char* base = str_basename(t->key);
    rule_t* rule;

    if(str_equal(base, "lib")) continue;

    stralloc_copys(&libname, base);
    stralloc_cats(&libname, libext);
    stralloc_nul(&libname);

    if((rule = get_rule(libname.s))) {
      sourcefile_t* pfile;
      strlist_init(&rule->deps, ' ');

      slist_foreach(srcdir->sources, pfile) {
        stralloc_copys(&objname, str_basename(pfile->name));

        if(stralloc_endb(&objname, ".c", 2)) objname.len -= 2;

        stralloc_cats(&objname, objext);

        strlist_push_sa(&rule->deps, &objname);
      }

      rule->cmd = &lib_command;
    }
  }

  stralloc_free(&libname);
  stralloc_free(&objname);
}

void
output_all_rules(buffer* b, HMAP_DB* hmap) {
  TUPLE* t;

  for(t = hmap_begin(hmap); t; t = hmap_next(hmap, t)) {
    output_rule(b, t->vals.val_custom);
  }
}

void
strarray_dump(buffer* b, const strarray* arr) {
  char **p = strarray_begin(arr), **e = strarray_end(arr);

  while(p < e) {
    buffer_puts(b, *p);
    buffer_putnlflush(b);
    ++p;
  }
}

void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [sources...]\n",
                       "\n",
                       "Options\n",
                       "  -h, --help                show this help\n",
                       0);
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  stralloc sa;

  static int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
  int argi = 1;
  int c;
  int index = 0;
  struct longopt opts[] = {{"help", 0, NULL, 'h'},
    {"objext", 0, NULL, 'O'},
    {"exeext", 0, NULL, 'B'},
    {"libext", 0, NULL, 'L'},
    {"create-libs", 0, &cmd_libs, 'l'},
    {"create-objs", 0, &cmd_objs, 'o'},
    {"create-bins", 0, &cmd_bins, 'b'},
    {0}
  };

  for(;;) {
    c = getopt_long(argc, argv, "h", opts, &index);
    if(c == -1) break;
    if(c == 0) continue;

    switch(c) {
    case 'h': usage(argv[0]); return 0;
    case 'O': objext = optarg; break;
    case 'B': binext = optarg; break;
    case 'L': libext = optarg; break;
    default: usage(argv[0]); return 1;
    }
  }

  if(!cmd_bins && !cmd_libs && !cmd_objs) {
    cmd_objs = 1;
    cmd_libs = 1;
  }

  strarray_init(&srcs);
  stralloc_init(&sa);

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");
  stralloc_copys(&lib_command, "$(LIB) $@ $^");
  stralloc_copys(&link_command, "$(LINK) $(LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)");

  {
    HMAP_DB* sourcedirs;
    rule_t* rule = get_rule("all");

    hmap_init(1024, &sourcedirs);

    while(optind < argc) {
      if(str_end(argv[optind], ".c"))
        add_source(argv[optind], &srcs);
      else
        get_sources(argv[optind], &srcs);

      ++optind;
    }
    populate_sourcedirs(&srcs, sourcedirs);

    if(cmd_objs) compile_rules(rules, &srcs);
    lib_rules(rules, sourcedirs);

    link_rules(rules, &srcs);

      output_all_rules(buffer_1, rules);

    hmap_destroy(&sourcedirs);
  }

  return 0;
}