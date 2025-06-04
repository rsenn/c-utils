#ifndef GENMAKEFILE_VAR_H
#define GENMAKEFILE_VAR_H

#define MAP_USE_HMAP 1
#include "../../lib/stralloc.h"
#include "../../lib/strlist.h"
#include "../../lib/uint32.h"
#include "../../lib/map.h"

typedef struct {
  strlist value;
  uint32 serial;
} var_t;

typedef void(linklib_fmt)(const char*, stralloc*);

void format_linklib_lib(const char*, stralloc* out);
void format_linklib_switch(const char*, stralloc* out);
void format_linklib_dummy(const char*, stralloc* out);
void format_linkdir_switch(const char*, stralloc* out);
int var_isset(const char*);
var_t* var_list(const char*, char psa);
const char* var_get(const char*);
var_t* var_setb(const char*, const char* value, size_t vlen);
var_t* var_set(const char*, const char* value);
void var_unset(const char*);
void var_push(const char*, const char* value);
void var_push_sa(const char*, stralloc* value);
void push_lib(const char*, const char* lib);
void push_linkdir(const char*, const char* dir);
void with_lib(const char*);
void push_define(const char*);
void var_subst_sa(const char*, stralloc*, const stralloc*, const char*, const char*);

extern linklib_fmt *format_linklib_fn, *format_linkdir_fn;

extern MAP_T vars;

#endif /* defined(GENMAKEFILE_VAR_H) */
