#ifndef GENMAKEFILE_RULE_H
#define GENMAKEFILE_RULE_H

#define MAP_USE_HMAP 1

#include "types.h"
#include "../../lib/windoze.h"
#include "../../lib/unix.h"
#include "../../lib/mmap.h"
#include "../../lib/open.h"
#include "../../lib/rdir.h"
#include "../../lib/scan.h"
#include "../../lib/slist.h"
#include "../../lib/str.h"
#include "../../lib/stralloc.h"
#include "../../lib/strarray.h"
#include "../../lib/strlist.h"
#include "../../lib/uint32.h"
#include "../../lib/errmsg.h"
#include "../../lib/array.h"
#include "../../lib/byte.h"
#include "../../lib/fmt.h"
#include "../../lib/dir.h"
#include "../../lib/range.h"
#include "../../lib/case.h"
#include "../../lib/set.h"
#include "../../lib/map.h"
#include "../../lib/bool.h"

// typedef enum { PREPROCESS = 0, COMPILE, LIB, LINK, MKDIR, CLEAN, NUM_COMMANDS } command_type;

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

extern MAP_T rules;

target* rule_get(const char*);
target* rule_get_sa(stralloc*);
target* rule_find(const char*);
void rule_rename(target*, const char*, char);
target* rule_find_sa(stralloc*);
target* rule_find_b(const char*, size_t);
target* rule_find_lib(const char*, size_t, const char*, const char*);
int rule_match(target*, const char*);
void rule_command_subst(target*, stralloc*, const char*, size_t, bool, bool, const char quote_args[], char, const char*);
void rule_command(target*, stralloc*, bool, bool, const char quote_args[], char, const char*, const char*);
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

#endif
