#ifndef GENMAKEFILE_PATH_H
#define GENMAKEFILE_PATH_H

#include "../../lib/stralloc.h"
#include "../../lib/strlist.h"

void path_prefix_b(const stralloc*, const char* x, size_t n, stralloc* out, char psm);
void path_prefix_s(const stralloc*, const char* path, stralloc* out, char psm);
void path_prefix_sa(const stralloc*, stralloc* sa, char psm);
char* path_extension(const char*, stralloc* out, const char* ext);
char* path_output(const char*, stralloc* out, const char* ext, char psa);
char* path_wildcard(stralloc*, const char* wildchar);
char* path_clean_s(const char*);
char* path_clean_b(const char*, size_t* len);
const char* path_mmap_read(const char*, size_t* n, char psm);
void path_normalize(const char*, stralloc* out);
void path_normalize_b(const char*, size_t len, stralloc* out);
char* path_dirname_alloc(const char*);

#endif /* defined(GENMAKEFILE_PATH_H) */
