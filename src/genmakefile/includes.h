#ifndef GENMAKEFILE_INCLUDES_H
#define GENMAKEFILE_INCLUDES_H

#include "types.h"
#include "../../lib/strlist.h"
#include "../../lib/set.h"

void includes_extract(const char*, size_t n, strlist* includes, int sys);
void includes_cppflags(void);
int includes_get(const char*, strlist* includes, int sys, char pathsep_make);
void includes_add_b(const char*, size_t len);
void includes_add(const char*);
void includes_to_libs(const set_t*, strlist* libs, const char* libpfx, const char* incext, const char* libext);
int includes_find_sa(const char*, size_t n, stralloc* out);
char* includes_find_b(const char*, size_t n);
char* includes_find(const char*);

extern strlist include_dirs;

#endif /* defined(GENMAKEFILE_INCLUDES_H) */
