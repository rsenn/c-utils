#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "../../lib/uint32.h"
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
#endif
#include "compiler.h"

static const char* compiler_types[] = {
    "8cc", "bcc",    "clang",  "digitalmars", "dmc", "gcc",  "gnu", "gp", "htc",  "icl", "lcc",   "llvm", "msvc",
    "occ", "orange", "pelles", "picc",        "po",  "sdcc", "tcc", "vc", "vs20", "xc8", "zapcc", 0,
};

const char *make_begin_inline, *make_sep_inline, *make_end_inline, *comment = "#", *cross_compile = "", *quote_args = "";
bool batchmode, cygming;
build_tool_t build_tool = 0;

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

/* what bcc32 5.5 defines on its own for -tWC -tWM, found by compiling probes */
static const char* const bcc55_defs[] = {
    "__BORLANDC__=0x0551", "__TURBOC__=0x0551", "_M_IX86=300",   "__WIN32__=1",
    "_WIN32=1",            "_Windows=1",        "__CONSOLE__=1", "__FLAT__=1",
    "__CDECL__=1",         "__TLS__=1",         "_CPPUNWIND=1",  "_INTEGRAL_MAX_BITS=64",
    "_STDCALL_SUPPORTED=", "__MT__=1",          "_MT=1",         0,
};

const char* const*
compiler_predefs(const char* compiler) {
  if(compiler && str_start(compiler, "bcc") && str_contains(compiler, "55"))
    return bcc55_defs;

  return 0;
}

/**
 * @brief         Set the machine type
 *
 * @param      s     The new value
 *
 * @return     1 on success
 */
int
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
int
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
int
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
int
set_make_type(void) {
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

    if(cfg.sys.os == OS_WIN) {
      stralloc_copys(&commands.mkdir, "IF NOT EXIST $@ MKDIR $@");
      stralloc_copys(&commands.delete, "DEL /F");
    } else {
      stralloc_copys(&commands.mkdir, "test -d $@ || mkdir -p $@");
      stralloc_copys(&commands.delete, "rm -f");
    }
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

static bool
is_compiler(const char* s) {
  for(int i = 0; compiler_types[i]; i++)
    if(str_start(s, compiler_types[i]))
      return true;

  return false;
}

/**
 * @brief      Set the tools.compiler type
 *
 * @param      compiler  The compiler
 *
 * @return      1 on success
 */
int
set_compiler_type(const char* compiler) {
  var_set("CC", "cc");
  var_set("CXX", "c++");

  stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");

  set_command(&commands.lib, "$(LIB) /out:$@", "$^");
  set_command(&commands.link,
              "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@",
              "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  set_command(&commands.preprocess, "$(CPP) $(CPPFLAGS) $(DEFS) -o$@", "$<");

  /*
   * Visual C++ compiler
   */

  if(str_start(compiler, "msvc") || str_start(compiler, "icl") || str_start(compiler, "vs20") ||
     str_start(compiler, "vc") || compiler[str_find(compiler, "-cl")]) {
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

    stralloc_copys(&commands.compile,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
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

    var_push("LDFLAGS",
             "-libpath:\"$(UNIVERSALCRTSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)ucrt\\$(MACHINE)\"");
    var_push("LDFLAGS",
             "-libpath:\"$(WINDOWSSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)um\\$(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCTOOLSINSTALLDIR)lib\\$(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(WINDOWSSDKDIR)lib$(X64)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\lib$(AMD64)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\PlatformSDK\\lib$(AMD64)\"");
    var_push("LDFLAGS", "-incremental -manifest");

    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("LDFLAGS", "-debug");

    if(str_start(compiler, "icl"))
      var_push("LDFLAGS",
               "-manifest:embed -manifestuac:\"level='asInvoker' uiAccess='false'\"");

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

    set_command(&commands.link,
                "$(LINK) -out:$@ $(LDFLAGS) $(EXTRA_LDFLAGS) -pdb:\"$@.pdb\"",
                "$^ $(LIBS) $(EXTRA_LIBS)");
  } else if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming || str_start(compiler, "clang") ||
            str_start(compiler, "llvm") || str_start(compiler, "zapcc")) {
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

    /* --create-module: every source becomes its own -shared, -fPIC
     * loadable module (foo.c -> foo.so, no "lib" prefix) instead of an
     * executable -- see generate_module_rules(). -fPIC has to be a
     * compile flag (applies to every object genmakefile compiles in
     * this run), while -shared only belongs on the module's own link
     * recipe, kept separate from the ordinary commands.link so it
     * never leaks into a --create-bins/--create-libs run. */
    if(cmd_module)
      var_push("CFLAGS", "-fPIC");

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
    // set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS)
    // $(EXTRA_LIBS)");

    set_command(&commands.link,
                "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@",
                "$^ $(LIBS)");

    set_command(&commands.link_module,
                "$(CC) -shared $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@",
                "$^ $(LIBS)");

    /* cygming (mingw/msys/cygwin cross toolchain, selected via
     * -c/--cross) targets Windows regardless of the host this
     * genmakefile binary happened to be built on -- exts.bin/exts.slib
     * otherwise stay at their DEFAULT_* values baked in at genmakefile's
     * own compile time (see the cfg struct init and genmakefile.h),
     * which is wrong (.so instead of .dll) when cross-building. */
    exts.bin = cygming ? ".exe" : "";
    exts.slib = cygming ? ".dll" : DEFAULT_DSOEXT;

    format_linklib_fn = &format_linklib_switch;

    /*
     * Borland C++ Builder
     */
  } else if(str_start(compiler, "bcc")) {
    exts.obj = ".obj";
    exts.bin = ".exe";
    exts.lib = ".lib";
    format_linklib_fn = &format_linklib_lib;
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

      stralloc_copys(&commands.compile,
                     "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
      set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -e$@", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    }

    var_set("LINK", "ilink32");
    var_set("LIB", "tlib");
    var_push("STDC_LIBS", "cw32mt.lib");
    var_push("STDC_LIBS", "import32.lib");
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

    stralloc_copys(&commands.link,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&commands.link,
                   "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
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
    stralloc_copys(&commands.compile,
                   "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -o$@ $<");
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
    set_command(&commands.link,
                "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@",
                "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
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
    stralloc_copys(&commands.link,
                   "$(CC) $^ -Fe $@ $(LDFLAGS) $(EXTRA_LDFLAGS) $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
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
    stralloc_copys(&commands.link,
                   "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "sdcc")) {
    if(cmd_libs_explicit) {
      buffer_putm_internal(buffer_2,
                           "WARNING: the \"sdcc\" compiler doesn't support --create-libs, ignoring",
                           newline,
                           NULL);
      buffer_flush(buffer_2);
    }

    cmd_libs = 0;

    var_set("CC", "sdcc");
    var_set("LINK", "sdcc");
    var_set("LIB", "sdcclib");
    var_unset("CXX");
    cfg.mach.arch = PIC;
    exts.bin = ".cod";
    exts.obj = ".o";
    exts.lib = ".lib";

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

    set_command(&commands.lib, "$(LIB) r $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(&commands.link,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
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
    stralloc_copys(&commands.compile,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(&commands.link,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "xc8-cc")) {
    /* modern (v2.00+), LLVM-based XC8 driver -- gcc-like CLI, entirely
     * different from the legacy (v1.x, HI-TECH-derived) "xc8"/"picc"
     * compiler-type below, which "xc8-cc" would otherwise also match
     * via str_start(compiler, "xc8"), hence this branch comes first. */
    var_unset("CXX");
    var_set("CC", "xc8-cc");
    var_set("LIB", "xc8-ar");
    cfg.mach.arch = PIC;
    exts.bin = ".elf";
    /* despite the gcc-like CLI, "-c ... -o foo.o" still writes foo.p1 --
     * an intermediate/object file with a fixed extension, same as the
     * legacy xc8/picc driver -- not a true .o; xc8-cc ignores the
     * extension given via -o for -c and always appends .p1 itself. */
    exts.obj = ".p1";
    exts.lib = ".a";

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    var_push("CFLAGS", "-mcpu=$(CHIP)");

    /* the XC8 device pack directory -- resolved at build time from the
     * DFP_DIR environment variable (as MPLAB X's own generated
     * Makefiles do), not baked in at generation time. Unlike make,
     * ninja doesn't import the process environment into its own
     * variable namespace, and its $(NAME)->$name variable-reference
     * lowering (transform_subst_sa) would otherwise turn this into a
     * reference to a ninja variable that's never defined (silently
     * expanding to empty). Route around that for ninja with a
     * doubly-escaped $$DFP_DIR: ninja unescapes "$$" to a literal "$"
     * once at parse time and doesn't reinterpret it afterwards, so the
     * generated command line ends up with a literal $DFP_DIR that the
     * shell -- not ninja -- expands from its real environment. */
    var_set("MDFP", build_tool == TOOL_NINJA ? "$$DFP_DIR/xc8" : "$(DFP_DIR)/xc8");
    var_push("CFLAGS", "-mdfp=\"$(MDFP)\"");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "-Os");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-O2");

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_push("CFLAGS", "-gdwarf-3");

    var_push("CPPFLAGS", "-D__$(CHIP)=1");

    set_command(&commands.lib, "$(LIB) rcs $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(&commands.link,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
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
    var_push("LDFLAGS",
             "--runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,+download,+clib");
    // var_push("LDFLAGS", "--output=-default,elf,+mcof");
    // var_push("LDFLAGS", "--output=-mcof,+elf");
    var_push("LDFLAGS", "--stack=compiled");

    /*var_push("CFLAGS", "--errformat=\"%f:%l:%c error [%n]: %s\"");
    var_push("CFLAGS", "--warnformat=\"%f:%l:%c warning [%n]: %s\"");*/

    stralloc_copys(&commands.preprocess, "$(CPP) $(CPPFLAGS) $(DEFS) $< -o$@");
    stralloc_copys(&commands.compile,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(&commands.link,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&commands.lib,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) --OUTPUT=lpp --memorysummary -G -m$@.map -P --asmlist --output=default,-inhx032 --output=-mcof,+elf:multilocs -o$@.elf $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
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

  if(!cfg.chip.len)
    if(cfg.sys.os == OS_WIN)
      push_lib("EXTRA_LIBS", "kernel32");

  if(cygming) {
    if(build_tool != TOOL_NINJA)
      pathsep_args = '/';
    var_set("prefix", "/");
    var_push("prefix", str_start(tools.toolchain, "mingw") ? tools.toolchain : "usr");

    if(cygming && 0) {
      var_t* cross = var_set("CROSS_COMPILE", str_end(tools.toolchain, "64") ? "x86_64" : "i686");
      // cross->sep = '-';
      stralloc_cats(&cross->value.sa, str_start(tools.toolchain, "mingw") ? "-w64-" : "-pc-");
      stralloc_cats(&cross->value.sa,
                    str_start(tools.toolchain, "mingw")  ? "mingw32"
                    : str_start(tools.toolchain, "msys") ? "msys"
                                                         : "cygwin");
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
 * @brief      Resolve the build type, make tool and compiler to use, and
 *             apply the selected compiler's variable/command defaults.
 *
 *             Infers `tools.make` from `tools.compiler` (or vice versa by
 *             scanning tokens split out of the build dir / output file
 *             name), classifies `build_tool` (batch/ninja/shell/make), and
 *             finally calls `set_make_type()`/`set_compiler_type()` to
 *             populate `commands`/`exts`/`var_*` for the resolved
 *             compiler. Also applies `-c/--cross` prefixing and computes
 *             `batchmode`.
 *
 *             `cfg.build_type` itself is explicit-only: it comes from
 *             `--debug`/`--release`/`--relwithdebinfo`/`--minsizerel` (see
 *             the `getopt_long` table in `main()`) or, absent any of
 *             those, stays `BUILD_TYPE_RELEASE` (0) from the `byte_zero()`
 *             reset in `main()` -- inferring it from a directory name is
 *             a non-goal.
 *
 * @param      argv0  argv[0], for usage() on failure
 *
 * @return     0 on success; nonzero is the exit code main() should return
 *             (compiler/make-tool combination couldn't be resolved)
 */
int
resolve_toolchain(const char* argv0) {
  path_getcwd(&dirs.this.sa);

  if(tools.make == NULL && tools.compiler) {
    if(str_start(tools.compiler, "b")) {
      tools.make = "borland";

    } else if(str_start(tools.compiler, "msvc")) {
      tools.make = "nmake";
    } else if(str_start(tools.compiler, "g")) {
      tools.make = "gmake";
    } else if(str_start(tools.compiler, "o")) {
      tools.make = "omake";
    } else if(str_start(tools.compiler, "po")) {
      tools.make = "pomake";
    }
  }

  if(tools.make && str_equal(tools.make, "gmake"))
    make_capabs |= MAKE_RULE_PATTERN;

  if(tools.make && str_equal(tools.make, "make"))
    make_capabs |= MAKE_RULE_IMPLICIT;

  if(tools.toolchain)
    cygming =
        str_start(tools.toolchain, "mingw") || str_start(tools.toolchain, "cyg") || str_start(tools.toolchain, "msys");

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

  /* auto-detect tools.compiler by splitting the build dir / output file
   * name into '/'-'-'-'.' separated tokens and checking each one against
   * known compiler names. */
  {
    set_t toks;
    const char* s;
    size_t n;

    set_init(&toks, 0);

    {
      strlist tmp;
      strlist_init(&tmp, '\0');
      stralloc_copy(&tmp.sa, &dirs.obj.sa);

      if(outfile)
        strlist_push(&tmp, outfile);
      stralloc_replacec(&tmp.sa, '/', '\0');
      stralloc_replacec(&tmp.sa, '-', '\0');
      stralloc_replacec(&tmp.sa, '.', '\0');
      strlist_foreach(&tmp, s, n) { set_add(&toks, s, n); }

      strlist_free(&tmp);
    }

    {
      set_iterator_t it;
      stralloc tok;

      stralloc_init(&tok);

      set_foreach(&toks, it, s, n) {
        stralloc_copyb(&tok, s, n);
        stralloc_nul(&tok);

        if(!tools.compiler && is_compiler(tok.s)) {
          tools.compiler = (char*)s;
          break;
        }
      }

      stralloc_free(&tok);
    }

    set_free(&toks);
  }

  if(tools.compiler)
    if(!set_make_type() || !set_compiler_type(tools.compiler)) {
      usage(argv0);
      return 2;
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

  return 0;
}

