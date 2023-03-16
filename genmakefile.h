#ifndef _GENMAKEFILE_H
#define _GENMAKEFILE_H

#define MAP_USE_HMAP 1

#include "src/genmakefile/is.h"
#include "src/genmakefile/rule.h"
#include "src/genmakefile/sources.h"
#include "src/genmakefile/var.h"
#include "src/genmakefile/includes.h"

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

typedef enum { MAKE_IMPLICIT_RULES = 0x01, MAKE_PATTERN_RULES = 0x02 } tool_config_t;

typedef enum {
  BUILD_TYPE_RELEASE = 0,
  BUILD_TYPE_RELWITHDEBINFO,
  BUILD_TYPE_MINSIZEREL,
  BUILD_TYPE_DEBUG,
} build_type_t;

int mkdir_sa(const stralloc*, int);
int mkdir_components(strlist*, int);
void stralloc_weak(stralloc*, const stralloc*);
void buffer_putnl(buffer*, int);
void set_command(stralloc*, const char*, const char*);
void strarray_dump(buffer*, const strarray*);
int extract_build_type(const stralloc*);
void extract_tokens(const char*, size_t, set_t*);
void extract_pptok(const char*, size_t, set_t*);
void extract_vars(const char*, size_t, set_t*);
void format_linklib_lib(const char*, stralloc*);
void format_linklib_switch(const char*, stralloc*);
void format_linklib_dummy(const char*, stralloc*);
int main_scan(const char*, size_t);
int main_present(const char*);
void var_subst(const stralloc*, stralloc*, const char*, const char*, int);
void add_path_b(set_t*, const char*, size_t);
void add_path(set_t*, const char*);
void add_srcpath(set_t*, const char*);
void add_source(set_t*, const char*);
void add_path_sa(set_t*, stralloc*);
void add_path_relativeb(set_t*, stralloc*, const char*, size_t);
void push_lib(const char*, const char*);
void with_lib(const char*);
void push_define(const char*);
void get_rules_by_cmd(stralloc*, strlist*);
char* path_dirname_alloc(const char*);
void deps_indirect(set_t*, const strlist*);
void deps_direct(set_t*, const target*);
void print_rule_deps_r(buffer*, target*, set_t*, strlist*, int);
void print_rule_deps(buffer*, target*);
void remove_indirect_deps_recursive(array*, array*, int);
ssize_t remove_indirect_deps(array*);
void deps_for_libs(void);
void target_ptrs(const strlist*, array*);
void libdirs_add(const char* dir);
int input_command_line(const char*, size_t);

void output_var(buffer*, MAP_T*, const char*, int);
void output_all_vars(buffer*, MAP_T*, strlist*);
void output_make_rule(buffer*, target*);
void output_ninja_rule(buffer*, target*);
void output_all_rules(buffer*);
void output_build_rules(buffer*, const char*, const stralloc*);
void output_script(buffer*, target*);

int set_machine(const char*);
int set_chip(const char*);
int set_system(const char*);
int set_make_type(void);
int set_compiler_type(const char*);

void usage(char*);

// extern exts_t exts;
extern config_t cfg;
extern dirs_t dirs;
extern tools_t tools;
extern tool_config_t tool_config;
extern const char* project_name;
extern int cmd_objs, cmd_libs, cmd_bins;
extern set_t link_libraries;
extern MAP_T sourcedirs, targetdirs, rules;
extern strlist link_dirs;
extern strlist vpath;
extern strlist build_as_lib;
extern union commands commands;
extern bool inst_bins, inst_libs;
extern const char* newline;
extern strarray dirstack;
extern set_t build_directories;

#endif
