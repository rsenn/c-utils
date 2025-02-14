#ifndef GENMAKEFILE_VAR_H
#define GENMAKEFILE_VAR_H

#include "types.h"

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
void var_subst(const stralloc*,
               stralloc* out,
               const char* pfx,
               const char* sfx,
               int tolower);
void push_lib(const char*, const char* lib);
void push_linkdir(const char*, const char* dir);
void with_lib(const char*);
void push_define(const char*);

extern linklib_fmt *format_linklib_fn, *format_linkdir_fn;

extern MAP_T vars;

#endif /* defined(GENMAKEFILE_VAR_H) */
