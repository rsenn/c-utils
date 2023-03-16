#ifndef GENMAKEFILE_INCLUDES_H
#define GENMAKEFILE_INCLUDES_H

#include "types.h"
#include "../../lib/strlist.h"
#include "../../lib/set.h"

void includes_cppflags(void);
int includes_get(const char*, strlist* includes, int sys, stralloc* thisdir, char pathsep_make);
void includes_add_b(const char*, size_t len, stralloc* builddir, stralloc* outdir);
void includes_add(const char*, stralloc* builddir, stralloc* outdir);
void includes_to_libs(
    const set_t*, strlist* libs, const char* libpfx, stralloc* thisdir, const char* incext, const char* libext);

extern strlist include_dirs;

#endif /* defined(GENMAKEFILE_INCLUDES_H) */
