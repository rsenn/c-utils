#ifndef GENMAKEFILE_RULE_H
#define GENMAKEFILE_RULE_H

#define MAP_USE_HMAP 1
#define MAP_USE_HMAP 1

#include "types.h"
#include "../../lib/stralloc.h"
#include "../../lib/strlist.h"
#include "../../lib/array.h"
#include "../../lib/set.h"
#include "../../lib/bool.h"
#include "../../lib/uint32.h"
#include "../../lib/map.h"

typedef enum {
  TOOL_NINJA = 1,
  TOOL_BATCH,
  TOOL_SHELL,
} build_tool_t;

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
  bool phony : 1;
  command_type type;
} target;

extern MAP_T rule_map;

target* rule_get(const char*);
target* rule_get_sa(stralloc*);
target* rule_new(const char*);
target* rule_new_sa(stralloc*);
target* rule_new_b(const char*, size_t);
target* rule_find(const char*);
target* rule_find_sa(stralloc*);
target* rule_find_b(const char*, size_t);
void rule_rename(target*, const char*, char);
target* rule_find_lib(const char*, size_t, const char*, const char*);
int rule_match(target*, const char*);
void rule_command_subst(target*, stralloc*, const char*, size_t, build_tool_t, const char*, char, const char*);
void rule_command(target*, stralloc*, build_tool_t, const char*, char, const char*, const char*);
int rule_add_dep(target*, target*);
void rule_add_deps(target*, const strlist*);
void rule_dep_list_recursive(target*, set_t*, int, strlist*);
void rule_dep_list(target*, set_t*);
void rule_deps_indirect(target*, set_t*);
void rule_prereq(target*, set_t*);
void rule_prereq_recursive(target*, set_t*);
void rule_dump(target*);
bool rule_is_compile(target*);
bool rule_is_lib(target*);
bool rule_is_link(target*);
void rule_list(const strlist*, array*);
char* rule_prereq_sa(target*, stralloc*);
char* rule_output_sa(target*, stralloc*);

void transform_subst_sa(const stralloc*, stralloc*, const char*, const char*, int);
void set_subst_b(const set_t*, const char*, size_t, stralloc*, const char*);
void set_subst_sa(const set_t*, stralloc*, const char*);

#endif
