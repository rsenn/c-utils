#ifndef _GENMAKEFILE_H
#define _GENMAKEFILE_H

#define MAP_USE_HMAP 1

#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/rdir.h"
#include "lib/scan.h"
#include "lib/slist.h"
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
#include "lib/set.h"
#include "lib/map.h"
#include "lib/bool.h"

typedef enum { OS_WIN, OS_MAC, OS_LINUX } os_type;
typedef enum { LANG_C, LANG_CXX } lang_type;

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
  enum { X86, ARM, PIC } arch;
  enum { _14, _16, _32, _64 } bits;
} machine_type;

typedef struct {
  os_type os;
  enum { NTOS, UNIX } type;
} system_type;

typedef struct {
  union {
    struct slink link;
    struct sourcefile* next;
  };
  const char* name;
  int has_main;
  strlist includes;
  set_t deps;
  set_t pptoks;
} sourcefile;

typedef struct {
  int n_sources;
  slink* sources;
  set_t includes;
  array rules;
  set_t pptoks;
  set_t deps;
  uint32 serial;
} sourcedir;

typedef struct target_s {
  union {
    const char* name;
    stralloc namesa;
  };
  set_t output;
  set_t prereq;
  union {
    stralloc recipe;
    strlist cmds;
  };
  array deps;
  array objs;
  uint32 serial;
  strlist vars;
  bool disabled : 1;
  bool outputs : 1;
} target;

typedef struct {
  strlist work;
  strlist build;
  strlist out;
  strlist this;
} dirs_t;

typedef struct {
  char *src, *inc;
  char* obj;
  char* lib;
  char* slib;
  char* bin;
  char* pps;
} exts_t;

typedef struct {
  char* toolchain;
  char* compiler;
  char* make;
  char* preproc;
} tools_t;

typedef enum { MAKE_IMPLICIT_RULES = 0x01, MAKE_PATTERN_RULES = 0x02 } tool_config_t;

typedef enum {
  BUILD_TYPE_RELEASE = 0,
  BUILD_TYPE_RELWITHDEBINFO,
  BUILD_TYPE_MINSIZEREL,
  BUILD_TYPE_DEBUG,
} build_type_t;

typedef struct {
  machine_type mach;
  system_type sys;
  stralloc chip;
  int build_type;
  int lang;
} config_t;

typedef struct {
  strlist value;
  uint32 serial;
} var_t;

int mkdir_sa(const stralloc*, int);
int mkdir_components(strlist*, int);

void stralloc_weak(stralloc*, const stralloc*);

void put_newline(buffer*, int);

void set_command(stralloc*, const char*, const char*);

void strarray_dump(buffer*, const strarray*);

void path_prefix_b(const stralloc*, const char*, size_t, stralloc*);
void path_prefix_s(const stralloc*, const char*, stralloc*);
void path_prefix_sa(const stralloc*, stralloc*);
char* path_extension(const char*, stralloc*, const char*);
char* path_output(const char*, stralloc*, const char*);
char* path_wildcard(stralloc*, const char*);
const char* path_mmap_read(const char*, size_t*);

int extract_build_type(const stralloc*);
void extract_tokens(const char*, size_t, set_t*);
void extract_pptok(const char*, size_t, set_t*);
void extract_vars(const char*, size_t, set_t*);

void format_linklib_lib(const char*, stralloc*);
void format_linklib_switch(const char*, stralloc*);
void format_linklib_dummy(const char*, stralloc*);

size_t skip_comment(const char*, size_t);

int main_scan(const char*, size_t);
int main_present(const char*);

void includes_cppflags(void);
void includes_extract(const char*, size_t, strlist*, int);
int includes_get(const char*, strlist*, int);
void includes_add(const char*);
void includes_to_libs(const set_t*, strlist*);

void var_subst(const stralloc*, stralloc*, const char*, const char*, int);

target* rule_get(const char*);
target* rule_get_sa(stralloc*);
target* rule_find(const char*);
void rule_rename(target*, const char*);
target* rule_find_sa(stralloc*);
target* rule_find_b(const char*, size_t);
int rule_match(target*, const char*);
void rule_command_subst(target*, stralloc*, const char*, size_t);
void rule_command(target*, stralloc*);
int rule_add_dep(target*, target*);
void rule_add_deps(target*, const strlist*);
void rule_dep_list_recursive(target*, set_t*, int, strlist*);
void rule_dep_list(target*, set_t*);
void rule_deps_indirect(target*, set_t*);
void rule_dump(target*);
bool rule_is_compile(target* rule);
bool rule_is_lib(target* rule);
bool rule_is_link(target* rule);
void rule_prereq_recursive(target*, set_t* s);
target* rule_find_lib(const char* name, size_t namelen);

void add_path_b(set_t*, const char*, size_t);
void add_path(set_t*, const char*);
void add_srcpath(set_t*, const char*);
void add_source(set_t*, const char*);
void add_path_sa(set_t*, stralloc*);
void add_path_relativeb(set_t*, stralloc*, const char*, size_t);

int is_source(const char*);
int is_source_sa(stralloc*);
int is_source_b(const char*, size_t);
int is_filename(const char*);
int is_filename_sa(stralloc*);
int is_filename_b(const char*, size_t);
int is_include(const char*);
int is_include_sa(stralloc*);
int is_include_b(const char*, size_t);
int is_object(const char*);
int is_object_sa(stralloc*);
int is_object_b(const char*, size_t);
int is_lib_b(const char* filename, size_t len);
int is_lib_sa(stralloc* sa);
int is_lib(const char* s);

sourcefile* sources_new(const char*);
int sources_add(const char*);
int sources_add_b(const char*, size_t);
int sources_sort(const char**, const char**);
void sources_get(const char*);
const char* sources_find(const char*, size_t, size_t*);
void sources_deps(sourcefile*, strlist*);
void sources_readdir(stralloc*, strarray*);
void sources_addincludes(sourcefile*, sourcedir*, const strlist*, strarray*);
bool sources_iscplusplus(void);

int var_isset(const char*);
var_t* var_list(const char*);
const char* var_get(const char*);
var_t* var_set(const char*, const char*);
void var_unset(const char*);
void var_push(const char*, const char*);
void var_push_sa(const char*, stralloc*);

void push_lib(const char*, const char*);

void with_lib(const char*);

void push_define(const char*);

void get_rules_by_cmd(stralloc*, strlist*);

char* dirname_alloc(const char*);

sourcedir* sourcedir_find(const char*);
sourcedir* sourcedir_findsa(stralloc*);
sourcedir* sourcedir_findb(const char*, size_t);
sourcedir* sourcedir_getb(const char*, size_t);
sourcedir* sourcedir_getsa(stralloc*);
void sourcedir_addsource(const char*, strarray*);
void sourcedir_populate(strarray*);
void sourcedir_dump_all(buffer*);
void sourcedir_dep_recursive(sourcedir*, strlist*, uint32, sourcedir*);
void sourcedir_deps(sourcedir*, strlist*);
void sourcedir_deps_s(const char*, strlist*);
void sourcedir_deps_b(const char*, size_t, strlist*);
void sourcedir_printdeps(sourcedir*, buffer*, int);

void deps_indirect(set_t*, const strlist*);
void deps_direct(set_t*, const target*);

void print_rule_deps_r(buffer*, target*, set_t*, strlist*, int);
void print_rule_deps(buffer*, target*);

void remove_indirect_deps_recursive(array*, array*, int);
ssize_t remove_indirect_deps(array*);

void deps_for_libs(void);

void target_ptrs(const strlist*, array*);

target* gen_single_rule(stralloc* output, stralloc* cmd);
void gen_clean_rule(void);

int filter_pptoks(const void*, size_t);

target* gen_srcdir_compile_rules(sourcedir*, const char*);
target* gen_simple_compile_rules(sourcedir*, const char*, const char*, const char*, stralloc*);
target* gen_srcdir_lib_rule(sourcedir*, const char*);
void gen_srcdir_rule(sourcedir*, const char*);
void gen_lib_rules(void);
int gen_link_rules(void);
target* gen_install_rules(void);

void get_keys(MAP_T*, strlist*);
#define MAP_USE_HMAP 1

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

extern exts_t exts;
extern config_t cfg;
extern dirs_t dirs;
extern set_t srcs;
extern tools_t tools;
extern tool_config_t tool_config;
extern const char* project_name;
extern int cmd_objs, cmd_libs, cmd_bins;
extern set_t link_libraries;
#endif
