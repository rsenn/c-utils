#ifndef _GENMAKEFILE_H
#define _GENMAKEFILE_H

#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/path.h"
#include "lib/rdir.h"
#include "lib/scan.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/uint32.h"
#include "lib/errmsg.h"
#include "lib/array.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/dir.h"
#include "lib/range.h"
#include "lib/case.h"
#include "map.h"

typedef enum { WIN, MAC, LINUX } os_type;

#if WINDOWS
#define MAX_CMD_LEN 1023

#define DEFAULT_OBJEXT ".obj"
#define DEFAULT_LIBEXT ".lib"
#define DEFAULT_LIBPFX ""
#define DEFAULT_EXEEXT ".exe"
#define DEFAULT_PPSEXT ".pp.c"
#else
#define MAX_CMD_LEN 8191

#define DEFAULT_OBJEXT ".o"
#define DEFAULT_LIBEXT ".a"
#define DEFAULT_LIBPFX "lib"
#define DEFAULT_EXEEXT ""
#define DEFAULT_PPSEXT ".pp.c"
#endif

#if WINDOWS_NATIVE
#define DEFAULT_PATHSEP '\\'
#else
#define DEFAULT_PATHSEP '/'
#endif

typedef struct {
  enum { X86, ARM, PIC } arch;
  enum { _14, _16, _32, _64 } bits;
} machine_type;

typedef struct {
  os_type os;
  enum { NTOS, UNIX } type;
} system_type;

typedef struct {
  struct slink link;
  const char* name;
  int has_main;
} sourcefile;

typedef struct {
  int n_sources;
  slink* sources;
  strlist includes;
  array rules;
} sourcedir;

typedef struct target_s {
  const char* name;
  strlist output;
  strlist prereq;
  stralloc recipe;
  array deps;
  array objs;
  uint32 serial;
} target;

typedef struct {
  strlist work;
  strlist build;
  strlist out;
  strlist this;
} dirs_t;

typedef struct {
  char* obj;
  char* lib;
  char* bin;
  char* pps;
} exts_t;

typedef struct {
  char* toolchain;
  char* compiler;
  char* make;
  char* preproc;
} tools_t;

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
} config_t;

void add_include_dir(const char* dir);
void add_path_sa(strlist* list, stralloc* path);
void add_path(strlist* list, const char* path);
void add_source(const char* filename);
void add_srcpath(strlist* list, const char* path);
void debug_int(const char* name, int i);
void debug_sa(const char* name, stralloc* sa);
void debug_s(const char* name, const char* s);
void debug_sl(const char* name, const strlist* l);
void debug_target(const target* t);
void deps_direct(strlist* l, const target* t);
void deps_for_libs(HMAP_DB* rules);
void deps_indirect(strlist* l, const strlist* names);
char* dirname_alloc(const char* p);
void rule_dump(target* rule);
void dump_sourcedirs(buffer* b, HMAP_DB* sourcedirs);
int extract_build_type(const stralloc* s);
void extract_includes(const char* x, size_t n, strlist* includes, int sys);
void extract_pptok(const char* x, size_t n, strlist* tokens);
void extract_tokens(const char* x, size_t n, strlist* tokens);
target* rule_find_b(const char* x, size_t n);
target* rule_find(const char* needle);
target* rule_find_sa(stralloc* name);
void format_linklib_dummy(const char* libname, stralloc* out);
void format_linklib_lib(const char* libname, stralloc* out);
void format_linklib_switch(const char* libname, stralloc* out);
void gen_clean_rule(HMAP_DB* rules);
target* gen_install_rules(HMAP_DB* rules);
void gen_lib_rules(HMAP_DB* rules, HMAP_DB* srcdirs);
int gen_link_rules(HMAP_DB* rules);
target* gen_simple_compile_rules(
    HMAP_DB* rules, sourcedir* srcdir, const char* dir, const char* fromext, const char* toext, stralloc* cmd);
target* gen_srcdir_compile_rules(HMAP_DB* rules, sourcedir* sdir, const char* dir);
void gen_srcdir_rule(HMAP_DB* rules, sourcedir* sdir, const char* name);
int get_includes(const char* srcfile, strlist* includes, int sys);
void get_keys(MAP_T map, strlist* list);
void get_ref_vars(const stralloc* in, strlist* out);
target* rule_get(const char* name);
target* rule_get_sa(stralloc* name);
void get_rules_by_cmd(stralloc* cmd, strlist* deps);
sourcedir* get_sourcedir_b(const char* x, size_t n);
sourcedir* get_sourcedir(const char* path);
sourcedir* get_sourcedir_sa(stralloc* path);
void get_sources(const char* basedir);
strlist* get_var(const char* name);
int has_main(const char* filename);
void include_dirs_to_cppflags(void);
void includes_to_libs(const strlist* includes, strlist* libs);
int is_include(const char* filename);
int is_include_sa(stralloc* sa);
int is_object(const char* filename);
int is_object_sa(stralloc* sa);
int isset(const char* name);
int is_source(const char* filename);
int is_source_sa(stralloc* sa);
target* lib_rule_for_sourcedir(HMAP_DB* rules, sourcedir* srcdir, const char* name);
int main(int argc, char* argv[]);
int rule_match(target* rule, const char* pattern);
int mkdir_components(strlist* dir, int mode);
int mkdir_sa(const stralloc* dir, int mode);
sourcefile* new_source(const char* name);
void output_all_rules(buffer* b, HMAP_DB* hmap);
void output_all_vars(buffer* b, MAP_T vars, strlist* varnames);
void output_build_rules(buffer* b, const char* name, const stralloc* cmd);
void output_make_rule(buffer* b, target* rule);
void output_ninja_rule(buffer* b, target* rule);
void output_script(buffer* b, target* rule);
void output_var(buffer* b, HMAP_DB* vars, const char* name);
char* path_extension(const char* in, stralloc* out, const char* ext);
char* path_output(const char* in, stralloc* out, const char* ext);
void path_prefix_b(const stralloc* prefix, const char* x, size_t n, stralloc* out);
void path_prefix_s(const stralloc* prefix, const char* path, stralloc* out);
char* path_wildcard(stralloc* sa, const char* wildchar);
void populate_sourcedirs(HMAP_DB* sourcedirs);
void print_target_deps(buffer* b, target* t);
void print_target_deps_r(buffer* b, target* t, strlist* deplist, strlist* hierlist, int depth);
void push_define(const char* def);
void push_lib(const char* name, const char* lib);
void push_var(const char* name, const char* value);
void push_var_sa(const char* name, stralloc* value);
void put_newline(buffer* b, int flush);
ssize_t remove_indirect_deps(array* deps);
void remove_indirect_deps_recursive(array* top, array* a, int depth);
void rule_command_subst(target* rule, stralloc* out, const char* prereq, size_t plen);
void rule_command(target* rule, stralloc* out);
int scan_main(const char* x, size_t n);
int set_chip(const char* s);
void set_command(stralloc* sa, const char* cmd, const char* args);
int set_compiler_type(const char*);
int set_machine(const char* s);
int set_make_type(void);
int set_system(const char* s);
strlist* set_var(const char* name, const char* value);
int sort_sources(const char** a, const char** b);
void stralloc_weak(stralloc* out, const stralloc* from);
void strarray_dump(buffer* b, const strarray* arr);
void subst_var(const stralloc* in, stralloc* out, const char* pfx, const char* sfx, int tolower);
void target_add_deps(target* t, const strlist* deps);
int target_add_dep(target* t, target* other);
void target_dep_list_recursive(strlist* l, target* t, int depth, strlist* hier);
void target_dep_list(strlist* l, target* t);
void target_deps_indirect(strlist* l, target* t);
void target_ptrs(const strlist* targets, array* out);
void unset_var(const char* name);
void usage(char* argv0);
const char* var(const char* name);
void with_lib(const char* lib);

void output_mplab_project(buffer* b, MAP_T rules, MAP_T vars, const strlist* include_dirs);

extern exts_t exts;
extern config_t cfg;
extern dirs_t dirs;
extern MAP_T rules, vars, sourcedirs;
extern strarray srcs;
extern tools_t tools;

#endif
