#ifndef GENMAKEFILE_VAR_H
#define GENMAKEFILE_VAR_H

#include "types.h"

typedef void(linklib_fmt)(const char*, stralloc*);

void format_linklib_lib(const char*, stralloc*);
void format_linklib_switch(const char*, stralloc*);
void format_linklib_dummy(const char*, stralloc*);
int var_isset(const char*);
var_t* var_list(const char*, char);
const char* var_get(const char*);
var_t* var_setb(const char*, const char*, size_t);
var_t* var_set(const char*, const char*);
void var_unset(const char*);
void var_push(const char*, const char*);
void var_push_sa(const char*, stralloc*);
void var_subst(const stralloc*, stralloc*, const char*, const char*, int);
void push_lib(const char*, const char*);
void with_lib(const char*);

extern linklib_fmt* format_linklib_fn;

extern MAP_T vars;

#endif /* defined(GENMAKEFILE_VAR_H) */
