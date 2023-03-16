#ifndef GENMAKEFILE_PATH_H
#define GENMAKEFILE_PATH_H

#include "../../lib/stralloc.h"
#include "../../lib/strlist.h"

char* path_clean_b(const char*, size_t*);
char* path_clean_s(const char*);
char* path_dirname_alloc(const char*);
char* path_extension(const char*, stralloc*, const char* ext);
const char* path_mmap_read(const char*, size_t*, char pathsep_make);
void path_normalize_b(const char*, size_t, stralloc* out, stralloc* builddir, stralloc* outdir);
void path_normalize(const char*, stralloc*, stralloc* builddir, stralloc* outdir);
char* path_output(const char*, stralloc*, const char* ext, stralloc* builddir, char pathsep_args);
void path_prefix_b(const stralloc*, const char*, size_t n, stralloc* out, char pathsep_make);
void path_prefix_sa(const stralloc*, stralloc*, char pathsep_make);
void path_prefix_s(const stralloc*, const char*, stralloc* out, char pathsep_make);
char* path_wildcard(stralloc*, const char*);

#endif /* defined(GENMAKEFILE_PATH_H) */
