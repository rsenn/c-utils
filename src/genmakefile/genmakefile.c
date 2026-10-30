#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "../../lib/uint32.h"
#include "compiler.h"
#include "../../lib/byte.h"
#include "../../lib/sig.h"
#include "../../lib/debug.h"
#include "genmakefile.h"
#include "is.h"
#include "path.h"
#include "rule.h"
#include "mplab.h"
#include "cmake.h"
#include "sources.h"
#include "sourcedir.h"
#include "ansi.h"
#include "generate.h"
#include "input.h"
#include "output.h"
#include "var.h"
#include <string.h>

#if !WINDOWS_NATIVE
#include <unistd.h>
#else
#include <io.h>
#include <sys/stat.h>
#endif

#ifndef _WIN32
#define _mkdir mkdir
#endif

extern buffer* unix_optbuf;

const char* const build_types[] = {"Release", "RelWithDebInfo", "MinSizeRel", "Debug"};


char pathsep_make = DEFAULT_PATHSEP, pathsep_args = DEFAULT_PATHSEP;
const char* builddir_varname = "BUILDDIR";
strarray dirstack = {0};
int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0, cmd_module = 0, cmd_libs_explicit = 0;
commands_t commands;
strlist vpath = {0}, build_as_lib = {0}, link_dirs = {0};
set_t link_libraries = {0, 0, 0, byte_hash}, build_directories = {0, 0, 0, byte_hash};
bool inst_bins = false, inst_libs = false;
const char *libpfx = DEFAULT_LIBPFX, *newline = "\n", *outfile = NULL, *infile = NULL, *project_name = NULL,
           *cmake_builddir = NULL;
exts_t exts = {
    .obj = DEFAULT_OBJEXT,
    .lib = DEFAULT_LIBEXT,
    .slib = DEFAULT_DSOEXT,
    .bin = DEFAULT_EXEEXT,
    .pps = DEFAULT_PPSEXT,
};
dirs_t dirs;
tools_t tools;
/* zeroed for real by byte_zero(&cfg, ...) in main() before any of this is
 * read -- build_type/lang here are moot, not a meaningful default. */
config_t cfg = {0};
MAP_T targetdirs;

/**
 * @brief      Gets the map keys.
 *
 * @param[in]  map   The map
 * @param      list  The list
 */
static void
map_keys_get(const MAP_T* map, strlist* list) {
  MAP_PAIR_T t;

  MAP_FOREACH(*map, t) { strlist_push(list, MAP_ITER_KEY(t)); }
}

/**
 * @brief      Dumps map keys.
 *
 * @param[in]  m     { parameter_description }
 */
static void
map_keys_dump(const MAP_T* m) {
  strlist out;

  strlist_init(&out, '\n');

  map_keys_get(m, &out);
  buffer_puts(buffer_2, "keys:\n  ");
  buffer_putsl(buffer_2, &out, "\n  ");
  buffer_putnlflush(buffer_2);

  strlist_free(&out);
}

/**
 * @brief      Create a directory
 *
 * @param[in]  dir   Directory path
 * @param[in]  mode  Mode
 *
 * @return     0 on success
 */
static int
mkdir_sa(const stralloc* dir, int mode) {
  int ret;
  stralloc sa;

  stralloc_init(&sa);
  stralloc_copy(&sa, dir);

  if(sa.len > 0 && stralloc_endb(&sa, &pathsep_make, 1))
    sa.len -= 1;

  stralloc_nul(&sa);
  ret = _mkdir(sa.s, mode);
  stralloc_free(&sa);
  return ret;
}

/**
 * @brief      Create all directory components
 *
 * @param[in]  dir   Directory path
 * @param[in]  mode  Mode
 *
 * @return     0 on success
 */
static int
mkdir_components(strlist* dir, int mode) {
  int ret = 0;
  size_t i, n = strlist_count(dir);

  for(i = 1; i <= n; ++i) {
    strlist r = strlist_range(dir, 0, i);

    if(r.sa.len) {
      if(mkdir_sa(&r.sa, mode) == -1) {
        if(errno == EEXIST) {
          errno = 0;
          continue;
        }

        ret = -1;
        break;
      }
    }
  }

  return ret;
}

/**
 * @brief      Make a stralloc point to another one
 *
 * @param      out   Output
 * @param[in]  from  Input
 */
void
stralloc_weak(stralloc* out, const stralloc* from) {
  if(out->a)
    stralloc_free(out);

  out->a = 0;
  out->s = from->s;
  out->len = from->len;
}

/**
 * @brief      Puts a newline into a b uffer
 *
 * @param      b      Output buffer
 * @param      flush  flush
 */
void
buffer_putnl(buffer* b, int flush) {
  buffer_puts(b, newline);

  if(flush)
    buffer_flush(b);
}

/**
 * @brief      Skips a comment
 * @param[in]  p    Buffer
 * @param[in]  len   Length
 *
 * @return
 */
static inline size_t
skip_comment(const char* p, size_t len) {
  return byte_finds(p, len, "*/");
}

/**
 * @brief      main_scan  Checks if the given source file contains a main()        function
 *
 * @param[in]  p     Buffer
 * @param      n     Length
 *
 * @return     true if main() exists
 */
static bool
main_scan(const char* p, size_t n) {
  const char *end, *x;

  for(x = p, end = p + n; x + 5 < end;) {
    size_t i;

    if(x[0] == '/') {
      if(x[1] == '*') {
        i = skip_comment(x, n);
        x += i;
        continue;
      }

      if(x[1] == '/') {
        i = byte_chr(x, n, '\n');
        x += i + 1;
      }
    } else if(!isalpha(x[0]) && x[0] != '_') {
      if(byte_equal(&x[1], 4, "main")) {
        ssize_t i = 0;

        if(x + 5 >= end)
          return false;

        i += 5;
        x += i;

        if(x + (i = scan_whitenskip(x, end - x)) >= end)
          break;

        if((x += i) + 1 < end && *x == '(')
          return true;
      }
    }

    x++;
  }

  return false;
}

/**
 * @brief                Checks for main() routine in source file
 *
 * @param      filename  Path to source file
 *
 * @return     1 when yes, 0 when no, -1 on error
 */
int
main_present(const char* filename) {
  char* x;
  size_t n;

  if(str_equal(exts.src, ".asm") && is_source(filename))
    return 1;

  if((x = (char*)path_mmap_read(filename, &n, pathsep_make))) {
    int ret = main_scan(x, n);

    mmap_unmap(x, n);
    return ret;
  }

  return -1;
}

/**
 * @brief      Get indirect deps from specified targets
 *
 * @param      s      Output set
 * @param      names  Target names
 */
static void
deps_indirect(set_t* s, const strlist* names) {
  size_t n;
  const char* x;
  target* t;
  strlist hier;

  strlist_init(&hier, '\0');

  strlist_foreach(names, x, n) {
    if((t = rule_find_b(x, n))) {
      strlist_pushb(&hier, x, n);
      rule_dep_list_recursive(t, s, -1, &hier);
      strlist_zero(&hier);
    }
  }

  strlist_free(&hier);
}

/**
 * @brief      Get dependencies for libraries
 */
static void
deps_for_libs(void) {
  MAP_PAIR_T t;
  set_t indir;

  set_init(&indir, 0);

  MAP_FOREACH(sourcedir_map, t) {
    sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
    target* lib;
    stralloc sa;

    stralloc_init(&sa);
    path_prefix_s(&dirs.work.sa, str_basename(MAP_ITER_KEY(t)), &sa, pathsep_make);
    stralloc_cats(&sa, exts.lib);

    if((lib = rule_find_sa(&sa))) {
      strlist libs;

      strlist_init(&libs, ' ');
      includes_to_libs(&srcdir->includes, &libs);
      strlist_removes(&libs, lib->name);
      set_clear(&indir);
      deps_indirect(&indir, &libs);

      rule_list(&libs, &lib->deps);
      strlist_free(&libs);
    }

    stralloc_free(&sa);
  }

  set_free(&indir);
}

/**
 * @brief      Print rule dependencies
 *
 * @param      b         Output buffer
 * @param      t         Rule
 * @param      deplist   Dependency list
 * @param      hierlist  Hierarchy
 * @param      depth     Depth
 */
static void
print_rule_deps_r(buffer* b, target* t, set_t* deplist, strlist* hierlist, int depth) {
  target** ptr;
  size_t l = hierlist->sa.len;

  strlist_push(hierlist, t->name);
  stralloc_nul(&hierlist->sa);

  array_foreach_t(&t->deps, ptr) {
    const char* name = (*ptr)->name;

    if(strlist_contains(hierlist, name))
      continue;

    buffer_puts(b, "# ");
    buffer_putnspace(b, depth * 2);
    buffer_puts(b, str_basename(t->name));
    buffer_puts(b, " -> ");
    buffer_puts(b, str_basename(name));
    buffer_putnl(b, 1);

    if(set_adds(deplist, name))
      print_rule_deps_r(b, (*ptr), deplist, hierlist, depth + 1);
  }

  hierlist->sa.len = l;
}

/**
 * @brief       Prints dependency tree for a target
 *
 * @param      b     Output buffer
 * @param      t     Target
 */
void
print_rule_deps(buffer* b, target* t) {
  set_t deplist;
  strlist hierlist;

  set_init(&deplist, 0);
  strlist_init(&hierlist, 0);
  set_adds(&deplist, t->name);
  buffer_putm_internal(b, "# Dependencies for '", t->name, "':", NULL);
  buffer_putnlflush(b);
  print_rule_deps_r(b, t, &deplist, &hierlist, 0);

  set_free(&deplist);
  strlist_free(&hierlist);
}

/**
 * @brief      Add library dir
 *
 * @param[in]  dir   The library dir
 */
void
libdirs_add(const char* dir) {
  stralloc tmp;

  stralloc_init(&tmp);
  path_normalize_sa(dir, &tmp);

  strlist_push_unique_sa(&link_dirs, &tmp);

  stralloc_free(&tmp);
}

/**
 * @brief        Show command line usage
 *
 * @param  argv0 The errmsg arguments array 0
 */
void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [common-options] [--] [target1-options...] -- [target2-options...] ...\n"
                       "\n"
                       "  Multiple targets may be given in one invocation, each with its own\n"
                       "  options and sources, separated by a literal \"--\"; every target's rules\n"
                       "  are generated into the same output file.\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "\n"
                       "  -o, --output FILE         write to file\n"
                       "  -f, --input-file FILE     read from build-log file\n"
                       "  -M, --cmake-builddir DIR  read a CMake build directory recursively for\n"
                       "                            link.txt/compile_commands.json\n"
                       "\n"
                       "  -T, --objext EXT          object file extension\n"
                       "  -B, --exeext EXT          binary file extension\n"
                       "  -X, --libext EXT          library file extension\n"
                       "\n"
                       "      --create-libs         create rule_map for libraries\n"
                       "      --create-objs         create rule_map for objects\n"
                       "      --create-bins         create rule_map for programs\n"
                       "      --create-module       create rule_map for loadable module\n"
                       "  -S, --build-as-lib TARGET build target as library\n"
                       "\n"
                       "  -i, --install             create installation rule_map\n"
                       "\n"
                       //"  -R, --objdir   DIR        object directory\n"
                       "  -O, --outdir   DIR        output directory\n"
                       "  -d, --builddir DIR        build directory\n"
                       "  -k, --bindir   DIR        binary directory (defaults to --builddir)\n"
                       "\n"
                       "  -a, --arch   MACHINE      set architecture\n"
                       "  -s, --system OS           set operating system\n"
                       "  -c, --cross  TARGET       set cross compiler\n"
                       "\n"
                       "  -D, --define NAME[=VALUE] add a preprocessor definition\n"
                       "  -I, --include-path DIR    add an include directory\n"
                       "  -L, --library-path DIR    add a library directory\n"
                       "  -l, --link LIB            link a library\n"
                       "\n"
                       "  -t, --compiler-type TYPE  compiler type, one of:\n"
                       "                            (default: guessed from --builddir/--output,\n"
                       "                            otherwise unset)\n"
                       "\n"
                       "     gcc         GNU make\n"
                       "     bcc55       Borland C++ Builder 5.5\n"
                       "     bcc         Borland C++ Builder >= 6.0\n"
                       "     lcc         lcc\n"
                       "     tcc         TinyCC\n"
                       "     msvc        Visual C++\n"
                       "     icl         Intel C++\n"
                       "     clang       LLVM\n"
                       "     occ         OrangeC\n"
                       "     dmc         Digital Mars C++\n"
                       "     pocc        Pelles-C\n"
                       "     zapcc[-cl]  ZapCC\n"
                       "     zapcc[-cl]  ZapCC\n"
                       "     htc         Hi-Tech C for PIC or PIC18\n"
                       "     xc8         Microchip C Compiler for 8-bit PIC\n"
                       "     sdcc        Small Device C Compiler\n"
                       "\n"
                       "  -m, --make-type TYPE      make program type, one of:\n"
                       "                            (default: derived from --compiler-type,\n"
                       "                            otherwise \"make\")\n"
                       "     nmake       Microsoft NMake\n"
                       "     borland     Borland Make\n"
                       "     gmake       GNU Make\n"
                       "     omake       OrangeCC Make\n"
                       "     pomake      Pelles-C Make\n"
                       "     make        Other make\n"
                       "     batch       Windows batch (.bat .cmd)\n"
                       "     shell       Bourne shell script\n"
                       "     ninja       Ninja build\n"
                       "     mplab       MPLAB project (.mcp)\n"
                       "     mplabx      MPLAB X project (NetBeans)\n"
                       "     cmake       KitWare CMake\n"
                       "\n"
                       "  Specify build type:\n"
                       "\n"
                       "    --debug            with debugging information, no optimization\n"
                       "    --relwithdebinfo   with debugging information, optimize for speed\n"
                       "    --release          optimize for speed\n"
                       "    --minsizerel       optimize for size\n"
                       "\n",
                       NULL);
  buffer_putnlflush(buffer_1);
}

/**
 * @brief      Resolve dirs.work/out/obj/bin/this to their final values and
 *             make dirs.out/dirs.obj/dirs.bin absolute, canonical and
 *             collapsed.
 *
 *             `-w`/`--workdir` (dirs.work) is honored if the user gave it
 *             explicitly; otherwise it falls back to outfile's dirname or
 *             the literal "build".
 *
 *             `-k`/`--bindir` (dirs.bin) is honored if the user gave it
 *             explicitly; otherwise it defaults to dirs.obj.
 */
static void
resolve_directories(void) {
  stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);

  strlist_nul(&dirs.out);
  strlist_nul(&dirs.this);

  if(dirs.work.sa.len == 0) {
    if(outfile)
      path_dirname_s(outfile, &dirs.work.sa);
    else
      stralloc_copys(&dirs.work.sa, "build");
  }

  if(dirs.out.sa.len == 0)
    path_cat_sa(&dirs.out.sa, dirs.this.sa.s, dirs.this.sa.len, dirs.work.sa.s, dirs.work.sa.len);

  if(dirs.obj.sa.len == 0) {
    if(strlist_contains(&dirs.work, "build") && strlist_count(&dirs.work) > 1) {
      stralloc_copy(&dirs.obj.sa, &dirs.work.sa);

    } else if(tools.toolchain && !strlist_contains(&dirs.this, "build")) {
      stralloc target;

      stralloc_init(&target);
      stralloc_copys(&target, tools.toolchain);

      if(cfg.chip.s) {
        stralloc_cats(&target, "-");
        stralloc_cat(&target, &cfg.chip);
      }

      stralloc_nul(&target);
      stralloc_copy(&dirs.obj.sa, &dirs.work.sa);

      if(cross_compile && *cross_compile) {
        strlist_push(&dirs.obj, cross_compile);
      } else {
        strlist_push_sa(&dirs.obj, &target);
        stralloc_catc(&dirs.obj.sa, '-');
        stralloc_cats(&dirs.obj.sa, build_types[cfg.build_type]);
      }

      stralloc_free(&target);
    }

    stralloc_replacec(&dirs.obj.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  }

  if(dirs.work.sa.len == 0)
    stralloc_copys(&dirs.work.sa, ".");

  path_abs_sa(&dirs.out.sa, dirs.out.sa.s, dirs.out.sa.len);
  path_canon_sa(&dirs.out.sa);

  path_abs_sa(&dirs.obj.sa, dirs.obj.sa.s, dirs.obj.sa.len);
  path_canon_sa(&dirs.obj.sa);

  if(dirs.bin.sa.len == 0)
    stralloc_copy(&dirs.bin.sa, &dirs.obj.sa);

  path_abs_sa(&dirs.bin.sa, dirs.bin.sa.s, dirs.bin.sa.len);
  path_canon_sa(&dirs.bin.sa);

  strlist_nul(&dirs.this);
  strlist_nul(&dirs.out);
  strlist_nul(&dirs.obj);
  strlist_nul(&dirs.bin);
  strlist_nul(&dirs.work);
}

/**
 * @brief      Apply -L/-l/-I search paths, compute the relative
 *             `sources_dir`, and create the output/work/build directories
 *             on disk.
 *
 * @param      libdirs   -L arguments
 * @param      libs      -l arguments
 * @param      includes  -I arguments
 */
static void
setup_search_paths(strarray* libdirs, strarray* libs, strarray* includes) {
  stralloc tmp;
  char** it;

  stralloc_init(&tmp);

  if(tools.preproc)
    var_set("CPP", tools.preproc);

  strarray_foreach(libdirs, it) { push_linkdir("LIBS", *it); }
  strarray_foreach(libs, it) { with_lib(*it); }
  strarray_foreach(includes, it) { includes_add(*it); }

  includes_cppflags();

  strlist_nul(&dirs.this);
  strlist_nul(&dirs.out);
  path_rel_sa(&sources_dir, dirs.this.sa.s, str_len(dirs.this.sa.s), dirs.out.sa.s, str_len(dirs.out.sa.s));
  stralloc_nul(&sources_dir);

  if(dirs.out.sa.len) {
    stralloc_replacec(&dirs.this.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    path_abs_sa(&dirs.out.sa, dirs.out.sa.s, dirs.out.sa.len);
    stralloc_zero(&tmp);
    path_rel_sa(&tmp, dirs.this.sa.s, str_len(dirs.this.sa.s), dirs.out.sa.s, str_len(dirs.out.sa.s));
    stralloc_copy(&sources_dir, &tmp);
    stralloc_nul(&sources_dir);

    stralloc_zero(&tmp);
  }

  path_rel_sa(&tmp, dirs.obj.sa.s, str_len(dirs.obj.sa.s), dirs.out.sa.s, str_len(dirs.out.sa.s));

  strlist_nul(&dirs.work);
  stralloc_replacec(&dirs.work.sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

  mkdir_components(&dirs.out, 0755);

  if(stralloc_diffs(&dirs.work.sa, "."))
    mkdir_components(&dirs.work, 0755);

  mkdir_components(&dirs.obj, 0755);

  stralloc_free(&tmp);
}

/**
 * @brief      Open the `-o FILE` output file if one was given, pointing
 *             `*out` at it; otherwise `*out` is left untouched (stdout).
 *
 * @param      filebuf  Buffer storage to truncate/open `outfile` into
 * @param      out      Output buffer pointer to redirect
 *
 * @return     0 on success, 2 if the file couldn't be opened
 */
static int
open_output_file(buffer* filebuf, buffer** out) {
  if(outfile) {
    if('\\' != PATHSEP_C)
      stralloc_replacec(&dirs.out.sa, '\\', PATHSEP_C);

    if(stralloc_equals(&dirs.out.sa, "."))
      stralloc_zero(&dirs.out.sa);
    else
      stralloc_catc(&dirs.out.sa, pathsep_make);

    byte_zero(filebuf, sizeof(*filebuf));

    if(buffer_truncfile(filebuf, outfile)) {
      errmsg_warnsys("ERROR: opening '", outfile, "'", 0);
      return 2;
    }

    *out = filebuf;
  }

  return 0;
}

/**
 * @brief      Create the "all" phony target and, if the resolved make
 *             tool supports pattern/implicit rules, the generic
 *             compile-pattern rule; also wires up the `.asm.o` rule when
 *             building assembly sources.
 *
 * @param      all             Receives the "all" target
 * @param      compile_target  Receives the generic compile rule, or stays
 *                              NULL if the make tool doesn't support one
 */
static void
seed_base_rules(target** all, target** compile_target) {
  *all = rule_get("all");
  (*all)->phony = true;

  stralloc_catc(&(*all)->recipe, '\n');

  if(make_capabs & (MAKE_RULE_PATTERN | MAKE_RULE_IMPLICIT)) {
    stralloc rn;
    bool outputs = false;

    stralloc_init(&rn);

    if(make_capabs & MAKE_RULE_PATTERN) {
      stralloc_copys(&rn, "$(BUILDDIR)");
      stralloc_cats(&rn, "%");
      stralloc_cats(&rn, exts.obj);
      stralloc_cats(&rn, ": %");
      stralloc_cats(&rn, exts.src);
      outputs = true;
    } else {
      stralloc_copys(&rn, exts.src);
      stralloc_cats(&rn, exts.obj);
    }

    *compile_target = rule_get_sa(&rn);
    (*compile_target)->outputs = outputs;

    if(stralloc_length(&(*compile_target)->recipe) == 0)
      stralloc_copy(&(*compile_target)->recipe, &commands.compile);

    stralloc_free(&rn);
  }

  if(str_equal(exts.src, ".asm")) {
    target* assemble = rule_get(".asm.o");

    stralloc_copy(&assemble->recipe, &commands.compile);
  }
}

/**
 * @brief      Ingest a hand-written `-f/--input-file` makefile fragment,
 *             then rewrite every rule's baked-in recipe so any literal
 *             CFLAGS/CPPFLAGS/LDFLAGS text becomes a `$(VAR)` reference
 *             again, feeding the first compile-type recipe found back
 *             into the generic `compile_target` (if any) and recording
 *             the first known build directory as `builddir_varname`.
 *
 * @param      all             The "all" target, passed through to
 *                              input_process_file()
 * @param      compile_target  The generic compile rule, or NULL
 */
static void
ingest_input_file(target* all, target* compile_target) {
  input_process_file(infile, all);

  MAP_PAIR_T iter;

  MAP_FOREACH(rule_map, iter) {
    target* rule = MAP_ITER_VALUE(iter);
    static const char* varnames[] = {
        "CFLAGS",
        "CPPFLAGS",
        "LDFLAGS",
        0,
    };

    for(size_t i = 0; varnames[i]; ++i) {
      const char* value;

      if(!(value = var_get(varnames[i])) || !value[0])
        continue;

      if(stralloc_contains(&rule->recipe, value)) {
        stralloc tmp;
        stralloc_init(&tmp);

        var_subst_b(varnames[i], &tmp, rule->recipe.s, rule->recipe.len, 0, 0);

        stralloc_copy(&rule->recipe, &tmp);
        stralloc_nul(&rule->recipe);
        stralloc_free(&tmp);
      }
    }

    set_subst_sa(&rule->prereq, &rule->recipe, rule->type == COMPILE ? "$<" : "$^");
    set_subst_sa(&rule->output, &rule->recipe, "$@");

    if(rule->type == COMPILE && stralloc_length(&rule->recipe)) {
      if(compile_target && !stralloc_length(&compile_target->recipe)) {
        stralloc_copy(&compile_target->recipe, &rule->recipe);
      }
    }
  }

  {
    stralloc builddir;

    stralloc_init(&builddir);

    set_at_sa(&build_directories, 0, &builddir);

    var_set_b(builddir_varname, builddir.s, builddir.len);
    stralloc_free(&builddir);
  }
}

/**
 * @brief      Build the `COMMON_FLAGS` variable by substituting
 *             CFLAGS/CPPFLAGS/LDFLAGS references into it.
 */
static void
compute_common_flags(void) {
  static const char* varnames[] = {
      "CFLAGS",
      "CPPFLAGS",
      "LDFLAGS",
      0,
  };

  var_list("COMMON_FLAGS", 0)->value.sep = ' ';

  for(size_t i = 0; varnames[i]; ++i) {
    var_t* v;

    if((v = var_list(varnames[i], 0))) {
      var_subst_sa("COMMON_FLAGS", &v->value.sa, NULL, NULL);
    }
  }
}

/**
 * @brief      When a single generic compile_target rule exists, fold
 *             every other compile-type rule's output into it and disable
 *             the (now redundant) per-file recipes for gmake.
 *
 * @param      compile_target  The generic compile rule, or NULL
 */
static void
consolidate_compile_target(target* compile_target) {
  if(compile_target) {
    MAP_PAIR_T it;

    strlist_nul(&dirs.work);
    strlist_push_unique(&vpath, ".");
    strlist_push_unique_sa(&vpath, &dirs.work.sa);

    set_clear(&compile_target->output);

    MAP_FOREACH(rule_map, it) {
      target* rule = MAP_ITER_VALUE(it);

      if(rule_is_compile(rule) && rule != compile_target) {
        stralloc_free(&rule->recipe);
        stralloc_init(&rule->recipe);

        if(str_equal(tools.make, "gmake"))
          rule->disabled = 1;

        set_cat(&compile_target->output, &rule->output);
      }
    }
  }
}

/**
 * @brief      Classify every remaining positional argv[] entry as a
 *             `NAME=value` variable assignment or a path to add to
 *             `args`, then validate at least one path (or `-f`) was
 *             given.
 *
 * @param      argc  Argument count
 * @param      argv  Argument vector
 * @param      args  Receives path-shaped positional arguments
 *
 * @return     0 on success, nonzero (the exit code for main() to return)
 *             if neither positional args nor `-f/--input-file` were given
 */
static int
consume_positional_args(int argc, char** argv, strarray* args) {
  while(unix_optind < argc) {
    stralloc arg;

    stralloc_init(&arg);
    stralloc_copys(&arg, argv[unix_optind]);
    stralloc_nul(&arg);

    if(stralloc_contains(&arg, "=")) {
      size_t eqpos;
      const char* v;

      eqpos = str_chr(arg.s, '=');
      arg.s[eqpos++] = '\0';
      v = &arg.s[eqpos];
      var_set(arg.s, v);
      ++unix_optind;
      continue;
    }

    stralloc_replacec(&arg, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_nul(&arg);

#if WINDOWS_NATIVE || defined(__MINGW32__)
    if(str_rchrs(argv[unix_optind], "*?", 2) < str_len(argv[unix_optind]))
      strarray_glob(args, arg.s);
    else
#endif

      strarray_push(args, arg.s);
    ++unix_optind;

    stralloc_free(&arg);
  }

  path_canon_sa(&dirs.out.sa);

  /* No arguments given */

  if(strarray_size(args) == 0 && !infile && !cmake_builddir) {
    buffer_putsflush(buffer_2, "ERROR: No arguments given\n\n");
    usage(argv[0]);
    return 1;
  }

  return 0;
}

/**
 * @brief      Validate and classify every entry in `args`: source/include
 *             files are added directly, directories are scanned
 *             recursively (via sources_get()/sources_add(), populating
 *             the global sources_set), then the discovered sources are
 *             drained into a sorted `sources` array.
 *
 * @param      args     Positional path arguments (from
 *                       consume_positional_args())
 * @param      sources  Receives the final, sorted list of source files
 *
 * @return     0 on success, nonzero (ret=127, main() should `goto fail`)
 *             if a given path doesn't exist
 */
static int
discover_sources(strarray* args, strarray* sources) {
  char** arg;
  const char* x;
  size_t n;

  strarray_foreach(args, arg) {
    {
      const char* p = *arg;

      if(!path_exists(p)) {
        buffer_putm_internal(buffer_2, "ERROR: Doesn't exist: ", p, newline, NULL);
        buffer_flush(buffer_2);
        return 127;
      }

      if(is_source(p) || is_include(p))
        sources_add(p);
      else if(path_is_directory(p))
        sources_get(p);
    }
  }

  {
    set_iterator_t it;
    set_foreach(&sources_set, it, x, n) {
      if(is_source_b(x, n))
        strarray_pushb(sources, x, n);
    }

    strarray_sort(sources, &sources_sort_callback);
  }

  return 0;
}

/**
 * @brief      Recompute `builddir_varname`'s value for gmake, re-check
 *             `batchmode` now that dirs.work is final, and compute
 *             `project_name`.
 */
static void
finalize_build_metadata(void) {
  if(str_start(tools.make, "g")) {
    stralloc builddir, workabs;

    /* dirs.obj.sa was made absolute above (path_absolute_sa), but
     * dirs.work.sa never is -- path_relative_to_s() needs both sides
     * absolute to produce a sane result, so resolve a throwaway
     * absolute copy of dirs.work.sa here (same trick path_output2()
     * uses for the same reason). */
    stralloc_init(&workabs);
    path_abs_sa(&workabs, dirs.work.sa.s, str_len(dirs.work.sa.s));

    stralloc_init(&builddir);
    path_rel_sa(&builddir, dirs.obj.sa.s, dirs.obj.sa.len, workabs.s, workabs.len);
    stralloc_nul(&builddir);

    if(!stralloc_endc(&builddir, PATHSEP_C))
      stralloc_catc(&builddir, PATHSEP_C);
    stralloc_nul(&builddir);
    var_set(builddir_varname, builddir.s);
    stralloc_free(&builddir);
    stralloc_free(&workabs);
  }

  if(((build_tool == TOOL_BATCH || build_tool == TOOL_SHELL) && stralloc_equals(&dirs.work.sa, ".")))
    batchmode = 1;

  if(output_name.len) {
    project_name = str_ndup(output_name.s, output_name.len);
  } else {
    stralloc abspath;

    stralloc_init(&abspath);
    path_abs_sa(&abspath, dirs.this.sa.s, str_len(dirs.this.sa.s));
    stralloc_nul(&abspath);
    project_name = str_dup(path_basename_s(abspath.s));
    stralloc_free(&abspath);
  }
}

/**
 * @brief      Populate sourcedir_map from `sources`, then generate
 *             library/compile rules for every source directory and link
 *             rules for every program, wiring the results into `all`.
 *             Only called when no `-f/--input-file` was given.
 *
 *             NB: `sources2` (the copy iterated for
 *             sourcedir_addsource()) is never freed here -- neither was
 *             it in the original code. See BUGS
 *             (genmakefile-sources2-leak); not fixed in this refactor.
 *
 * @param      sources  The discovered, sorted source file list --
 *                       consumed: freed by this function via
 *                       sourcedir_populate()+strarray_free()
 * @param      all      The "all" target
 */
static void
generate_all_rules(strarray* sources, target* all) {
  MAP_PAIR_T t;
  int link_rules = 0;
  stralloc src;
  strarray sources2;
  char** ptr;

  stralloc_init(&src);

  strarray_init(&sources2);
  strarray_copy(&sources2, sources);

  strarray_foreach(&sources2, ptr) { sourcedir_addsource(*ptr, sources, &progs, &bins, pathsep_make); }

  sourcedir_populate(sources);
  strarray_free(sources);
  stralloc_free(&src);

  if(cmd_libs) {
    generate_lib_rules(build_tool == TOOL_SHELL, build_tool == TOOL_BATCH, batchmode, pathsep_args, pathsep_make);
    deps_for_libs();
  } else {
    MAP_FOREACH(sourcedir_map, t) {
      sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
      generate_simple_compile_rules(srcdir, MAP_ITER_KEY(t), exts.src, exts.obj, &commands.compile, pathsep_args, NULL);
    }
  }

  if(cmd_bins) {
    int ret;

    if(!(ret = generate_link_rules(pathsep_args, pathsep_make)))
      cmd_bins = 0;

    link_rules += ret;
  }

  if(cmd_module)
    generate_module_rules(pathsep_args, pathsep_make);

  if(cmd_bins == 0 || cmd_libs == 1) {
    MAP_FOREACH(rule_map, t) {
      target* tgt = MAP_ITER_VALUE(t);

      if(stralloc_equal(&tgt->recipe, &commands.lib) && cmd_libs)
        set_adds(&all->prereq, MAP_ITER_KEY(t));
    }
  }
}

/**
 * @brief      Derive mkdir rules for every directory referenced by
 *             rule_map, add the clean rule, attach every link-type rule
 *             as a prerequisite of `all`, and generate install rules if
 *             requested. Runs regardless of `-f/--input-file`.
 *
 * @param      all  The "all" target
 */
static void
generate_auxiliary_rules(target* all) {
  {
    MAP_PAIR_T t;
    set_t rule_dirs = SET();
    set_iterator_t it;
    const char* d;
    size_t dlen;

    MAP_FOREACH(rule_map, t) {
      target* rule = MAP_ITER_VALUE(t);
      stralloc dir, name;
      size_t namelen;

      /* some rule names are "target: mask" pattern-rule keys (see
       * generate_srcdir_rule()/generate_srcdir_compile_rules()), not a
       * plain path -- only the part before ": " is an actual target.
       * path_dirname() can't be handed that bound directly: its
       * "no separator" check reads path[size], which for a bound
       * short of the real NUL isn't '\0' -- so cut a properly
       * NUL-terminated copy first. */
      namelen = str_find(rule->name, ": ");

      stralloc_init(&name);
      stralloc_copyb(&name, rule->name, namelen);
      stralloc_nul(&name);

      stralloc_init(&dir);
      path_dirname_s(name.s, &dir);
      stralloc_nul(&dir);
      stralloc_free(&name);

      if(dir.len && !stralloc_equals(&dir, "."))
        set_insert(&rule_dirs, dir.s, dir.len);

      stralloc_free(&dir);
    }

    set_foreach(&rule_dirs, it, d, dlen) {
      stralloc dirsa;

      stralloc_init(&dirsa);
      stralloc_copyb(&dirsa, d, dlen);
      stralloc_nul(&dirsa);

      generate_mkdir_rule(&dirsa);
      set_insert(&all->prereq, dirsa.s, dirsa.len);

      stralloc_free(&dirsa);
    }

    set_free(&rule_dirs);
  }

  generate_clean_rule(pathsep_make);

  {
    MAP_PAIR_T t;
    MAP_FOREACH(rule_map, t) {
      target* rule = MAP_ITER_VALUE(t);

      if(rule_is_link(rule))
        set_adds(&all->prereq, rule->name);
    }
  }

  if(inst_bins || inst_libs)
    generate_install_rules();
}

/**
 * @brief      Dump the entire "source of truth" -- every rule, variable,
 *             source directory, source file, and the resolved
 *             config/exts/commands/dirs/tools state -- to debug_buf.
 *             Called once, right before the output-writing step. A no-op
 *             unless compiled with DEBUG_OUTPUT.
 */
static void
dump_state(void) {
#ifdef DEBUG_OUTPUT
  MAP_PAIR_T t;
  struct dnode* link;

  buffer_puts(debug_buf, "==== genmakefile state dump ====\n");

  config_dump(&cfg);
  exts_dump(&exts);
  commands_dump(&commands);

  buffer_puts(debug_buf, "dirs.work=\"");
  buffer_putsa(debug_buf, &dirs.work.sa);
  buffer_puts(debug_buf, "\" dirs.out=\"");
  buffer_putsa(debug_buf, &dirs.out.sa);
  buffer_puts(debug_buf, "\" dirs.obj=\"");
  buffer_putsa(debug_buf, &dirs.obj.sa);
  buffer_puts(debug_buf, "\" dirs.this=\"");
  buffer_putsa(debug_buf, &dirs.this.sa);
  buffer_putc(debug_buf, '"');
  buffer_putnlflush(debug_buf);

  buffer_putm_internal(debug_buf, "tools.toolchain=", tools.toolchain ? tools.toolchain : "", NULL);
  buffer_putm_internal(debug_buf, " compiler=", tools.compiler ? tools.compiler : "", NULL);
  buffer_putm_internal(debug_buf, " make=", tools.make ? tools.make : "", NULL);
  buffer_putm_internal(debug_buf, " preproc=", tools.preproc ? tools.preproc : "", NULL);
  buffer_putnlflush(debug_buf);

  buffer_puts(debug_buf, "-- rules --\n");
  MAP_FOREACH(rule_map, t) { rule_dump(MAP_ITER_VALUE(t)); }

  buffer_puts(debug_buf, "-- vars --\n");
  MAP_FOREACH(vars, t) { var_dump(MAP_ITER_KEY(t), (var_t*)MAP_ITER_VALUE(t)); }

  buffer_puts(debug_buf, "-- source directories --\n");
  sourcedir_dump_all(debug_buf);

  buffer_puts(debug_buf, "-- source files --\n");
  dlist_foreach_down(&sources_list, link) { sourcefile_dump(dlist_data(link, sourcefile*)); }

  buffer_putnlflush(debug_buf);
#endif
}

/**
 * @brief      Handle the MPLAB/CMake "project file" output modes, which
 *             replace normal rule emission entirely.
 *
 * @param      out  Output buffer
 *
 * @return     true if a project file was written (caller should skip the
 *             normal makefile/script output), false otherwise
 */
static bool
write_project_file(buffer* out) {
  if(!case_diffs(tools.make, "mplab")) {
    output_mplab_project(out, 0, 0, &include_dirs);
    return true;
  }

  if(!case_diffs(tools.make, "cmake")) {
    output_cmake_project(out, &rule_map, &vars, &include_dirs, &link_dirs);
    return true;
  }

  return false;
}

/**
 * @brief      Emit the "Generated by" banner, all variables, the
 *             VPATH/ninja preamble, fallback compile recipes for any
 *             still-empty object rule, and finally the rules/script
 *             themselves (output_script() for batch/shell, otherwise
 *             output_all_rules()).
 *
 * @param      out      Output buffer
 * @param      cmdline  The original command line, for the banner
 */
static void
write_makefile_output(buffer* out, strlist* cmdline) {
  buffer_putm_internal(out, comment, " Generated by:", newline, comment, "  ", NULL);
  buffer_putsa(out, &cmdline->sa);
  buffer_putsflush(out, newline);
  stralloc_nul(&cfg.chip);
  var_set("CHIP", cfg.chip.s);

  if(build_tool == TOOL_NINJA) {
    stralloc tmp;

    stralloc_init(&tmp);
    path_rel_sa(&tmp, dirs.obj.sa.s, dirs.obj.sa.len, dirs.out.sa.s, dirs.out.sa.len);

    while(stralloc_endb(&tmp, &pathsep_args, 1))
      tmp.len--;

    var_set_b("objdir", tmp.s, tmp.len);
    var_set("extra_cflags", "$$EXTRA_CFLAGS");
    var_set("extra_ldflags", "$$EXTRA_LDFLAGS");

    stralloc_free(&tmp);
  }

  {
    strlist varnames;

    strlist_init(&varnames, '\0');
    map_keys_get(&vars, &varnames);

    output_all_vars(out, &vars, &varnames, build_tool);
    strlist_free(&varnames);
  }

  if(str_equal(tools.make, "gmake")) {
    strlist_nul(&vpath);
    // buffer_putm_internal(out,   "\nvpath ", vpath.sa.s,   "\n", NULL);
    stralloc_replacec(&vpath.sa, ' ', ':');
    buffer_putm_internal(out, "VPATH = ", vpath.sa.s, "\n\n", NULL);
    buffer_flush(out);
  }

  if(build_tool == TOOL_NINJA) {
    output_ninja_rule(out, "cc", &commands.compile);
    output_ninja_rule(out, "link", &commands.link);
    output_ninja_rule(out, "lib", &commands.lib);
    buffer_putnl(out, 0);
  }

  {
    MAP_PAIR_T t;

    MAP_FOREACH(rule_map, t) {
      const char* name = MAP_ITER_KEY(t);
      target* rule = MAP_ITER_VALUE(t);

      if(rule->recipe.len)
        continue;

      if(str_end(name, exts.obj))

        if(!str_end(tools.make, "make"))
          stralloc_weak(&rule->recipe, &commands.compile);
    }
  }

  if(build_tool == TOOL_BATCH || build_tool == TOOL_SHELL) {
    if(build_tool == TOOL_BATCH)
      buffer_putm_internal(out, "CD %~dp0", newline, NULL);
    else
      buffer_putm_internal(out, "cd \"$(dirname \"$0\")\"\n\n", NULL);

    output_script(out, NULL, build_tool, quote_args, pathsep_args, make_sep_inline);
  } else {
    output_all_rules(out, build_tool, quote_args, pathsep_args, pathsep_make, make_sep_inline);
  }
}

/**
 * @brief      Find the next literal "--" token, which delimits one
 *             target's options/arguments from the next in the
 *             `genmakefile [common-options] [--] [target1-opts] --
 *             [target2-opts] ...` multi-target CLI syntax.
 *
 * @param[in]  argc  Argument count
 * @param      argv  Argument vector
 * @param[in]  from  Index to start searching at
 *
 * @return     Index of the "--" token, or `argc` if none was found
 */
static int
find_target_separator(int argc, char* argv[], int from) {
  int i;

  for(i = from; i < argc; i++) {
    if(str_equal(argv[i], "--"))
      return i;
  }

  return argc;
}

/**
 * @brief      Parse one target's options (a `getopt_long()` loop over
 *             `argv[unix_optind..argc)`, which must not include a "--"
 *             separator -- callers scope `argc` to `find_target_separator()`
 *             so permutation of positional args never crosses a target
 *             boundary). Shared by the initial common+target1 parse and
 *             every subsequent `--`-delimited target segment in main().
 *
 * @param[in]  argc      Argument count, scoped to end just before the next
 *                        "--" (or the true argc if there is none)
 * @param      argv      Argument vector
 * @param      opts      Long-option table
 * @param      libs      Receives -l arguments
 * @param      libdirs   Receives -L arguments
 * @param      includes  Receives -I arguments
 *
 * @return     0 on success, 1 if `-h/--help` was given (usage() already
 *             printed, main() should return success), -1 on an option
 *             error (already printed, main() should return failure)
 */
static int
parse_target_options(
    int argc, char* argv[], struct unix_longopt* opts, strarray* libs, strarray* libdirs, strarray* includes) {
  int c, index = 0;
  const char* arg;
  const char* objdir = NULL;

  for(;;) {
    c = unix_getopt_long(argc, argv, "ha:bo:O:B:E:d:k:t:m:n:D:l:I:c:s:p:P:R:S:if:CW:w:L:O:T:X:x:M:", opts, &index);

    if(c == -1)
      break;

    if(c == 0)
      continue;

    arg = unix_optarg ? unix_optarg : argv[unix_optind];

    switch(c) {
      case 'h': {
        usage(argv[0]);
        return 1;
      }

      case 'C': {
        cfg.lang = LANG_C;
        break;
      }

      case 'c': {
        cross_compile = arg;
        break;
      }

      case 'b': {
        cmd_bins = 1;
        cmd_objs = 1;
        break;
      }

      case 'o': {
        outfile = arg;
        break;
      }

      case 'T': {
        exts.obj = arg;
        break;
      }

      case 'B': {
        exts.bin = arg;
        break;
      }

      case 'S': {
        strlist_push(&build_as_lib, arg);
        break;
      }

      case 'X': {
        exts.lib = arg;
        break;
      }

      case 'd': {
        //  dir = arg;
        stralloc_copys(&dirs.obj.sa, arg);
        break;
      }

      case 'k': {
        stralloc_copys(&dirs.bin.sa, arg);
        break;
      }

      case 'R': {
        objdir = arg;
        break;
      }

      case 'O': {
        stralloc_copys(&dirs.out.sa, arg);
        break;
      }

      case 'W': {
        scan_int(arg, &output_width);
        break;
      }

      case 'w': {
        stralloc_copys(&dirs.work.sa, arg);
        break;
      }

      case 't': {
        tools.toolchain = tools.compiler = arg;
        break;
      }

      case 'm': {
        tools.make = arg;
        break;
      }

      case 'P': {
        tools.preproc = arg;
        break;
      }

      case 'a': {
        set_machine(arg);
        break;
      }

      case 's': {
        set_system(arg);
        break;
      }

      case 'n': {
        stralloc_copys(&output_name, arg);
        break;
      }

      case 'p': {
        if(arg)
          set_chip(arg);
        break;
      }

      case 'f': {
        infile = arg;
        break;
      }

      case 'M': {
        cmake_builddir = arg;
        break;
      }

      case 'l': {
        strarray_push(libs, arg);
        break;
      }

      case 'i': {
        inst_bins = 1;
        inst_libs = 1;
        break;
      }

      case 'D': {
        push_define(arg);
        break;
      }

      case 'I': {
        strarray_push(includes, arg);
        break;
      }

      case 'L': {
        strarray_push(libdirs, arg);
        break;
      }

        /* debug log */
      case 'x': {
        if(buffer_appendfile(&debug_buffer, arg)) {
          errmsg_warnsys("Failed opening ", arg, " for output", 0);
          return -1;
        }

        break;
      }

      default:
        buffer_puts(buffer_2, "No such option '");
        buffer_putc(buffer_2, c);
        buffer_putsflush(buffer_2, "'\n");
        // usage(argv[0]);
        return -1;
    }
  }

  return 0;
}

/**
 * @brief      main
 *
 * @param      argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     0 on success
 */
int
main(int argc, char* argv[]) {
  strarray libs, libdirs, includes, args, sources;
  strlist cmdline;
  int ret = 0, seg_argc;
  const char* s;
  buffer filebuf, *out = buffer_1;
  target *all = 0, *compile_target = 0;
  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"objext", 1, NULL, 'T'},
      {"exeext", 1, NULL, 'B'},
      {"libext", 1, NULL, 'X'},
      {"create-libs", 0, &cmd_libs, 1},
      {"create-objs", 0, &cmd_objs, 1},
      {"create-bins", 0, &cmd_bins, 1},
      {"create-module", 0, &cmd_module, 1},
      //{"create-bins", 0, 0, 'b'},
      {"no-create-libs", 0, &cmd_libs, 0},
      {"no-create-objs", 0, &cmd_objs, 0},
      {"no-create-bins", 0, &cmd_bins, 0},
      {"no-create-module", 0, &cmd_module, 0},
      {"name", 0, 0, 'n'},
      {"install", 0, 0, 'i'},
      {"include-path", 0, 0, 'I'},
      {"library-path", 0, 0, 'L'},
      {"objdir", 1, 0, 'R'},
      {"outdir", 1, 0, 'O'},
      {"builddir", 1, 0, 'd'},
      {"bindir", 1, 0, 'k'},
      {"workdir", 1, 0, 'w'},
      {"compiler-type", 1, 0, 't'},
      {"make-type", 1, 0, 'm'},
      {"arch", 1, 0, 'a'},
      {"system", 1, 0, 's'},
      {"release", 0, &cfg.build_type, BUILD_TYPE_RELEASE},
      {"Release", 0, &cfg.build_type, BUILD_TYPE_RELEASE},
      {"relwithdebinfo", 0, &cfg.build_type, BUILD_TYPE_RELWITHDEBINFO},
      {"RelWithDebInfo", 0, &cfg.build_type, BUILD_TYPE_RELWITHDEBINFO},
      {"minsizerel", 0, &cfg.build_type, BUILD_TYPE_MINSIZEREL},
      {"MinSizeRel", 0, &cfg.build_type, BUILD_TYPE_MINSIZEREL},
      {"debug", 0, &cfg.build_type, BUILD_TYPE_DEBUG},
      {"Debug", 0, &cfg.build_type, BUILD_TYPE_DEBUG},
      {"define", 1, NULL, 'D'},
      {"build-as-lib", 0, 0, 'S'},
      {"input-file", 0, 0, 'f'},
      {"cmake-builddir", 1, 0, 'M'},
      {"cross", 0, 0, 'c'},
      {"chip", 1, 0, 'p'},
      {"preprocessor", 1, 0, 'P'},
      {"lang-c", 0, &cfg.lang, LANG_C},
      {"width", 0, &output_width, 'W'},
      {"cxx", 0, &cfg.lang, LANG_CXX},
      {"c++", 0, &cfg.lang, LANG_CXX},
      {0, 0, 0, 0},
  };

  strarray_init(&sources);
  strarray_init(&args);
  strarray_init(&libs);
  strarray_init(&libdirs);
  strarray_init(&includes);

#if !WINDOWS_NATIVE
  sig_ignore(SIGTRAP);
#endif
  errmsg_iam(argv[0]);
  uint32_seed(NULL, 0);

#ifdef _MSC_VER
  unix_optbuf = buffer_1;
#endif

  byte_zero(&cfg, sizeof(cfg));
  byte_zero(&dirs, sizeof(dirs));
  byte_zero(&rule_map, sizeof(rule_map));
  byte_zero(&vars, sizeof(vars));
  byte_zero(&bins, sizeof(bins));
  byte_zero(&tools, sizeof(tools));
  set_init(&link_libraries, 0);
  set_init(&build_directories, 0);

  MAP_NEW(sourcedir_map);
  MAP_NEW(targetdirs);
  MAP_NEW(rule_map);
  MAP_NEW(vars);

#if defined(_WIN32) || defined(_WIN64) || WINDOWS_NATIVE
  cfg.sys.os = OS_WIN;
  cfg.sys.type = NTOS;
#elif defined(__linux__) || defined(__unix__)
  cfg.sys.os = OS_LINUX;
  cfg.sys.type = UNIX;
#elif defined(__APPLE__)
  cfg.sys.os = OS_MAC;
  cfg.sys.type = UNIX;
#endif

  pathsep_args = WINDOWS_NATIVE ? '\\' : '/';
  pathsep_make = cfg.sys.type == NTOS ? '\\' : '/';

  strlist_init(&vpath, ' ');
  strlist_init(&cmdline, ' ');
  strlist_fromv(&cmdline, (const char**)argv, argc);

  strlist_init(&dirs.this, pathsep_make);
  strlist_init(&dirs.out, pathsep_make);
  strlist_init(&dirs.obj, pathsep_make);
  strlist_init(&dirs.bin, pathsep_make);
  strlist_init(&dirs.work, pathsep_make);

  exts.src = ".c";
  exts.inc = ".h";

  seg_argc = find_target_separator(argc, argv, 1);

  switch(parse_target_options(seg_argc, argv, opts, &libs, &libdirs, &includes)) {
    case 1: ret = 0; goto quit;
    case -1: ret = 1; goto quit;
    default: break;
  }

  cmd_libs_explicit = cmd_libs;

  if(!cmd_bins && !cmd_libs && !cmd_objs && !cmd_module) {
    cmd_bins = 1;
    cmd_objs = 1;
    cmd_libs = 1;
  }

  if(inst_bins)
    cmd_bins = 1;

  if(!cmd_libs)
    inst_libs = 0;

  if(!format_linklib_fn)

    format_linklib_fn = &format_linklib_lib;

  if((ret = resolve_toolchain(argv[0])))
    goto quit;

  resolve_directories();

  setup_search_paths(&libdirs, &libs, &includes);

  if((ret = open_output_file(&filebuf, &out)))
    goto quit;

  seed_base_rules(&all, &compile_target);

  if(infile)
    ingest_input_file(all, compile_target);

  if(cmake_builddir)
    input_cmake_builddir(cmake_builddir, all);

  compute_common_flags();

  consolidate_compile_target(compile_target);

  if((ret = consume_positional_args(seg_argc, argv, &args)))
    goto quit;

  if((ret = discover_sources(&args, &sources)))
    goto fail;

  finalize_build_metadata();

  if(!infile && !cmake_builddir)
    generate_all_rules(&sources, all);

  /* additional "--"-delimited targets: genmakefile [common-options] [--]
   * [target1-opts] -- [target2-opts] ... -- [targetN-opts] */
  while(!infile && !cmake_builddir && seg_argc < argc) {
    unix_optind = seg_argc + 1;
    seg_argc = find_target_separator(argc, argv, unix_optind);

    strarray_free(&libs);
    strarray_free(&libdirs);
    strarray_free(&includes);
    strarray_free(&args);
    strarray_free(&sources);
    strarray_init(&libs);
    strarray_init(&libdirs);
    strarray_init(&includes);
    strarray_init(&args);
    strarray_init(&sources);

    cmd_objs = cmd_libs = cmd_bins = cmd_module = 0;

    switch(parse_target_options(seg_argc, argv, opts, &libs, &libdirs, &includes)) {
      case 1: ret = 0; goto quit;
      case -1: ret = 1; goto quit;
      default: break;
    }

    cmd_libs_explicit = cmd_libs;

    if(!cmd_bins && !cmd_libs && !cmd_objs && !cmd_module) {
      cmd_bins = 1;
      cmd_objs = 1;
      cmd_libs = 1;
    }

    if(inst_bins)
      cmd_bins = 1;

    if(!cmd_libs)
      inst_libs = 0;

    setup_search_paths(&libdirs, &libs, &includes);

    set_clear(&sources_set);
    MAP_DESTROY(sourcedir_map);
    MAP_NEW(sourcedir_map);

    if((ret = consume_positional_args(seg_argc, argv, &args)))
      goto quit;

    if((ret = discover_sources(&args, &sources)))
      goto fail;

    generate_all_rules(&sources, all);
  }

  generate_auxiliary_rules(all);

  dump_state();

fail:
  if(!write_project_file(out))
    write_makefile_output(out, &cmdline);

quit:
  strarray_free(&args);
  strarray_free(&sources);
  strlist_free(&cmdline);

  strarray_free(&libs);
  strarray_free(&libdirs);
  strarray_free(&includes);

  MAP_DESTROY(sourcedir_map);

  return ret;
}
