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

typedef enum { OS_WIN, OS_MAC, OS_LINUX } os_type;

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
  strlist includes;
  strlist pptoks;
} sourcefile;

typedef struct {
  int n_sources;
  slink* sources;
  strlist includes;
  array rules;
  strlist pptoks;
} sourcedir;

typedef struct target_s {
  const char* name;
  strlist output;
  strlist prereq;
  stralloc recipe;
  array deps;
  array objs;
  uint32 serial;
  strlist vars;
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

void includes_add(const char*);
void add_path_sa(strlist*, stralloc* path);
void add_path(strlist*, const char* path);
void sourcedir_add(strlist*, const char* path);
void deps_direct(strlist*, const target* t);
void deps_indirect(strlist*, const strlist* names);
char* dirname_alloc(const char*);
int extract_build_type(const stralloc*);
void extract_includes(const char*, size_t n, strlist* includes, int sys);
void extract_pptok(const char*, size_t n, strlist* tokens);
void extract_tokens(const char*, size_t n, strlist* tokens);
void format_linklib_dummy(const char*, stralloc* out);
void format_linklib_lib(const char*, stralloc* out);
void format_linklib_switch(const char*, stralloc* out);
int get_includes(const char*, strlist* includes, int sys);
void get_ref_vars(const stralloc*, strlist* out);
void get_rules_by_cmd(stralloc*, strlist* deps);
void includes_cppflags(void);
void includes_to_libs(const strlist*, strlist* libs);
int input_command_line(const char*, size_t n);
int is_include(const char*);
int is_include_sa(stralloc*);
int is_object(const char*);
int is_object_sa(stralloc*);
int is_source(const char*);
int is_source_sa(stralloc*);
int main(int, char* argv[]);
int main_present(const char*);
int main_scan(const char*, size_t n);
int mkdir_components(strlist*, int mode);
int mkdir_sa(const stralloc*, int mode);
void output_build_rules(buffer*, const char* name, const stralloc* cmd);
void output_make_rule(buffer*, target* rule);
void output_ninja_rule(buffer*, target* rule);
void output_script(buffer*, target* rule);
char* path_extension(const char*, stralloc* out, const char* ext);
char* path_output(const char*, stralloc* out, const char* ext);
void path_prefix_b(const stralloc*, const char* x, size_t n, stralloc* out);
void path_prefix_s(const stralloc*, const char* path, stralloc* out);
char* path_wildcard(stralloc*, const char* wildchar);
void print_rule_deps(buffer*, target* t);
void print_rule_deps_r(buffer*, target* t, strlist* deplist, strlist* hierlist, int depth);
void push_define(const char*);
void push_lib(const char*, const char* lib);
void put_newline(buffer*, int flush);
ssize_t remove_indirect_deps(array*);
void remove_indirect_deps_recursive(array*, array* a, int depth);
void rule_add_deps(target*, const strlist* deps);
int rule_add_dep(target*, target* other);
void rule_command_subst(target*, stralloc* out, const char* prereq, size_t plen);
void rule_command(target*, stralloc* out);
void rule_dep_list_recursive(target*, strlist* l, int depth, strlist* hier);
void rule_dep_list(target*, strlist* l);
void rule_deps_indirect(target*, strlist* l);
void rule_dump(target*);
target* rule_find_b(const char*, size_t n);
target* rule_find(const char*);
target* rule_find_sa(stralloc*);
target* rule_get(const char*);
target* rule_get_sa(stralloc*);
int rule_match(target*, const char* pattern);
void rule_rename(target*, const char* name);
int set_chip(const char*);
void set_command(stralloc*, const char* cmd, const char* args);
int set_compiler_type(const char*);
int set_machine(const char*);
int set_make_type(void);
int set_system(const char*);
sourcedir* sourcedir_findb(const char*, size_t n);
sourcedir* sourcedir_find(const char*);
sourcedir* sourcedir_find_sa(stralloc*);
sourcedir* sourcedir_get_sa(stralloc*);
void sources_get(const char*);
sourcefile* sources_new(const char*);
int sources_sort(const char**, const char** b);
void stralloc_weak(stralloc*, const stralloc* from);
void strarray_dump(buffer*, const strarray* arr);
void target_ptrs(const strlist*, array* out);
void usage(char*);
const char* var_get(const char*);
int var_isset(const char*);
strlist* var_list(const char*);
void var_push(const char*, const char* value);
void var_push_sa(const char*, stralloc* value);
strlist* var_set(const char*, const char* value);
void var_subst(const stralloc*, stralloc* out, const char* pfx, const char* sfx, int tolower);
void var_unset(const char*);
void with_lib(const char*);

extern exts_t exts;
extern config_t cfg;
extern dirs_t dirs;
extern strarray srcs;
extern tools_t tools;

#endif
