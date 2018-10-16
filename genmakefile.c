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
#define DEFAULT_OBJEXT ".obj"
#define DEFAULT_LIBEXT ".lib"
#define DEFAULT_EXEEXT ".exe"
#else
#define DEFAULT_OBJEXT ".o"
#define DEFAULT_LIBEXT ".a"
#define DEFAULT_EXEEXT ""
#endif

#if WINDOWS_NATIVE
#define DEFAULT_PATHSEP '\\'
#else
#define DEFAULT_PATHSEP '/'
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
static const char* objext = DEFAULT_OBJEXT;
static const char* libext = DEFAULT_LIBEXT;
static const char* binext = DEFAULT_EXEEXT;
static stralloc builddir;
static char pathsep = DEFAULT_PATHSEP;

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

  if(stralloc_endb(out, "lib", 3))
    out->len -= 3;
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
 * Add a path to a strlist
 */
void
add_path(strlist* list, const char* path) {
  size_t i, len = str_len(path);

  strlist_push(list, path);

  for(i = list->sa.len - len; i < list->sa.len; ++i) {
    if(list->sa.s[i] == '/' || list->sa.s[i] == '\\')
      list->sa.s[i] = pathsep;
  }
}
void
add_path_sa(strlist* list, stralloc* path) {
  stralloc_nul(path);
  add_path(list, path->s);
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

void
push_var_sa(const char* name, stralloc* value) {
  strlist_push_unique_sa(get_var(name), value);
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

void
with_lib(const char* lib) {
  stralloc def;
  stralloc_init(&def);
  stralloc_copys(&def, "-DHAVE_");
  if(str_find(lib, "lib") == str_len(lib))
    stralloc_cats(&def, "LIB");
  stralloc_cats(&def, lib);
  stralloc_cats(&def, "=1");
  byte_upper(def.s, def.len);

  push_var_sa("DEFS", &def);

  push_lib("LIBS", lib);
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
      add_path(&rule->deps, *srcfile);

      rule->cmd = &compile_command;
    }
  }

  stralloc_free(&obj);
}

/**
 * Generate compile rules for every source file with a main()
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
    add_path_sa(&rule->deps, &bin);

    if((rule = get_rule_sa(&bin))) {
      add_path_sa(&rule->deps, &obj);

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
  stralloc lib, obj;
  stralloc_init(&lib);
  stralloc_init(&obj);

  hmap_foreach(srcdirs, t) {
    sourcedir_t* srcdir = hmap_data(t);
    const char* base = str_basename(t->key);
    rule_t* rule;

    if(str_equal(base, "lib") || base[0] == '\0') continue;

    stralloc_zero(&lib);
    stralloc_cat(&lib, &builddir);
    stralloc_cats(&lib, base);
    stralloc_cats(&lib, libext);

    if((rule = get_rule_sa(&lib))) {
      sourcefile_t* pfile;
      strlist_init(&rule->deps, ' ');

      slist_foreach(srcdir->sources, pfile) {
        src_to_obj(pfile->name, &obj);

        add_path_sa(&rule->deps, &obj);
      }

      rule->cmd = &lib_command;
    }
  }

  stralloc_free(&lib);
  stralloc_free(&obj);
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

  stralloc_copys(&builddir, "build\\");
  stralloc_cats(&builddir, type);
  stralloc_cats(&builddir, "\\");

  stralloc_copys(&mkdir_command, "IF NOT EXIST \"$@\" MKDIR \"$@\"");

  if(str_start(type, "gnu") || str_start(type, "gcc")) {

    /*
     * GNU GCC compatible compilers
     */

    set_var("CC", "gcc");
    set_var("CXX", "g++");

    set_var("AR", "ar");

    stralloc_copys(&lib_command, "$(AR) rcs $@ $^");
    stralloc_copys(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)");

    format_linklib_fn = &format_linklib_switch;

    /*
     * Visual C++ compiler
     */
  } else    if(str_start(type, "msvc") || str_start(type, "icl")) {

    set_var("CC", "cl");
    set_var("LIB", "lib");
    set_var("LINK", "link");

    stralloc_copys(&link_command, "$(LINK) /OUT:$@ @<<\n\t\t$(LDFLAGS) $^ $(LIBS) $(EXTRA_LIBS)\n<<");
    stralloc_copys(&lib_command, "$(LIB) /OUT:$@ @<<\n\t\t$^\n<<");

    /*
     * Intel C++ compiler
     */
    if(str_start(type, "icl")) {
      set_var("CC", "icl");
      set_var("CXX", "icl");

      set_var("LINK", "xilink");
      set_var("LIB", "xilib");

      push_var("CFLAGS", "-Qip -Qunroll4 -nologo");
    }

    /*
     * Borland C++ Builder
     */
  } else if(str_start(type, "bcc")) {

    push_var("DEFS", "-DWIN32_LEAN_AND_MEAN");

    push_var("CFLAGS", "-q -tWC -tWM -O1");
    push_var("CPPFLAGS", "-Dinline=__inline");

    if(str_find(type, "55") == str_len(type)) {
      set_var("CC", "bcc32c");
      set_var("CXX", "bcc32x");

      stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -o $@ @&&|\n$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)\n|");
    } else {
      set_var("CC", "bcc32");
      set_var("CXX", "bcc32");

      push_var("CFLAGS", "-ff -fp");

      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
      stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -e$@ @&&|\n$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)\n|");
    }

    set_var("LINK", "ilink32");
    set_var("LIB", "tlib");

    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");

    stralloc_copys(&lib_command, "$(LIB) @&&|\n\t/a /u \"$@\" $^\n|");

    /*
     * LCC compiler
     */
  } else if(str_start(type, "lcc")) {

    set_var("CC", "lcc");

    /*
     * Tiny CC compiler
     */
  } else   if(str_start(type, "tcc")) {
    set_var("CC", "tcc");

  } else {
    return 0;
  }

   with_lib("zlib");
   with_lib("bz2");
   with_lib("lzma");

  push_lib("EXTRA_LIBS", "advapi32");
  push_lib("EXTRA_LIBS", "ws2_32");
  push_lib("EXTRA_LIBS", "iphlpapi");
  push_lib("EXTRA_LIBS", "psapi");

  return 1;
}

int
main(int argc, char* argv[]) {
  const char* type = "gnu";
  static int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
  int c;
  int index = 0;
  const char* outdir = NULL;
  strarray args;

  struct longopt opts[] = {
    {"help", 0, NULL, 'h'},
    {"objext", 0, NULL, 'O'},
    {"exeext", 0, NULL, 'B'},
    {"libext", 0, NULL, 'L'},
    {"create-libs", 0, &cmd_libs, 0},
    {"create-objs", 0, &cmd_objs, 0},
    {"create-bins", 0, &cmd_bins, 0},
    {"builddir", 0, 0, 'd'},
    {"type", 0, 0, 't'},
    {0}
  };

  for(;;) {
    c = getopt_long(argc, argv, "hO:B:L:d:t:", opts, &index);
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

  strarray_init(&args);
  strarray_init(&srcs);

   while(optind < argc) {
#if WINDOWS_NATIVE && !MINGW
     if(str_rchrs(argv[optind], "*?", 2) < str_len(argv[optind]))
       strarray_glob(&args, argv[optind]);
     else
#endif
     strarray_push(&args, argv[optind]);
     ++optind;
   }

  {
    char** arg;
    HMAP_DB* sourcedirs;
    rule_t* all = get_rule("all");

    if(builddir.len) {
      rule_t* rule;

      if((rule = get_rule_sa(&builddir))) {
        rule->cmd = &mkdir_command;

        add_path_sa(&all->deps, &builddir);
      }
    }

    hmap_init(1024, &sourcedirs);

    strarray_foreach(&args, arg) {
      if(str_end(*arg, ".c"))
        add_source(*arg, &srcs);
      else
        get_sources(*arg, &srcs);
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
