#ifndef GENMAKEFILE_INCLUDES_H
#define GENMAKEFILE_INCLUDES_H

#include "types.h"
#include "../../lib/strlist.h"
#include "../../lib/set.h"

void includes_add_b(const char*, size_t, stralloc*, stralloc*);
void includes_add(const char*, stralloc*, stralloc*);
void includes_cppflags(void);
void includes_extract(const char*, size_t, strlist*, int);
int includes_get(const char*, strlist*, int, stralloc*, char);
void includes_to_libs(const set_t*, strlist*, const char*, stralloc*, const char*, const char*);

extern strlist include_dirs;

#endif /* defined(GENMAKEFILE_INCLUDES_H) */
