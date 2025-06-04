#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "lib/uint32.h"
#include "lib/byte.h"
#include "lib/sig.h"
#include "debug.h"
#include "genmakefile.h"
#include "src/genmakefile/is.h"
#include "src/genmakefile/path.h"
#include "src/genmakefile/rule.h"
#include "src/genmakefile/mplab.h"
#include "src/genmakefile/cmake.h"
#include "src/genmakefile/sources.h"
#include "src/genmakefile/sourcedir.h"
#include "src/genmakefile/ansi.h"
#include "src/genmakefile/generate.h"
#include "src/genmakefile/input.h"
#include "src/genmakefile/output.h"
#include "src/genmakefile/var.h"
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

void debug_int(const char* name, int i);
void debug_sa(const char* name, stralloc* sa);
void debug_sl(const char* name, const strlist* l, const char* sep);
void debug_str(const char* name, const char* s);

const char* const build_types[] = {"Release", "RelWithDebInfo", "MinSizeRel", "Debug"};

static const char *make_begin_inline, *make_sep_inline, *make_end_inline, *comment = "#", *cross_compile = "", *builddir_varname = "BUILDDIR", *quote_args = "";
static bool batchmode, cygming;
static strlist system_path;
static build_tool_t build_tool = 0;

char pathsep_make = DEFAULT_PATHSEP, pathsep_args = DEFAULT_PATHSEP;
strarray dirstack = {0};
int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0, cmd_module = 0;
commands_t commands;
strlist vpath = {0}, build_as_lib = {0}, link_dirs = {0};
set_t link_libraries = {0, 0, 0, byte_hash}, build_directories = {0, 0, 0, byte_hash};
bool inst_bins = false, inst_libs = false;
const char *libpfx = DEFAULT_LIBPFX, *newline = "\n", *outfile = NULL, *infile = NULL, *project_name = NULL;
exts_t exts = {
    .obj = DEFAULT_OBJEXT,
    .lib = DEFAULT_LIBEXT,
    .slib = DEFAULT_DSOEXT,
    .bin = DEFAULT_EXEEXT,
    .pps = DEFAULT_PPSEXT,
};
dirs_t dirs;
tools_t tools;
config_t cfg = {{0, 0}, {0, 0}, {0, 0, 0}, 1, LANG_CXX};
tool_config_t tool_config = 0;
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

#ifdef DEBUG_OUTPUT_
    debug_sa("mkdir_components", &r.sa);
#endif

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
 * @brief      Sets a command string to a new value
 *
 * @param      sa    Output
 * @param      cmd   Command
 * @param      args  Arguments
 */
static void
set_command(stralloc* sa, const char* cmd, const char* args) {
  stralloc_copys(sa, cmd);

  if(args) {
    stralloc_catc(sa, ' ');

    if(!(build_tool == TOOL_NINJA || build_tool == TOOL_BATCH) && (make_begin_inline && make_end_inline)) {
      stralloc_cats(sa, make_begin_inline);

      if(!str_start(tools.make, "nmake"))
        stralloc_subst(sa, args, str_len(args), "$^", "$|");
      else
        stralloc_copys(sa, args);

      stralloc_cats(sa, make_end_inline);
    } else {
      stralloc_cats(sa, args);
    }
  }

  if(str_start(tools.make, "nmake"))
    stralloc_replaces(sa, "$^", "$**");
}

/**
 * @brief      extract_build_type
 *
 * @param      s     { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
static int
extract_build_type(const stralloc* s) {
  size_t i;

  for(i = 0; i < sizeof(build_types) / sizeof(build_types[0]); ++i)

    if(stralloc_contains(s, build_types[i]))
      return i;

  return -1;
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

#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(debug_buf, "Deps for library '", lib->name, "': ", NULL);
      buffer_putsa(debug_buf, &libs.sa);
      buffer_putnlflush(debug_buf);
#endif

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
 * @brief       Removes all indirect dependencies
 *
 * @param      top    Toplevel dependencies
 * @param      a      Dependency layer array
 * @param      depth  Recursion depth
 */
/*static void
remove_indirect_deps_recursive(array* top, array* a, int depth) {
  target **p, **found;

  array_foreach_t(a, p) {
    target* t;

    if((t = *p) == NULL)
      continue;

    if(depth > 0)

      if((found = array_find(top, sizeof(target*), &t)))
        *found = NULL;

    if(a != &t->deps)

      if(depth < 100 && array_length(&t->deps, sizeof(target*)) > 0)
        remove_indirect_deps_recursive(top, &t->deps, depth + 1);
  }
}*/

/**
 * @brief      Removes indirect deps
 *
 * @param      deps  Dependencies
 *
 * @return    Number of dependencies removed
 */
/*static ssize_t
remove_indirect_deps(array* deps) {
  size_t w, r, n;
  target** a;

  remove_indirect_deps_recursive(deps, deps, 0);

  n = array_length(deps, sizeof(target*));
  a = array_start(deps);

  for(w = 0, r = 0; r < n; ++r)

    if(a[r])
      a[w++] = a[r];

  array_truncate(deps, sizeof(target*), w);

  return r - w;
}*/

/**
 * @brief         Set the machine type
 *
 * @param      s     The new value
 *
 * @return     1 on success
 */
static int
set_machine(const char* s) {
  int ret = 1;

  if(s[str_find(s, "64")])
    cfg.mach.bits = _64;
  else if(s[str_find(s, "32")])
    cfg.mach.bits = _32;
  else if(s[str_find(s, "386")])
    cfg.mach.bits = _32;
  else
    ret = 0;

  if(str_start(s, "pic"))
    cfg.mach.arch = PIC;
  else if(s[str_find(s, "arm")] || s[str_find(s, "aarch")])
    cfg.mach.arch = ARM;
  else if(s[str_find(s, "86")])
    cfg.mach.arch = X86;
  else
    ret = 0;

  return ret;
}

/**
 * @brief         Set the system type
 *
 * @param      s     The new value
 *
 * @return     1 on success
 */
static int
set_chip(const char* s) {
  int ret = 1;
  size_t pos = 0;

  if(s[(pos = str_find(s, "16f"))] || s[(pos = str_find(s, "16F"))]) {
    cfg.mach.arch = PIC;
    cfg.mach.bits = _14;
  } else if(s[(pos = str_find(s, "18f"))] || s[(pos = str_find(s, "18F"))]) {
    cfg.mach.arch = PIC;
    cfg.mach.bits = _16;
  }

  stralloc_copys(&cfg.chip, &s[pos]);

  return ret;
}

/**
 * @brief         Set the system type
 *
 * @param      s     The new value
 *
 * @return     1 on success
 */
static int
set_system(const char* s) {
  int ret = 1;

  if(str_contains(s, "win") || str_contains(s, "mingw")) {
    cfg.sys.os = OS_WIN;
    cfg.sys.type = NTOS;
  } else if(s[str_find(s, "msys")] || s[str_find(s, "cygwin")]) {
    cfg.sys.os = OS_WIN;
    cfg.sys.type = UNIX;
  } else if(s[str_find(s, "mac")]) {
    cfg.sys.os = OS_MAC;
    cfg.sys.type = UNIX;
  } else if(s[str_find(s, "lin")]) {
    cfg.sys.os = OS_LINUX;
    cfg.sys.type = UNIX;
  } else {
    ret = 0;
  }

  return ret;
}

/**
 * @brief        Set tools.make program type
 *
 * @return     1 on success
 */
static int
set_make_type() {
  const char* inst = "install";

#if WINDOWS_NATIVE
  newline = "\r\n";
#else
  newline = "\n";
#endif

  stralloc_copys(&commands.mkdir, cfg.sys.os == OS_WIN ? "IF NOT EXIST $@ MKDIR $@" : "mkdir -p $@");

  if(str_start(tools.make, "batch") || str_start(tools.make, "cmd")) {
    pathsep_args = '\\';
    pathsep_make = '\\';
  } else if(str_start(tools.make, "bmake") || str_start(tools.make, "borland")) {
    /* Borland LANG_C++ Builder Make */
    pathsep_make = '\\';
    make_begin_inline = "@&&|\r\n ";
    make_sep_inline = " ";
    make_end_inline = "\r\n|";
    inst = "COPY /Y";
    newline = "\r\n";
  } else if(str_start(tools.make, "nmake")) {
    /* Microsoft NMake */
    pathsep_make = '\\';
    pathsep_args = '\\';
    // make_begin_inline = "@<<$*.rsp\r\n";
    // make_end_inline = "\r\n<<keep";
    stralloc_copys(&commands.mkdir, "IF NOT EXIST $@ MKDIR $@");
    stralloc_copys(&commands.delete, "DEL /F");
    newline = "\r\n";
    inst = "COPY /Y";
  } else if(str_start(tools.make, "gmake") || str_start(tools.make, "gnu")) {
    newline = "\n";
    pathsep_make = '/';
    stralloc_copys(&commands.mkdir, "test -d $@ || mkdir -p $@");
    stralloc_copys(&commands.delete, "rm -f");
  } else if(str_start(tools.make, "omake") || str_start(tools.make, "orange")) {
    pathsep_make = '\\';

    if(inst_bins || inst_libs)
      var_set("INSTALL", "copy /y");
  } else if(str_start(tools.make, "ninja")) {
    build_tool = TOOL_NINJA;
    pathsep_make = pathsep_args = PATHSEP_C;
    make_begin_inline = make_sep_inline = make_end_inline = 0;
  } else if(str_start(tools.make, "po")) {
    pathsep_make = '\\';
    make_begin_inline = "@<<\r\n ";
    make_end_inline = "\r\n<<";
    inst = "copy /y";
  } else if(str_equal(tools.make, "mplab")) {
  } else if(str_equal(tools.make, "cmake")) {
    builddir_varname = "CMAKE_CURRENT_BINARY_DIR";
    sourcedir_varname = "CMAKE_CURRENT_SOURCE_DIR";
  }

  if(inst_bins || inst_libs)
    var_set("INSTALL", inst);

  pathsep_args = pathsep_make;
  return 1;
}

/**
 * @brief      Set the tools.compiler type
 *
 * @param      compiler  The compiler
 *
 * @return      1 on success
 */
static int
set_compiler_type(const char* compiler) {
  var_set("CC", "cc");
  var_set("CXX", "c++");

  stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");

  set_command(&commands.lib, "$(LIB) /out:$@", "$^");
  set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  set_command(&commands.preprocess, "$(CPP) $(CPPFLAGS) $(DEFS) -o$@", "$<");

  /*
   * Visual C++ compiler
   */

  if(str_start(compiler, "msvc") || str_start(compiler, "icl") || str_start(compiler, "vs20") || str_start(compiler, "vc") || compiler[str_find(compiler, "-cl")]) {
    exts.obj = ".obj";
    exts.bin = ".exe";
    exts.lib = ".lib";
    var_set("CC", "cl -nologo");
    var_set("LIB", "lib");
    var_set("LINK", "link");
    var_push("CFLAGS", cfg.build_type == BUILD_TYPE_DEBUG ? "-MTd" : "-MT");
    var_push("CPPFLAGS", "-Dinline=__inline");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_push("CFLAGS", "-Zi");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "-Os");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-Ox");
    // var_push("LDFLAGS", "/DEBUG /DYNAMICBASE /INCREMENTAL /NXCOMPAT /TLBID:1");

    // var_push("LDFLAGS", "/SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT");
    // var_push("LDFLAGS", "/MANIFEST /manifest:embed2 /MANIFESTUAC:\"level=asInvoker uiAccess=false\"");

    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS)                    c -Fo\"$@\" $<");
    set_command(&commands.lib, "$(LIB) -out:$@", "$^");
    // stralloc_copys(&commands.lib, "$(LIB) /OUT:$@ <<\n\t\t$^\n<<");

    /*
     * Intel C++ compiler
     */

    if(str_start(compiler, "icl")) {
      var_set("CC", "icl -nologo");
      var_set("CXX", "icl -nologo");
      var_set("LINK", "xilink");
      var_set("LIB", "xilib");
      var_push("CFLAGS", "-Qip");
      var_push("CFLAGS", "-Qunroll4");
      var_push("CFLAGS", "-Qauto-ilp32");

      if(cfg.mach.bits == _64)
        var_push("LDFLAGS", "-libpath:\"$(ROOT)\\compiler\\lib\\intel64\"");
      else
        var_push("LDFLAGS", "-libpath:\"$(ROOT)\\compiler\\lib\"");
      // stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo $@ $<");
    }

    var_push("LDFLAGS", "-libpath:\"$(UNIVERSALCRTSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)             crt\\$(MACHINE)\"");
    var_push("LDFLAGS",
             "-libpath:\"$(WINDOWSSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)um\\$"
             "(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCTOOLSINSTALLDIR)lib\\$(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(WINDOWSSDKDIR)lib$(X64)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\lib$(AMD64)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\PlatformSDK\\lib$(AMD64)\"");
    var_push("LDFLAGS", "-incremental -manifest");

    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("LDFLAGS", "-debug");

    if(str_start(compiler, "icl"))
      var_push("LDFLAGS", "-manifest:embed -manifestuac:\"level='asInvoker'                iAccess='false'\"");

    if(cfg.mach.arch == ARM) {
      var_push("LDFLAGS", "-machine:ARM");
      var_set("MACHINE", cfg.mach.bits == _64 ? "arm64" : "arm");
    } else if(cfg.mach.bits == _64) {
      var_push("LDFLAGS", "-machine:X64");
      var_set("MACHINE", "x64");
      var_set("X64", "\\x64");
      var_set("AMD64", "\\amd64");
    } else if(cfg.mach.bits == _32) {
      var_push("LDFLAGS", "-machine:X86");
      var_set("MACHINE", "x86");
      var_set("X64", "");
    }

    set_command(&commands.link, "$(LINK) -out:$@ $(LDFLAGS) $(EXTRA_LDFLAGS) -pdb:\"$@.pdb\"", "$^ $(LIBS) $(EXTRA_LIBS)");
  } else if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming || str_start(compiler, "clang") || str_start(compiler, "llvm") || str_start(compiler, "zapcc")) {
    exts.lib = ".a";
    exts.obj = ".o";

    if(str_start(compiler, "zapcc"))
      var_set("CC", "zapcc");

    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_set("CFLAGS", "-O0");
    else if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_set("CFLAGS", "-Os");
    else
      var_set("CFLAGS", "-O2");

    if(str_end(compiler, "32"))
      var_push("CFLAGS", "-m32");

    if(str_end(compiler, "64"))
      var_push("CFLAGS", "-m64");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("LDFLAGS", "-g");
    }

    /*
     * GNU GCC compatible compilers
     */

    if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming) {
      var_set("CC", "gcc");
      var_set("CXX", "g++");
      var_set("AR", str_start(compiler, "gcc") ? "gcc-ar" : "ar");

      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
        var_push("CFLAGS", "-ggdb");
    } else if(str_start(compiler, "clang") || str_start(compiler, "llvm")) {
      pathsep_args = '/';
      var_set("CC", "clang");
      var_set("CXX", "clang++");
      var_set("AR", "llvm-ar");
    }
    set_command(&commands.lib, "$(AR) rcs $@", "$^");
    // set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");

    set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS)                 (EXTRA_LDFLAGS) -o $@", "$^ $(LIBS)");
    exts.bin = "";

    format_linklib_fn = &format_linklib_switch;

    /*
     * Borland C++ Builder
     */
  } else if(str_start(compiler, "bcc")) {
    // pathsep_args = '\\';
    // var_push("DEFS", "-DWIN32_LEAN_AND_MEAN");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_set("CFLAGS", "-O1");
    else if(cfg.build_type == BUILD_TYPE_RELEASE || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_set("CFLAGS", "-O -O2");
    // var_push("CFLAGS", "-q");
    var_push("CFLAGS", "-tWC -tWM");
    var_push("CPPFLAGS", "-Dinline=__inline");
    var_push("LDFLAGS", "-q");

    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-w");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "-d -a-");

    /* Embracadero C++ */

    if(!str_contains(compiler, "55") && !str_contains(compiler, "60")) {
      var_set("CC", "bcc32c");
      var_set("CXX", "bcc32x");
      /* C99 standard */
      var_push("CFLAGS", "-An");

      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
        var_push("CFLAGS", "-v");

      /*if(cfg.build_type != BUILD_TYPE_DEBUG)
          var_push("CFLAGS", "-Or");*/
      set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ ", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

      /* Borland C++ Builder 5.5 */
    } else {
      var_set("CC", "bcc32");
      var_set("CXX", "bcc32");
      var_push("CFLAGS", "-ff -fp");

      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
        var_push("CFLAGS", "-y");

      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
        var_push("CFLAGS", "-v");
        var_push("LDFLAGS", "-v");
      }

      if(cfg.build_type == BUILD_TYPE_DEBUG)
        var_push("CFLAGS", "-w-use");
      else
        var_push("CFLAGS", "-r");

      stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS)                      c -o$@ $<");
      set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -e$@", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    }

    var_set("LINK", "ilink32");
    var_set("LIB", "tlib");
    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");
    set_command(&commands.lib, "$(LIB) /p256 $@ /u", "$^");
    quote_args = "\"";
    pathsep_args = '\\';

    /*
     * LCC compiler
     */
  } else if(str_start(compiler, "lcc")) {
    if(cfg.mach.bits == _64) {
      var_set("CC", "lcc64");
      var_set("LINK", "lcclnk64");
      var_set("LIB", "lcclib64");
    } else {
      var_set("CC", "lcc");
      var_set("LINK", "lcclnk");
      var_set("LIB", "lcclib");
    }

    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-g2");

    make_begin_inline = 0;
    make_end_inline = 0;
    // var_push("STDC_LIBS", "oldnames.lib");

    if(cfg.mach.bits == _64) {
      var_push("STDC_LIBS", "ccl64.lib");
      var_push("STDC_LIBS", "libc64.lib");
    } else {
      var_push("STDC_LIBS", "libc.lib");
    }

    stralloc_copys(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o                    @ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&commands.link, "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS)                    (EXTRA_LIBS) $(STDC_LIBS)");
    /*
     * Tiny CC compiler
     */
  } else if(str_start(compiler, "tcc")) {
    exts.lib = ".a";
    exts.obj = ".o";

    format_linklib_fn = &format_linklib_switch;
    var_set("CC", "tcc");
    var_set("AR", "$(CC) -ar");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_push("CFLAGS", "-g");
    // var_push("LDFLAGS",  "-Wl,-subsystem=console");

    /*if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("LDFLAGS", "-Wl,-file-alignment=16");*/
    var_push("CFLAGS", "-Wall");

    var_push("CPPFLAGS", "-D__TCC__=1");
    set_command(&commands.lib, "$(AR) r $@", "$^");
    set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");
  } else if(str_start(compiler, "occ") || str_start(compiler, "orange")) {
    var_set("CC", "occ");
    var_set("LIB", "olib");
    var_set("LINK", "olink");
    exts.obj = ".o";
    exts.lib = ".l";
    var_push("CPPFLAGS", "-Dinline=__inline");
    // var_push("LDFLAGS", "/Wcm");
    var_push("CFLAGS", "-C+? +1 -v -E-36 -E-39");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "+v");
      var_push("LDFLAGS", "-v -c+");
    }

    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-O-");

    var_push("LDFLAGS", "-T:CON32");
    push_lib("DEFAULT_LIBS", "clwin");
    push_lib("DEFAULT_LIBS", "climp");
    // stralloc_copys(&commands.compile, "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -o$@ \"/I;\" $<");
    stralloc_copys(&commands.compile, "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS)                    (DEFS) -o$@ $<");
    set_command(&commands.lib, "$(LIB) /! $@", "$^");
    set_command(&commands.link, "$(LINK) -c /! $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@", "$^ c0xpe.o $(LIBS) $(DEFAULT_LIBS)");
  } else if(str_start(compiler, "8cc")) {
    exts.lib = ".a";
    exts.obj = ".o";
    var_set("CC", "8cc");
  } else if(str_start(compiler, "dmc") || str_start(compiler, "digitalmars")) {
    // pathsep_args = '\\';
    var_set("CC", "dmc");
    var_set("LIB", "lib");
    var_set("CFLAGS", "");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("LDFLAGS", "-g");
    }

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL) {
      var_push("CFLAGS", "-a1 -o+space ");
      var_push("LDFLAGS", "-Nc");
    } else if(cfg.build_type == BUILD_TYPE_DEBUG) {
      // var_push("CFLAGS", "-o-");
    } else {
      var_push("CFLAGS", "-o");
    }
    // set_command(&commands.lib, "$(LIB) -c $@", "$^");
    set_command(&commands.lib, "$(LIB) -c $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
    set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "pelles") || str_start(compiler, "po")) {
    var_set("CC", "cc");
    var_set("LINK", "polink");
    var_set("LIB", "polib");
    var_set("TARGET", cfg.mach.bits == _64 ? "amd64-coff" : "x86-coff");
    var_set("CFLAGS", "-W0");

    if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-Ob1");
    // var_push("CFLAGS", "-fp:precise");
    var_push("CFLAGS", "-Ze"); /* Activates Microsoft's extensions to C */
    // var_push("CFLAGS", "-Zx"); /* Activates Pelle's extensions to C */
    var_push("CFLAGS", "-Go"); /* Accepts 'old' names for C runtime functions */

    // var_push("CFLAGS", "-Gz"); /* default to __stdcall */
    var_push("CPPFLAGS", "-D__POCC__");

    if(cfg.mach.bits == _64) {
      var_set("MACHINE", "AMD64");
      var_set("L64", "64");
      // exts.lib = "64.lib";
      var_push("CPPFLAGS", "-D_M_AMD64");
    } else if(cfg.mach.bits == _32) {
      var_set("MACHINE", "X86");
      var_set("L64", "");
      var_push("CPPFLAGS", "-D_M_IX86");
    }

    var_push("CFLAGS", "-T$(TARGET)");
    var_push("LDFLAGS", "-machine:$(MACHINE)");
    var_push("LDFLAGS", "-libpath:\"%PELLESC%\\lib\"");
    var_push("LDFLAGS", "-libpath:\"%PELLESC%\\lib\\win$(L64)\"");
    /*    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
          var_push("CFLAGS", "-Os");
        else*/

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-Zi");
      var_push("LDFLAGS", "-DEBUG");
    }

    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -Fo$@");
    stralloc_copys(&commands.link, "$(CC) $^ -Fe $@ $(LDFLAGS) $(EXTRA_LDFLAGS) $(LIBS)                    (EXTRA_LIBS) $(STDC_LIBS)");
    pathsep_args = '\\';
    quote_args = "\"";
  } else if(str_start(compiler, "gp")) {
    var_set("AS", "gpasm");
    var_set("LINK", "gplink");
    var_set("LIB", "gplib");
    var_unset("CXX");
    cfg.mach.arch = PIC;
    exts.bin = ".cof";
    exts.obj = ".o";
    exts.lib = ".a";
    exts.src = ".asm";
    exts.inc = ".inc";

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("LDFLAGS", "-g");
    }

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    if(!var_isset("MACH")) {
      if(cfg.mach.bits == _14)
        var_set("MACH", "pic14");
      else
        var_set("MACH", "pic16");
    }

    if(cfg.mach.bits == _14) {
    }

    if(cfg.mach.bits == _16) {
    }

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL) {
    } else if(cfg.build_type != BUILD_TYPE_DEBUG) {
    }

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-d");
    }
    var_push("CFLAGS", "-p$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");
    var_push("CPPFLAGS", "-DGPUTILS=1");

    /*if(cfg.mach.bits == _16) {
      var_push("LIBS", "-llibm18f.lib");
    } else {
      var_push("LIBS", "-llibm.lib");
    }*/
    set_command(&commands.lib, "$(LIB) rcs $@", "$^");
    stralloc_copys(&commands.compile, "$(AS) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(&commands.link, "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS)                    (EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "sdcc")) {
    var_set("CC", "sdcc");
    var_set("LINK", "sdcc");
    var_set("LIB", "sdar");
    var_unset("CXX");
    cfg.mach.arch = PIC;
    exts.bin = ".cof";
    exts.obj = ".o";
    exts.lib = ".a";

    // var_set("TARGET", cfg.mach.bits == _14 ? "pic16" : "pic18");

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");
    /* stralloc_nul(&cfg.chip);
     var_set("CHIP", cfg.chip.s);*/

    /*{
      stralloc chipdef;
      stralloc_init(&chipdef);
      stralloc_copys(&chipdef, "-DPIC"); stralloc_cat(&chipdef, &cfg.chip);
      stralloc_upper(&chipdef);
      stralloc_cats(&chipdef, "=1");
      var_push_sa("CPPFLAGS", &chipdef);
    }*/

    if(!var_isset("MACH")) {
      if(cfg.mach.bits == _14)
        var_set("MACH", "pic14");
      else
        var_set("MACH", "pic16");
    }

    var_set("CFLAGS", "--use-non-free");

    if(cfg.mach.bits == _16)
      var_push("CFLAGS", "--pstack-model=large");

    if(cfg.mach.bits == _16) {
      var_push("CFLAGS", "--mplab-comp");
      // var_push("CFLAGS", "--extended");
      var_push("CFLAGS", "--pstack-model=large");
      var_push("CFLAGS", "--optimize-cmp");
      var_push("CFLAGS", "--optimize-df");
    }

    var_push("CFLAGS", "--float-reent");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "--opt-code-size");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "--opt-code-speed");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_push("CFLAGS", "--debug");

    var_push("CFLAGS", "-m$(MACH) -p$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");
    var_push("CPPFLAGS", "-DSDCC=1");
    // var_push("LDFLAGS", "--out-fmt-ihx");

    if(cfg.mach.bits == _16)
      var_push("LIBS", "-llibm18f.lib");
    else
      var_push("LIBS", "-llibm.lib");

    set_command(&commands.lib, "$(LIB) rcs $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o                    @ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "htc")) {
    var_unset("CXX");
    var_set("LIB", "libr");
    cfg.mach.arch = PIC;
    exts.bin = ".cof";
    exts.obj = ".p1";
    exts.lib = ".a";

    // var_set("CFLAGS", "--mode=pro");
    var_push("CFLAGS", "-N127");
    // var_push("CFLAGS", "-V");
    var_push("CPPFLAGS", "-DHI_TECH_C=1");
    var_set("TARGET", cfg.mach.bits == _14 ? "pic16" : "pic18");
    // var_push("CPPFLAGS", cfg.mach.bits == _14 ?
    //"-DPIC16=1" : "-DPIC18=1");
    var_set("CC", cfg.mach.bits == _14 ? "picc" : "picc18");
    var_set("LINK", cfg.mach.bits == _14 ? "picc" : "picc18");

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    /*stralloc_nul(&cfg.chip);
    var_set("CHIP", cfg.chip.s);*/

    if(!var_isset("MACH")) {
      if(cfg.mach.bits == _14)
        var_set("MACH", "pic14");
      else
        var_set("MACH", "pic16");
    }

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "--opt=space");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "--opt=speed");
    else
      var_push("CFLAGS", "--opt=all");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("CFLAGS", "--debugger=pickit3");
    }

    var_push("CFLAGS", "--double=32");
    // var_push("CFLAGS", "--warn=-3");
    var_push("CFLAGS", "-q");
    var_push("CFLAGS", "--chip=$(CHIP)");
    var_push("LDFLAGS", "--output=mcof");
    var_push("CFLAGS", "--runtime=default,-keep,+download");
    var_push("CFLAGS", "--summary=default");
    /*var_push("CFLAGS", "--errformat=\"%f:%l:%c error [%n]: %s\"");
    var_push("CFLAGS", "--warnformat=\"%f:%l:%c warning [%n]: %s\"");*/
    var_push("LDFLAGS", "--asmlist");
    // var_push("CPPFLAGS", "-D__$(CHIP)=1");
    set_command(&commands.lib, "$(LIB) $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS)                    -pass1 -c $< -o$@");
    stralloc_copys(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@                    ^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "xc8") || str_start(compiler, "picc")) {
    // no_libs = 1;
    var_unset("CXX");
    var_set("CC", "xc8");
    var_set("LINK", "mplink");
    var_set("LIB", "mplib");
    cfg.mach.arch = PIC;
    cmd_libs = 0;
    exts.bin = ".cof";
    exts.obj = ".p1";
    exts.lib = ".lpp";
    var_push("DEFS", "-D__XC=1");
    var_set("TARGET", cfg.mach.bits == _14 ? "pic16" : "pic18");
    var_push("CPPFLAGS", cfg.mach.bits == _14 ? "-DPIC16=1" : "-DPIC18=1");

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    var_push("CFLAGS", "--mode=pro");
    var_push("CFLAGS", "--float=24");
    var_push("CFLAGS", "--double=32");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "--opt=default,+asm,-asmfile,-speed,+space,+debug,3");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "--opt=default,+asm,+asmfile,+speed,-space,-debug,9");
    else {
      var_push("CFLAGS", "--opt=default,+asm,+asmfile,-speed,-space,+debug");
      var_push("CFLAGS", "-g");
      var_push("CFLAGS", "--debugger=pickit3");
    }
    // var_push("CFLAGS", //"-fp:precise");
    var_push("CFLAGS", "-q");
    var_push("CFLAGS", "--asmlist");
    var_push("CFLAGS", "--chip=$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");
    var_push("LDFLAGS", "--output=-elf,+mcof");
    // var_push("LDFLAGS", "--output=-mcof,+elf:multilocs");
    var_push("LDFLAGS", "--summary=default");
    var_push("LDFLAGS", "--runtime=default,+clear,+init,-keep,-no_startup,-osccal,-             esetbits,+download,+clib");
    // var_push("LDFLAGS", "--output=-default,elf,+mcof");
    // var_push("LDFLAGS", "--output=-mcof,+elf");
    var_push("LDFLAGS", "--stack=compiled");

    /*var_push("CFLAGS", "--errformat=\"%f:%l:%c error [%n]: %s\"");
    var_push("CFLAGS", "--warnformat=\"%f:%l:%c warning [%n]: %s\"");*/

    stralloc_copys(&commands.preprocess, "$(CPP) $(CPPFLAGS) $(DEFS) $< -o$@");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_CFLAGS)                    (CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@                    ^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&commands.lib,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS)                    -OUTPUT=lpp --memorysummary -G -m$@.map -P --asmlist                    "
                   "-output=default,-inhx032 --output=-mcof,+elf:multilocs -o$@.elf                    ^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else {
    return 0;
  }

  if(cfg.build_type == BUILD_TYPE_DEBUG) {
    var_push("DEFS", "-D_DEBUG=1");
  } else {
    var_push("DEFS", "-DNDEBUG=1");

    if(str_equal(exts.src, ".c"))
      var_push("CFLAGS", cfg.build_type == BUILD_TYPE_MINSIZEREL ? "-O1" : "-O2");
  }

  if(cfg.mach.arch == PIC) {
    stralloc chipdef;

    stralloc_init(&chipdef);
    stralloc_cat(&chipdef, &cfg.chip);
    stralloc_lower(&chipdef);
    stralloc_cats(&chipdef, "=1");
    stralloc_inserts(&chipdef, "-D__", 0);

    var_push_sa("CPPFLAGS", &chipdef);
    stralloc_free(&chipdef);
  }

  if(cfg.sys.os == OS_WIN) {
    // push_lib("EXTRA_LIBS", "advapi32");
    /*  if(str_start(compiler, "dmc"))
        push_lib("EXTRA_LIBS", "wsock32"); else
        push_lib("EXTRA_LIBS", "ws2_32");
    */
    push_lib("EXTRA_LIBS", "kernel32");
  }

  if(cygming) {
    if(build_tool != TOOL_NINJA)
      pathsep_args = '/';
    var_set("prefix", "/");
    var_push("prefix", str_start(tools.toolchain, "mingw") ? tools.toolchain : "usr");

    if(cygming && 0) {
      var_t* cross = var_set("CROSS_COMPILE", str_end(tools.toolchain, "64") ? "x86_64" : "i686");
      // cross->sep = '-';
      stralloc_cats(&cross->value.sa, str_start(tools.toolchain, "mingw") ? "-w64-" : "-pc-");
      stralloc_cats(&cross->value.sa, str_start(tools.toolchain, "mingw") ? "mingw32" : str_start(tools.toolchain, "msys") ? "msys" : "cygwin");
      stralloc_catc(&cross->value.sa, '-');
    }
  }

  if(var_isset("CROSS_COMPILE")) {
    stralloc_prepends(&commands.compile, "$(CROSS_COMPILE)");
    stralloc_prepends(&commands.lib, "$(CROSS_COMPILE)");
    stralloc_prepends(&commands.link, "$(CROSS_COMPILE)");
  }

  return 1;
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
  path_normalize(dir, &tmp);

  if(strlist_push_unique_sa(&link_dirs, &tmp)) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Added to lib dirs: ");
    buffer_putsa(buffer_2, &tmp);
    buffer_putnlflush(buffer_2);
#endif
  }

  stralloc_free(&tmp);
}

/**
 * @brief        Show command line usage
 *
 * @param  argv0 The errmsg arguments array 0
 */
static void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [sources...]\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "\n"
                       "  -o, --output FILE         write to file\n"
                       "  -f, --input-file FILE     read from input file\n"
                       "\n"
                       "  -T, --objext EXT          object file extension\n"
                       "  -B, --exeext EXT          binary file extension\n"
                       "  -X, --libext EXT          library file extension\n"
                       "\n"
                       "      --create-libs         create rule_map for libraries\n"
                       "      --create-objs         create rule_map for objects\n"
                       "      --create-bins         create rule_map for programs\n"
                       "      --create-module       create rule_map for loadable module\n"
                       "  -i, --install             create installation rule_map\n"
                       "\n"
                       //"  -R, --objdir   DIR        object directory\n"
                       "  -O, --outdir   DIR        output directory\n"
                       "  -d, --builddir DIR        build directory\n"
                       "  -a, --arch                set architecture\n"
                       "  -s, --system OS           set operating system\n"
                       "  -c, --cross TARGET        set cross compiler\n"
                       "\n"
                       "  -D, --define NAME[=VALUE] add a preprocessor definition\n"
                       "  -I, --include-path DIR    add an include directory\n"
                       "  -L, --library-path DIR    add a library directory\n"
                       "  -l, --link LIB            link a library\n"
                       "\n"
                       "  -t, --compiler-type TYPE  compiler type, one of:\n"
                       "\n"
                       "  -S, --build-as-lib TARGET build target as library\n"
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
                       "  Specify build type:\n\n"
                       "    --debug            with debugging information, no                        ptimization\n"
                       "    --relwithdebinfo   with debugging information, optimize for                        peed\n"
                       "    --release          optimize for speed\n"
                       "    --minsizerel       optimize for size\n"
                       "\n",
                       NULL);
  buffer_putnlflush(buffer_1);
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
  stralloc tmp;
  strlist cmdline;
  int c, ret = 0, index = 0;
  const char *s, *objdir = NULL;
  set_t toks;
  buffer filebuf, *out = buffer_1;
  size_t n;
  target *all = 0, *compile = 0;
  char **it, **arg, **ptr, *x;
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
  stralloc_init(&tmp);

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

  if((s = env_get("PATH")) == 0)
    s = "/usr/local/bin:/usr/bin:/bin";

  path_split(s, &system_path, PATHSEP_C);
  strlist_init(&vpath, ' ');
  strlist_init(&cmdline, ' ');
  strlist_fromv(&cmdline, (const char**)argv, argc);

  strlist_init(&dirs.this, pathsep_make);
  strlist_init(&dirs.out, pathsep_make);
  strlist_init(&dirs.build, pathsep_make);
  strlist_init(&dirs.work, pathsep_make);

  exts.src = ".c";
  exts.inc = ".h";

  for(;;) {
    const char* arg;
    c = unix_getopt_long(argc, argv, "habo:O:B:E:d:t:m:n:a:D:l:I:c:s:p:P:R:S:if:CW:w:L:O:T:X:x:", opts, &index);

    if(c == -1)
      break;

    if(c == 0)
      continue;

    arg = unix_optarg ? unix_optarg : argv[unix_optind];

    switch(c) {
      case 'h': {
        usage(argv[0]);
        ret = 0;
        goto quit;
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
        stralloc_copys(&dirs.build.sa, arg);
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

      case 'l': {
        strarray_push(&libs, arg);
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
#ifdef DEBUG_OUTPUT_
        buffer_puts(debug_buf, "Add -I: ");
        buffer_puts(debug_buf, arg);
        buffer_putnlflush(debug_buf);
#endif
        strarray_push(&includes, arg);
        break;
      }

      case 'L': {
#ifdef DEBUG_OUTPUT_
        buffer_puts(debug_buf, "Add -L: ");
        buffer_puts(debug_buf, arg);
        buffer_putnlflush(debug_buf);
#endif
        strarray_push(&libdirs, arg);
        break;
      }

        /* debug log */
      case 'x': {
        if(buffer_appendfile(&debug_buffer, arg)) {
          errmsg_warnsys("Failed opening ", arg, " for output", 0);
          return 127;
        }

        break;
      }

      default:
        buffer_puts(buffer_2, "No such option '");
        buffer_putc(buffer_2, c);
        buffer_putsflush(buffer_2, "'\n");
        // usage(argv[0]);
        ret = 1;
        goto quit;
    }
  }

  if(!cmd_bins && !cmd_libs && !cmd_objs) {
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

  path_getcwd(&dirs.this.sa);

  if(cfg.build_type == -1)

    if((cfg.build_type = extract_build_type(&dirs.build.sa)) == -1)

      if((cfg.build_type = extract_build_type(&dirs.this.sa)) == -1)
        cfg.build_type = extract_build_type(&dirs.out.sa);

  if(cfg.build_type == -1)
    cfg.build_type = BUILD_TYPE_DEBUG;

  if(tools.make == NULL && tools.compiler) {
    if(str_start(tools.compiler, "b")) {
      tools.make = "borland";

    } else if(str_start(tools.compiler, "msvc")) {
      tools.make = "nmake";
    } else if(str_start(tools.compiler, "g")) {
      tools.make = "gmake";
      tool_config |= MAKE_PATTERN_RULES;
    } else if(str_start(tools.compiler, "o")) {
      tools.make = "omake";
    } else if(str_start(tools.compiler, "po")) {
      tools.make = "pomake";
    }
  }

  if(tools.toolchain)
    cygming = str_start(tools.toolchain, "mingw") || str_start(tools.toolchain, "cyg") || str_start(tools.toolchain, "msys");

  if(cygming) {
    tools.compiler = "gcc";

    if(tools.make == 0)
      tools.make = "gmake";
  }

  if(tools.make == NULL)
    tools.make = "make";

  build_tool = (str_start(tools.make, "bat") || str_start(tools.make, "cmd")) ? TOOL_BATCH
               : tools.make[str_find(tools.make, "ninja")] != '\0'            ? TOOL_NINJA
               : str_start(tools.make, "sh")                                  ? TOOL_SHELL
                                                                              : 0;

  if(build_tool == TOOL_BATCH)
    comment = "REM ";

  /*if(tools.compiler == NULL) {
    if(cfg.mach.arch == PIC)
      tools.compiler = "xc8";

    if(tools.compiler == NULL)
      tools.compiler = "gcc";
    else if(cfg.mach.bits == 0)
      set_machine(tools.compiler);
  }*/

  set_init(&toks, 0);

  {
    strlist tmp;
    strlist_init(&tmp, '\0');
    stralloc_copy(&tmp.sa, &dirs.build.sa);

    if(outfile)
      strlist_push(&tmp, outfile);
    stralloc_replacec(&tmp.sa, '/', '\0');
    stralloc_replacec(&tmp.sa, '-', '\0');
    stralloc_replacec(&tmp.sa, '.', '\0');
    strlist_foreach(&tmp, s, n) { set_add(&toks, s, n); }

#ifdef DEBUG_OUTPUT_
    buffer_puts(debug_buf, "toks: ");
    buffer_putset(debug_buf, &toks, " ", 1);
#endif
  }

  {
    size_t i;
    set_iterator_t it;
    stralloc tok;

    stralloc_init(&tok);

    set_foreach(&toks, it, s, n) {
      stralloc_copyb(&tok, s, n);
      stralloc_nul(&tok);

      if(set_compiler_type(tok.s)) {
        tools.compiler = (char*)s;
        break;
      }

      if(cfg.build_type == -1) {
        for(i = 0; i < (sizeof(build_types) / sizeof(build_types[0])); ++i) {
          if(s[case_find(s, build_types[i])]) {
            cfg.build_type = i;
            break;
          }
        }
      }
    }

    stralloc_free(&tok);
  }

  if(tools.compiler)
    if(!set_make_type() || !set_compiler_type(tools.compiler)) {
      usage(argv[0]);
      ret = 2;
      goto quit;
    }

  if(*cross_compile) {
    var_set("CROSS_COMPILE", cross_compile);

    if(var_isset("CC"))
      stralloc_prepends(&var_list("CC", pathsep_args)->value.sa, "$(CROSS_COMPILE)");

    if(var_isset("CXX"))
      stralloc_prepends(&var_list("CXX", pathsep_args)->value.sa, "$(CROSS_COMPILE)");

    if(var_isset("AR"))
      stralloc_prepends(&var_list("AR", pathsep_args)->value.sa, "$(CROSS_COMPILE)");
  }

  batchmode = build_tool == TOOL_BATCH && stralloc_contains(&commands.compile, "-Fo");

  if(build_tool == TOOL_BATCH)
    pathsep_args = pathsep_make;

  stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);

  strlist_nul(&dirs.out);
  strlist_nul(&dirs.this);

  if(outfile)
    path_dirname(outfile, &dirs.work.sa);
  else
    stralloc_copys(&dirs.work.sa, "build");

  if(dirs.out.sa.len == 0)
    path_concat_sa(&dirs.this.sa, &dirs.work.sa, &dirs.out.sa);

  if(dirs.build.sa.len == 0) {
    if(strlist_contains(&dirs.work, "build") && strlist_count(&dirs.work) > 1) {
      stralloc_copy(&dirs.build.sa, &dirs.work.sa);

    } else if(tools.toolchain && !strlist_contains(&dirs.this, "build")) {
      stralloc target;

      stralloc_init(&target);
      stralloc_copys(&target, tools.toolchain);

      if(cfg.chip.s) {
        stralloc_cats(&target, "-");
        stralloc_cat(&target, &cfg.chip);
      }

      stralloc_nul(&target);
      stralloc_copy(&dirs.build.sa, &dirs.work.sa);

      if(cross_compile && *cross_compile) {
        strlist_push(&dirs.build, cross_compile);
      } else {
        strlist_push_sa(&dirs.build, &target);
        stralloc_catc(&dirs.build.sa, '-');
        stralloc_cats(&dirs.build.sa, build_types[cfg.build_type]);
      }

      stralloc_free(&target);
    }

    stralloc_replacec(&dirs.build.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  }

  if(dirs.work.sa.len == 0)
    stralloc_copys(&dirs.work.sa, ".");

  path_absolute_sa(&dirs.out.sa);
  path_canonical_sa(&dirs.out.sa);
  path_collapse_sa(&dirs.out.sa);

  path_absolute_sa(&dirs.build.sa);
  path_canonical_sa(&dirs.build.sa);
  path_collapse_sa(&dirs.build.sa);

  strlist_nul(&dirs.this);
  strlist_nul(&dirs.out);
  strlist_nul(&dirs.build);
  strlist_nul(&dirs.work);

  // debug_sa("dirs.this", &dirs.this.sa);
  // debug_sa("dirs.out", &dirs.out.sa);
  // debug_sa("dirs.build", &dirs.build.sa);

  if(tools.preproc)
    var_set("CPP", tools.preproc);

  strarray_foreach(&libdirs, it) { push_linkdir("LIBS", *it); }
  strarray_foreach(&libs, it) { with_lib(*it); }
  strarray_foreach(&includes, it) { includes_add(*it); }

  includes_cppflags();

  // debug_sa("dirs.work", &dirs.work.sa);
  strlist_nul(&dirs.this);
  strlist_nul(&dirs.out);
  path_relative_to(dirs.this.sa.s, dirs.out.sa.s, &sources_dir);
  stralloc_nul(&sources_dir);
  // debug_sa("srcdir", &srcdir);

  if(dirs.out.sa.len) {
    stralloc_replacec(&dirs.this.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    // debug_sa("dirs.this", &dirs.this.sa);
    // debug_sa("dirs.out", &dirs.out.sa);
    path_absolute_sa(&dirs.out.sa);
    stralloc_zero(&tmp);
    path_relative_to(dirs.this.sa.s, dirs.out.sa.s, &tmp);
    stralloc_copy(&sources_dir, &tmp);
    stralloc_nul(&sources_dir);

    stralloc_zero(&tmp);
  }

  path_relative_to(dirs.build.sa.s, dirs.out.sa.s, &tmp);

  strlist_nul(&dirs.work);
  stralloc_replacec(&dirs.work.sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

  mkdir_components(&dirs.out, 0755);

  if(stralloc_diffs(&dirs.work.sa, "."))
    mkdir_components(&dirs.work, 0755);

  mkdir_components(&dirs.build, 0755);

  if(outfile) {
    if('\\' != PATHSEP_C)
      stralloc_replacec(&dirs.out.sa, '\\', PATHSEP_C);

    if(stralloc_equals(&dirs.out.sa, "."))
      stralloc_zero(&dirs.out.sa);
    else
      stralloc_catc(&dirs.out.sa, pathsep_make);

    byte_zero(&filebuf, sizeof(filebuf));

    if(buffer_truncfile(&filebuf, outfile)) {
      errmsg_warnsys("ERROR: opening '", outfile, "'", 0);
      ret = 2;
      goto quit;
    }

    out = &filebuf;
    // path_absolute_sa(&dirs.out.sa);
  }

  all = rule_get("all");
  all->phony = true;

  stralloc_catc(&all->recipe, '\n');

  if(tool_config & (MAKE_PATTERN_RULES | MAKE_IMPLICIT_RULES)) {
    stralloc rn;
    bool outputs = false;

    stralloc_init(&rn);

    if(tool_config & MAKE_PATTERN_RULES) {
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

    compile = rule_get_sa(&rn);
    compile->outputs = outputs;

    stralloc_free(&rn);
  }

  if(str_equal(exts.src, ".asm")) {
    target* assemble = rule_get(".asm.o");

    stralloc_copy(&assemble->recipe, &commands.compile);
  }

  // strarray_init(&sources_list);

  if(infile) {
    input_process_file(infile, all);

    MAP_PAIR_T iter;

    MAP_FOREACH(rule_map, iter) {
      target* rule = MAP_ITER_VALUE(iter);
      static const char* varnames[] = {
          "CC",
          "CFLAGS",
          "CPPFLAGS",
          "LDFLAGS",
          0,
      };
      bool modified = false;

      for(size_t i = 0; varnames[i]; ++i) {
        const char* value;
        //  char namebuf[64];

        if(!(value = var_get(varnames[i])) || !value[0])
          continue;

#ifdef DEBUG_OUTPUT_
        buffer_putm_internal(debug_buf, "Rule: ", rule->name, 0);
        buffer_putm_internal(debug_buf, " ", "Variable: ", varnames[i], 0);
        buffer_putm_internal(debug_buf, " ", "Value: ", value, 0);
        buffer_putnlflush(debug_buf);
#endif

        if(stralloc_contains(&rule->recipe, value)) {
          size_t j = 0;
          stralloc tmp;
          stralloc_init(&tmp);

          var_subst_sa(varnames[i], &tmp, &rule->recipe, 0, 0);

          stralloc_copy(&rule->recipe, &tmp);
          stralloc_nul(&rule->recipe);
          stralloc_free(&tmp);

          modified = true;
        }
      }

      set_subst_sa(&rule->prereq, &rule->recipe, rule->type == COMPILE ? "$[<]" : "$[^]");
      set_subst_sa(&rule->output, &rule->recipe, "$@");

      if(modified) {
        buffer_putm_internal(buffer_1, "Modified: ", rule->name, 0);
        buffer_putm_internal(buffer_1, " ", "Recipe: ", rule->recipe.s, 0);
        buffer_putnlflush(buffer_1);
      }
    }

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "build_directories =\n\t");
    buffer_putset(buffer_2, &build_directories, "\n\t", 2);
    buffer_putnlflush(buffer_2);
#endif

    {
      stralloc builddir;

      stralloc_init(&builddir);

      set_at_sa(&build_directories, 0, &builddir);

      var_setb(builddir_varname, builddir.s, builddir.len);
      stralloc_free(&builddir);
    }
  }

  if(compile)
    stralloc_weak(&compile->recipe, &commands.compile);

  if(compile) {
    MAP_PAIR_T it;

    strlist_nul(&dirs.work);
    strlist_push_unique(&vpath, ".");
    strlist_push_unique_sa(&vpath, &dirs.work.sa);

    set_clear(&compile->output);

    MAP_FOREACH(rule_map, it) {
      target* rule = MAP_ITER_VALUE(it);

      if(rule_is_compile(rule) && rule != compile) {
        stralloc_free(&rule->recipe);
        stralloc_init(&rule->recipe);

        if(str_equal(tools.make, "gmake"))
          rule->disabled = 1;

        // set_cat(&compile->prereq, &rule->prereq);
        set_cat(&compile->output, &rule->output);
      }
    }
  }

  while(unix_optind < argc) {
    stralloc arg;

    stralloc_init(&arg);
    stralloc_copys(&arg, argv[unix_optind]);
    stralloc_nul(&arg);

    if(stralloc_contains(&arg, "=")) {
      size_t eqpos;
      const char* v;
      // debug_sa("Setting var", &arg);
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
      strarray_glob(&args, arg.s);
    else
#endif

      strarray_push(&args, arg.s);
    ++unix_optind;

    stralloc_free(&arg);
  }

  path_canonical_sa(&dirs.out.sa);
  path_collapse_sa(&dirs.out.sa);

#ifdef DEBUG_OUTPUT
  debug_sa("dirs.work", &dirs.work.sa);
  debug_sa("dirs.build", &dirs.build.sa);
  debug_sa("dirs.out", &dirs.out.sa);
  debug_sa("dirs.this", &dirs.this.sa);
  buffer_putnlflush(debug_buf);
#endif

  /* No arguments given */

  if(strarray_size(&args) == 0 && !infile) {
    buffer_putsflush(buffer_2, "ERROR: No arguments given\n\n");
    usage(argv[0]);
    ret = 1;
    goto quit;
  }

  generate_mkdir_rule(&dirs.work.sa);
  add_path_sa(&all->prereq, &dirs.work.sa);

#ifdef DEBUG_OUTPUT_
  {
    size_t n;

    buffer_puts(buffer_2, "dirs.out[]: ");
    buffer_putulong(buffer_2, strlist_count(&dirs.out));
    buffer_puts(buffer_2, ", dirs.out[0]: ");
    buffer_put(buffer_2, strlist_at_n(&dirs.out, 0, &n), n);
    buffer_putnlflush(buffer_2);
  }
#endif

  if(!stralloc_equals(&dirs.out.sa, "./")) {
    generate_mkdir_rule(&dirs.out.sa);
    add_path_sa(&all->prereq, &dirs.out.sa);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "args: ");
  strarray_dump(buffer_2, &args);
#endif

  strarray_foreach(&args, arg) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "argument: ");
    buffer_puts(buffer_2, *arg);
    buffer_putnlflush(buffer_2);
#endif
#if 0 // WINDOWS_NATIVE
    glob_t gl;
    size_t i;

    if(glob(*arg, GLOB_TILDE | GLOB_BRACE, 0, &gl)) {
      buffer_putm_internal(buffer_2, "ERROR: glob() ", *arg, newline, NULL);
      buffer_flush(buffer_2);
      continue;
    }

    for(i = 0; i < gl.gl_matchc; i++)
#else

#endif

    {
#if 0 // WINDOWS_NATIVE
      const char* p = gl.gl_pathv[i];
#else
      const char* p = *arg;
#endif

      if(!path_exists(p)) {
        buffer_putm_internal(buffer_2, "ERROR: Doesn't exist: ", p, newline, NULL);
        buffer_flush(buffer_2);
        ret = 127;
        goto fail;
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

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "adding to sources: ");
      buffer_put(buffer_2, x, n);
      buffer_putnlflush(buffer_2);
#endif

      if(is_source_b(x, n))
        strarray_pushb(&sources, x, n);
    }

    strarray_sort(&sources, &sources_sort_callback);
  }

  if(str_start(tools.make, "g")) {
    stralloc builddir;

    stralloc_init(&builddir);
    path_relative_to(dirs.build.sa.s, dirs.work.sa.s, &builddir);
    stralloc_nul(&builddir);

    if(!stralloc_endc(&dirs.work.sa, PATHSEP_C))
      stralloc_catc(&dirs.work.sa, PATHSEP_C);
    var_set(builddir_varname, dirs.work.sa.s);
    stralloc_copys(&dirs.build.sa, "$(BUILDDIR)");
    stralloc_free(&builddir);
  }

  if(((build_tool == TOOL_BATCH || build_tool == TOOL_SHELL) && stralloc_equals(&dirs.work.sa, ".")))
    batchmode = 1;

  if(output_name.len) {
    project_name = str_ndup(output_name.s, output_name.len);
  } else {
    stralloc abspath;

    stralloc_init(&abspath);
    path_absolute(dirs.this.sa.s, &abspath);
    stralloc_nul(&abspath);
    project_name = str_dup(path_basename(abspath.s));
    stralloc_free(&abspath);
  }

  if(!infile) {
    int link_rules = 0;
    stralloc src;
    strarray sources2;

    stralloc_init(&src);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "strarray sources: ");
    strarray_dump(buffer_2, &sources);
    buffer_putnlflush(buffer_2);
#endif

    strarray_init(&sources2);
    strarray_copy(&sources2, &sources);

    strarray_foreach(&sources2, ptr) { sourcedir_addsource(*ptr, &sources, &progs, &bins, pathsep_make); }

    sourcedir_populate(&sources);
    strarray_free(&sources);
    stralloc_free(&src);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "targetdirs:\n");

    MAP_FOREACH(targetdirs, t) {
      uint32* count_ptr = (uint32*)MAP_ITER_VALUE(t);

      buffer_puts(buffer_2, "  '");
      buffer_puts(buffer_2, MAP_ITER_KEY(t));
      buffer_puts(buffer_2, "' => ");
      buffer_putulong(buffer_2, *count_ptr);
      buffer_putnlflush(buffer_2);
    }
    buffer_putnlflush(buffer_2);
#endif

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "cmd_libs = ");
    buffer_putlong(buffer_2, cmd_libs);
    buffer_puts(buffer_2, " cmd_bins = ");
    buffer_putlong(buffer_2, cmd_libs);
    buffer_puts(buffer_2, " cmd_objs = ");
    buffer_putlong(buffer_2, cmd_objs);
    buffer_putnlflush(buffer_2);
#endif

#ifdef DEBUG_OUTPUT_
    MAP_FOREACH(targetdirs, t) {
      uint32* count_ptr = (uint32*)MAP_ITER_VALUE(t);

      buffer_puts(buffer_2, "  '");
      buffer_puts(buffer_2, MAP_ITER_KEY(t));
      buffer_puts(buffer_2, "' => ");
      buffer_putulong(buffer_2, *count_ptr);
      buffer_putnlflush(buffer_2);
    }
    buffer_putnlflush(buffer_2);
    sourcedir_dump_all(buffer_2, sourcedir_map);
#endif

    if(cmd_libs) {
      generate_lib_rules(build_tool == TOOL_SHELL, build_tool == TOOL_BATCH, batchmode, pathsep_args, pathsep_make);
      deps_for_libs();
    } else {
      MAP_PAIR_T t;
      MAP_FOREACH(sourcedir_map, t) {
        sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
        /*if(tools.preproc) {
          generate_simple_compile_rules(rule_map, srcdir, MAP_ITER_KEY(t),
        exts.src, exts.pps, &commands.preprocess);
          generate_simple_compile_rules(rule_map, srcdir, MAP_ITER_KEY(t),
        exts.pps, exts.obj, &commands.compile); } else */
        { generate_simple_compile_rules(srcdir, MAP_ITER_KEY(t), exts.src, exts.obj, &commands.compile, pathsep_args); }
      }
    }

    if(cmd_bins) {
      int ret;

#ifdef DEBUG_OUTPUT
      buffer_puts(debug_buf, "sources_list.length = ");
      buffer_putulong(debug_buf, dlist_length(&sources_list));
      buffer_putnlflush(debug_buf);
#endif

      if(!(ret = generate_link_rules(pathsep_args, pathsep_make)))
        cmd_bins = 0;

      link_rules += ret;

#ifdef DEBUG_OUTPUT
      buffer_puts(debug_buf, "bins = ");
      buffer_putstra(debug_buf, &bins, ", ");
      buffer_putnlflush(debug_buf);
#endif

#ifdef DEBUG_OUTPUT
      buffer_puts(debug_buf, "progs = ");
      buffer_putstra(debug_buf, &progs, ", ");
      buffer_putnlflush(debug_buf);
#endif
    }

    if(cmd_module) {
    }

    if(cmd_bins == 0 || cmd_libs == 1) {
      MAP_PAIR_T t;

      MAP_FOREACH(rule_map, t) {
        target* tgt = MAP_ITER_VALUE(t);

        if(stralloc_equal(&tgt->recipe, &commands.lib) && cmd_libs)
          set_adds(&all->prereq, MAP_ITER_KEY(t));
      }
    }
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

  {
    MAP_PAIR_T t;

    MAP_FOREACH(sourcedir_map, t) {
      sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);

#if DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "key: ", t->key, " pptoks: ", NULL);
      buffer_putset(buffer_2, &srcdir->pptoks, ", ", 2);
#endif
    }
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(debug_buf, "Dumping all rule_map...\n");

  {
    int i = 0;
    MAP_PAIR_T t;

    MAP_FOREACH(rule_map, t) {
      target* rule = MAP_ITER_VALUE(t);

      buffer_puts(debug_buf, PINK256 "Rule" NC " #");
      buffer_putlong(debug_buf, ++i);
      rule_dump(rule);
      buffer_putnlflush(debug_buf);
    }
  }
#endif
fail:

  if(!case_diffs(tools.make, "mplab")) {
    output_mplab_project(out, 0, 0, &include_dirs);
    goto quit;
  }

#ifdef DEBUG_OUTPUT_
  {
    strlist rule_names;

    strlist_init(&rule_names, '\0');

    map_keys_get(&rule_map, &rule_names);

    buffer_puts(debug_buf, "rule_names:\n\t");
    buffer_putsl(debug_buf, &rule_names, " \\\n\t");
    buffer_putnlflush(debug_buf);
    strlist_free(&rule_names);
  }
#endif

  if(!case_diffs(tools.make, "cmake")) {
    output_cmake_project(out, &rule_map, &vars, &include_dirs, &link_dirs);
    goto quit;
  }

  buffer_putm_internal(out, comment, " Generated by:", newline, comment, "  ", NULL);
  buffer_putsa(out, &cmdline.sa);
  buffer_putsflush(out, newline);
  stralloc_nul(&cfg.chip);
  var_set("CHIP", cfg.chip.s);

  if(build_tool == TOOL_NINJA) {
    stralloc tmp;

    stralloc_init(&tmp);
    path_relative_to_sa(&dirs.build.sa, &dirs.out.sa, &tmp);

    while(stralloc_endb(&tmp, &pathsep_args, 1))
      tmp.len--;

    var_setb("objdir", tmp.s, tmp.len);
    var_set("extra_cflags", "$$EXTRA_CFLAGS");
    var_set("extra_ldflags", "$$EXTRA_LDFLAGS");

    stralloc_free(&tmp);
  }

#ifdef DEBUG_OUTPUT
  {
    strlist varnames;

    strlist_init(&varnames, '\0');
    map_keys_get(&vars, &varnames);
    buffer_puts(debug_buf, "varnames: ");
    strlist_dump(debug_buf, &varnames);
    output_all_vars(out, &vars, &varnames, build_tool);
    strlist_free(&varnames);
  }
#endif

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

#if DEBUG_OUTPUT_
      buffer_puts(debug_buf, "Empty RULE '");
      buffer_puts(debug_buf, name);
      buffer_putc(debug_buf, '\'');
      buffer_putnlflush(debug_buf);
#endif
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

quit : {
  strlist deps;
  MAP_PAIR_T t;

  strlist_init(&deps, '\0');

  MAP_FOREACH(sourcedir_map, t) {
    sourcedir* sdir = *(sourcedir**)MAP_ITER_VALUE(t);

    if(1 /* && set_size(&sdir->deps)*/) {
      strlist_zero(&deps);
      sourcedir_deps(sdir, &deps);

#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(debug_buf, "source directory '", MAP_ITER_KEY(t), "' deps =\n", NULL);
      strlist_dump(debug_buf, &deps);
      buffer_putnlflush(debug_buf);
#endif
    }
  }

  strlist_free(&deps);
}

  {
    struct dnode* link;
    strlist deps;

    strlist_init(&deps, '\0');

    dlist_foreach_down(&sources_list, link) {
      sourcefile* source = dlist_data(link, sourcefile*);

#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(debug_buf, "source: ", source->name, " deps: ", NULL);
      strlist_zero(&deps);
      sources_deps(source, &deps);
      buffer_puts(debug_buf, " includes: ");
      strlist_dump(debug_buf, &source->includes);
      buffer_putnlflush(debug_buf);
#endif
    }

    strlist_free(&deps);
  }

  strarray_free(&args);
  strarray_free(&sources);
  strlist_free(&cmdline);

  strarray_free(&libs);
  strarray_free(&libdirs);
  strarray_free(&includes);
  stralloc_free(&tmp);

  MAP_DESTROY(sourcedir_map);

  return ret;
}
