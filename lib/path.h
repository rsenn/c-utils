#ifndef _PATH_H__
#define _PATH_H__

#include "stralloc.h"
#include "str.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PATH_FNM_NOMATCH     1
#define PATH_FNM_PATHNAME    (1 << 0) /* No wildcard can ever match /'.  */
#define PATH_FNM_NOESCAPE    (1 << 1) /* Backslashes don't quote special chars.  */
#define PATH_FNM_PERIOD      (1 << 2) /* Leading .' is matched only explicitly.  */

char*  path_basename(char* path);
int    path_canonicalize(const char* path, stralloc* sa, int symbolic);
char*  path_dirname(const char* path, stralloc* dir);
int    path_fnmatch(const char* pattern, unsigned int plen, const char* string, unsigned int slen, int flags);
void   path_getcwd(stralloc* sa);
char*  path_gethome(int uid);
size_t path_len_s(const char* s);
size_t path_len(const char* s, size_t n);
int    path_readlink(const char* path, stralloc* sa);
int    path_realpath(const char* path, stralloc* sa, int symbolic, stralloc* cwd);
size_t path_right(const char* s, size_t n);
size_t path_skips(const char* s);
size_t path_skip(const char* s, size_t n);
size_t path_num(const char* p, size_t len, int n);
size_t path_num_sa(const char* p, size_t len, stralloc* sa, int n);
int    path_exists(const char* p);
int    path_is_absolute(const char* p);
int    path_relative(const char*, const char* to, stralloc* rel);

#ifdef STRLIST_H
int    path_split(const char* p, strlist* sl);
#endif

int path_canonical_sa(stralloc*, stralloc* out);
int path_absolute_sa(stralloc*);
int path_collapse(const char*, stralloc* out);

#ifdef __cplusplus
}
#endif

#endif /* _PATH_H__ */
int path_getsep(const char*);
int path_canonical(const char*, stralloc* out);
