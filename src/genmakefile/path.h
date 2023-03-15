#ifndef GENMAKEFILE_PATH_H
#define GENMAKEFILE_PATH_H

#include "../../lib/stralloc.h"
#include "../../lib/path.h"

char* path_clean_b(const char*, size_t*, stralloc*);
char* path_clean_s(const char*, stralloc*);
char* path_extension(const char*, stralloc*, const char*);
const char* path_mmap_read(const char*, size_t*, stralloc*, char);
void path_normalize_b(const char*, size_t, stralloc*, stralloc*, stralloc*);
void path_normalize(const char*, stralloc*, stralloc*, stralloc*);
char* path_output(const char*, stralloc*, const char*, stralloc*, char);
void path_prefix_b(const stralloc*, const char*, size_t, stralloc*, char);
void path_prefix_sa(const stralloc*, stralloc*, char);
void path_prefix_s(const stralloc*, const char*, stralloc*, char);
char* path_wildcard(stralloc*, const char*);

#endif /* defined(GENMAKEFILE_PATH_H) */
