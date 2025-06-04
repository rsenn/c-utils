#ifndef _GENMAKEFILE_H
#define _GENMAKEFILE_H

#define MAP_USE_HMAP 1

#include "src/genmakefile/is.h"
#include "src/genmakefile/rule.h"
#include "src/genmakefile/sources.h"
#include "src/genmakefile/var.h"
#include "src/genmakefile/includes.h"

#include "debug.h"

#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/rdir.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/path.h"
#include "lib/uint32.h"
#include "lib/errmsg.h"
#include "lib/array.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/dir.h"
#include "lib/range.h"
#include "lib/case.h"
#include "lib/map.h"
#include "lib/bool.h"

#if WINDOWS
#define MAX_CMD_LEN 1023

#define DEFAULT_OBJEXT ".obj"
#define DEFAULT_LIBEXT ".lib"
#define DEFAULT_DSOEXT ".dll"
#define DEFAULT_LIBPFX ""
#define DEFAULT_EXEEXT ".exe"
#define DEFAULT_PPSEXT ".pp.c"
#else
#define MAX_CMD_LEN 8191

#define DEFAULT_OBJEXT ".o"
#define DEFAULT_LIBEXT ".a"
#define DEFAULT_DSOEXT ".so"
#define DEFAULT_LIBPFX "lib"
#define DEFAULT_EXEEXT ""
#define DEFAULT_PPSEXT ".pp.c"
#endif

#if WINDOWS_NATIVE
#define DEFAULT_PATHSEP '\\'
#else
#define DEFAULT_PATHSEP '/'
#endif

#define rule_foreach(it, r) MAP_FOREACH_VALUE(rules, it, r)

typedef struct {
  strlist work;
  strlist build;
  strlist out;
  strlist this;
} dirs_t;

typedef struct {
  const char *toolchain, *compiler, *make, *preproc;
} tools_t;

typedef enum {
  MAKE_IMPLICIT_RULES = 0x01,
  MAKE_PATTERN_RULES = 0x02,
} tool_config_t;

typedef enum {
  BUILD_TYPE_RELEASE = 0,
  BUILD_TYPE_RELWITHDEBINFO,
  BUILD_TYPE_MINSIZEREL,
  BUILD_TYPE_DEBUG,
} build_type_t;

typedef enum {
  TOOL_NINJA = 1,
  TOOL_BATCH,
  TOOL_SHELL,
} build_tool_t;

void stralloc_weak(stralloc*, const stralloc*);
void buffer_putnl(buffer*, int);

int main_present(const char*);
void var_subst(const stralloc*, stralloc*, const char*, const char*, int);
char* path_dirname_alloc(const char*);
void rule_list(const strlist*, array*);
int input_command_line(const char*, size_t);
void print_rule_deps(buffer* b, target* t);
void libdirs_add(const char*);

// extern config_t cfg;
extern dirs_t dirs;
extern tools_t tools;
extern tool_config_t tool_config;
extern const char* project_name;
extern int cmd_objs, cmd_libs, cmd_bins;
extern set_t link_libraries;
extern MAP_T  targetdirs, rules;

extern strlist link_dirs;
extern strlist vpath;
extern strlist build_as_lib;

extern commands_t commands;
extern bool inst_bins, inst_libs;
extern const char* newline;
extern const char* infile;
extern const char* libpfx;
extern strarray dirstack;
extern set_t build_directories;
extern char pathsep_make;
extern char pathsep_args;

#endif
