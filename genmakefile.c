#include "lib/getopt.h"
#include "lib/windoze.h"
#include "lib/buffer.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/path.h"
#include "lib/rdir.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/scan.h"

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
  slink* sources;
} sourcedir_t;

typedef struct {
  const char* name;
  strlist deps;
  stralloc* cmd;
} rule_t;

typedef void (linklib_fmt)(const char*, stralloc*);

static strarray srcs;
static stralloc compile_command, lib_command, link_command, mkdir_command;
static const char* objext = OBJEXT_DEFAULT;
static const char* libext = LIBEXT_DEFAULT;
static const char* binext = EXEEXT_DEFAULT;
static stralloc builddir;

static HMAP_DB *rules, *vars;

static linklib_fmt* format_linklib_fn;

/**
 * Output library name (+".lib")
 */
void
format_linklib_lib(const char* libname, stralloc* out) {
  stralloc_cats(out, libname);
  stralloc_cats(out, libext);
}

/**
 * Output library name (+ leading "-l")
 */
void
format_linklib_switch(const char* libname, stralloc* out) {
  stralloc_cats(out, "-l");
  stralloc_cats(out, libname);
}

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
      if(!stralloc_append(out, p))
        break;
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

rule_t*
get_rule_sa(stralloc* name) {
  stralloc_nul(name);
  return get_rule(name->s);
}

/**
 * Find or create variable
 */
strlist*
get_var(const char* name) {
  TUPLE* t;

  if(vars == NULL) hmap_init(1024, &vars);

  if(hmap_search(vars, name, str_len(name) + 1, &t) != HMAP_SUCCESS) {
    strlist var;
    strlist_init(&var, ' ');

    hmap_set(&vars, name, str_len(name) + 1, &var, sizeof(strlist));
    hmap_search(vars, name, str_len(name) + 1, &t);
  }

  return (strlist*)t->vals.val_chars;
}

/**
 * Set variable
 */
void
set_var(const char* name, const char* value) {
  strlist* var = get_var(name);

  stralloc_zero(&var->sa);
  stralloc_copys(&var->sa, value);
}

/**
 * Add value to variable
 */
void
push_var(const char* name, const char* value) {
  strlist* var = get_var(name);

  strlist_push_unique(var, value);
}

/**
 * Add library spec to variable
 */
void
push_lib(const char* name, const char* lib) {
  strlist* var = get_var(name);

  if(var->sa.len)
    stralloc_catc(&var->sa, var->sep);

  format_linklib_fn(lib, &var->sa);
}

/**
 * Search rules by command
 */
void
get_rules_by_cmd(stralloc* cmd, strlist* deps) {
  TUPLE* t;

  hmap_foreach(rules, t) {
    rule_t* rule = t->vals.val_custom;

    if(rule->cmd == cmd) {
      strlist_push(deps, rule->name);
    }
  }
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

/**
 * Change file extension and concatenate it to out.
 */
char*
change_ext(const char* in, stralloc* out, const char* ext) {
  size_t extpos = str_rchr(in, '.');

  stralloc_catb(out, in, extpos);
  stralloc_cats(out, ext);
  stralloc_nul(out);
  return out->s;
}

/**
 * Convert source file name to object file name
 */
char*
src_to_obj(const char* in, stralloc* out) {
  stralloc_zero(out);

  stralloc_cat(out, &builddir);

  return change_ext(str_basename(in), out, objext);
}

/**
 * Gets directory name from a file path (allocated).
 */
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
      slist_add(&srcdir->sources, &pfile->link);

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
  TUPLE* t;

  hmap_foreach(sourcedirs, t) {
    sourcedir_t* srcdir = hmap_data(t);
    sourcefile_t* pfile;

    buffer_puts(b, "source dir(");
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): ");
    buffer_put(b, t->key, t->key_len);

    slist_foreach(srcdir->sources, pfile) {
      buffer_putspace(b);
      buffer_puts(b, pfile->name);
    }

    buffer_putnlflush(b);
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

    src_to_obj(*srcfile, &obj);

    if((rule = get_rule_sa(&obj))) {
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

    src_to_obj(*srcfile, &obj);

    stralloc_zero(&bin);
    change_ext(str_basename(*srcfile), &bin, binext);

    rule = get_rule("all");
    strlist_push_sa(&rule->deps, &bin);

    if((rule = get_rule_sa(&bin))) {
      strlist_push_sa(&rule->deps, &obj);

      get_rules_by_cmd(&lib_command, &rule->deps);

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

  hmap_foreach(srcdirs, t) {
    sourcedir_t* srcdir = hmap_data(t);
    const char* base = str_basename(t->key);
    rule_t* rule;

    if(str_equal(base, "lib") || base[0] == '\0') continue;

    stralloc_zero(&libname);
    stralloc_cat(&libname, &builddir);
    stralloc_cats(&libname, base);
    stralloc_cats(&libname, libext);

    if((rule = get_rule_sa(&libname))) {
      sourcefile_t* pfile;
      strlist_init(&rule->deps, ' ');

      slist_foreach(srcdir->sources, pfile) {
        src_to_obj(pfile->name, &objname);

        strlist_push_sa(&rule->deps, &objname);
      }

      rule->cmd = &lib_command;
    }
  }

  stralloc_free(&libname);
  stralloc_free(&objname);
}

/**
 * Output all variables
 */
void
output_all_vars(buffer* b, HMAP_DB* vars) {
  TUPLE* t;

  hmap_foreach(vars, t) {
    strlist* var = hmap_data(t);

    buffer_puts(b, t->key);
    buffer_puts(b, " = ");
    buffer_putsa(b, &var->sa);
    buffer_putc(b, '\n');
  }
  buffer_putnlflush(b);
}

/**
 * Output the rule set
 */
void
output_all_rules(buffer* b, HMAP_DB* hmap) {
  TUPLE* t;

  hmap_foreach(hmap, t) {
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

/**
 * Show command line usage
 */
void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [sources...]\n",
                       "\n",
                       "Options\n",
                       "  -h, --help                show this help\n",
                       "  -O, --objext EXT          object file extension\n",
                       "  -B, --exeext EXT          binary file extension\n",
                       "  -L, --libext EXT          library file extension\n",
                       "  -l, --create-libs         create rules for libraries\n",
                       "  -o, --create-objs         create rules for objects\n",
                       "  -b, --create-bins         create rules for programs\n",
                       "  -d, --builddir            build directory\n",
                       "  -t, --type   TYPE         makefile type, one of:\n"
                       "\n"
                       "     gnu         GNU make\n"
                       "     bcc55       Borland C++ Builder 5.5\n"
                       "     bcc32       Borland C++ Builder new\n"
                       "     lcc         lcc make\n"
                       "     msvc        Visual C++ NMake\n"
                       "\n",
                       0);
  buffer_putnlflush(buffer_1);
}

/**
 * Set the build type
 */
int
set_type(const char* type) {

  push_var("CC", "cc");
  push_var("CXX", "c++");

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");

  push_var("CFLAGS", "-O2");
  push_var("DEFS", "-DHAVE_ERRNO_H=1");

  push_lib("EXTRA_LIBS", "ws2_32");
  push_lib("EXTRA_LIBS", "iphlpapi");

  stralloc_copys(&builddir, "build\\");
  stralloc_cats(&builddir, type);
  stralloc_cats(&builddir, "\\");

  stralloc_copys(&mkdir_command, "if not exist \"$@\" mkdir \"$@\"");

  if(str_equal(type, "gnu")) {

    /*
     * GNU GCC compatible compilers
     */

    set_var("CC", "gcc");
    set_var("CXX", "g++");

    set_var("AR", "ar");

    stralloc_copys(&lib_command, "$(AR) rcs $@ $^");
    stralloc_copys(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)");

  } else if(str_equal(type, "msvc")) {

      set_var("CC", "cl");
      set_var("LIB", "lib");
      set_var("LINK", "link");

      stralloc_copys(&link_command, "$(LINK) /OUT:$@ @<<\n\t\t$(LDFLAGS) $^ $(LIBS) $(EXTRA_LIBS)\n<<");
      stralloc_copys(&lib_command, "$(LIB) /OUT:$@ @<<\n\t\t$^\n<<");

 } else if(str_equal(type, "bcc55") || str_equal(type, "bcc32")) {

    /*
     * Borland C++ Builder
     */

    push_var("DEFS", "-DWIN32_LEAN_AND_MEAN");

    if(str_equal(type, "bcc32")) {
      set_var("CC", "bcc32c");
      set_var("CXX", "bcc32x");

      stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -o $@ @&&|\n$^ $(LIBS) $(EXTRA_LIBS)\n|");
    } else {
      set_var("CC", "bcc32");
      set_var("CXX", "bcc32");

     stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
     stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -e$@ @&&|\n$^ $(LIBS) $(EXTRA_LIBS)\n|");
    }
    set_var("LINK", "ilink32");
    set_var("LIB", "tlib");

    push_var("CFLAGS", "-q -tWC -tWM -O1 -ff -fp");

    push_var("CPPFLAGS", "-Dinline=__inline");

    push_var("LIBS", "cw32.lib import32.lib");

    stralloc_copys(&lib_command, "$(LIB) @&&|\n\t/a /u \"$@\" $^\n|");

  } else if(str_equal(type, "lcc")) {
    /*
     * LCC compiler
     */

    set_var("CC", "lcc");

  } else if(str_equal(type, "tcc")) {
    /*
     * Tiny CC compiler
     */

    set_var("CC", "tcc");

  } else {
    return 0;
  }
  return 1;
}

int
main(int argc, char* argv[]) {
  const char* type = "gnu";
  static int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
  int c;
  int index = 0;
  const char* outdir = NULL;
  struct longopt opts[] = {
    {"help", 0, NULL, 'h'},
    {"objext", 0, NULL, 'O'},
    {"exeext", 0, NULL, 'B'},
    {"libext", 0, NULL, 'L'},
    {"create-libs", 0, &cmd_libs, 'l'},
    {"create-objs", 0, &cmd_objs, 'o'},
    {"create-bins", 0, &cmd_bins, 'b'},
    {"builddir", 0, 0, 'd'},
    {"type", 0, 0, 't'},
    {0}
  };

  for(;;) {
    c = getopt_long(argc, argv, "hO:B:L:lobd:t:", opts, &index);
    if(c == -1) break;
    if(c == 0) continue;

    switch(c) {
    case 'h': usage(argv[0]); return 0;
    case 'O': objext = optarg; break;
    case 'B': binext = optarg; break;
    case 'L': libext = optarg; break;
    case 'd': outdir = optarg; break;
    case 't': type = optarg; break;
    default: usage(argv[0]); return 1;
    }
  }

  if(!cmd_bins && !cmd_libs && !cmd_objs) {
    cmd_objs = 1;
    cmd_libs = 1;
  }

  if(!format_linklib_fn)
    format_linklib_fn = &format_linklib_lib;

  if(!set_type(type)) {
    usage(argv[0]);
    return 2;
  }

  if(outdir)
    stralloc_copys(&builddir, outdir);

  strarray_init(&srcs);

  {
    HMAP_DB* sourcedirs;
    rule_t* all = get_rule("all");

    if(builddir.len) {
      rule_t* rule;

      if((rule = get_rule_sa(&builddir))) {
        rule->cmd = &mkdir_command;

        strlist_push_sa(&all->deps, &builddir);
      }
    }

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

    output_all_vars(buffer_1, vars);
    output_all_rules(buffer_1, rules);

//   hmap_dump(sourcedirs, buffer_1);

    hmap_destroy(&sourcedirs);
  }

  return 0;
}