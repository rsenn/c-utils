#include "lib/windoze.h"
#include "lib/buffer.h"
#include "lib/getopt.h"
#include "lib/hmap.h"
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
  stralloc cmd;
} rule_t;

static strarray srcs;
static stralloc compile_command, lib_command, link_command;
static const char* objext = OBJEXT_DEFAULT;
static const char* libext = LIBEXT_DEFAULT;
static const char* exeext = EXEEXT_DEFAULT;

static HMAP_DB* rules;

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
     }
     mmap_unmap(m, n);
     return ret;
  }
  return -1;
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

  if(rule->cmd.len) {
    buffer_puts(b, "\n\t");
    buffer_putsa(b, &rule->cmd);
  }

  buffer_putnlflush(b);
}

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
compile_rules(buffer* b, strarray* sources) {
  char** srcfile;
  stralloc obj;
  stralloc_init(&obj);

  strarray_foreach(sources, srcfile) {
    rule_t* rule;

    c_to_o(*srcfile, &obj);


    if((rule = get_rule(obj.s))) {

      strlist_push(&rule->deps, *srcfile);

      stralloc_copy(&rule->cmd, &compile_command);
      stralloc_catc(&rule->cmd, ' ');
      stralloc_copys(&rule->cmd, *srcfile);
      stralloc_catc(&rule->cmd, ' ');
      stralloc_cat(&rule->cmd, &rule->deps.sa);

      output_rule(b, rule);
    }
  }

  stralloc_free(&obj);
}

/**
 * Generate compile rules for every library given
 */
void
lib_rules(buffer* b, HMAP_DB* srcdirs) {
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

      stralloc_copy(&rule->cmd, &lib_command);
      stralloc_catc(&rule->cmd, ' ');
      stralloc_cat(&rule->cmd, &libname);
      stralloc_catc(&rule->cmd, ' ');
      stralloc_cat(&rule->cmd, &rule->deps.sa);

      output_rule(b, rule);
    }
  }

  stralloc_free(&libname);
  stralloc_free(&objname);
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
                           {"exeext", 0, NULL, 'E'},
                           {"libext", 0, NULL, 'L'},
                           {"create-libs", 0, &cmd_libs, 'l'},
                           {"create-objs", 0, &cmd_objs, 'o'},
                           {"create-bins", 0, &cmd_bins, 'b'},
                           {0}};

  for(;;) {
    c = getopt_long(argc, argv, "h", opts, &index);
    if(c == -1) break;
    if(c == 0) continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'O': objext = optarg; break;
      case 'E': exeext = optarg; break;
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

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c");
  stralloc_copys(&lib_command, "$(LIB)");
  stralloc_copys(&link_command, "$(LINK) $(LDFLAGS)");

  while(optind < argc) {
    HMAP_DB* sourcedirs;

    /*    strarray srcdirs;
        strarray_init(&srcdirs);
    */
    get_sources(argv[optind], &srcs);

    hmap_init(1024, &sourcedirs);

    //  strarray_transform(&srcs, &srcdirs, &dirname_alloc);

    if(cmd_objs)
      compile_rules(buffer_1, &srcs);

    populate_sourcedirs(&srcs, sourcedirs);

    //    dump_sourcedirs(buffer_1, sourcedirs);

    lib_rules(buffer_1, sourcedirs);
    // strarray_dump(buffer_1, &srcdirs);

    hmap_destroy(&sourcedirs);

    ++optind;
  }
  /*strarray_dump(buffer_1, &srcs);

    strarray_transform(&srcs, c_to_o);
   strarray_removesuffixs(&srcs, ".c");
    strarray_appends(&srcs, ".o");

    strarray_joins(&srcs, &sa, " ");
    buffer_putsa(buffer_1, &sa);
    buffer_putnlflush(buffer_1);*/

  return 0;
}