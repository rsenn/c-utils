#ifndef GENMAKEFILE_VAR_H
#define GENMAKEFILE_VAR_H

#include "types.h"

const char* var_get(const char*);
int var_isset(const char*);
var_t* var_list(const char*, char);
void var_push(const char*, const char*);
void var_push_sa(const char*, stralloc*);
var_t* var_setb(const char*, const char*, size_t);
var_t* var_set(const char*, const char*);
void var_subst(const stralloc*, stralloc*, const char*, const char*, int);
void var_unset(const char*);

extern MAP_T vars;

#endif /* defined(GENMAKEFILE_VAR_H) */
