#ifndef _PATH_H__
#define _PATH_H__

#include "windoze.h"
#include "stralloc.h"
#include "str.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WINDOWS_NATIVE
#define PATHSEP_C '\\'
#define PATHSEP_S "\\"
#define PATHLISTSEP_C ';'
#define PATHLISTSEP_S ";"
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#define PATHLISTSEP_C ':'
#define PATHLISTSEP_S ":"
#endif

#define PATH_FNM_NOMATCH 1
#define PATH_FNM_PATHNAME (1 << 0) /* No wildcard can ever match /'.  */
#define PATH_FNM_NOESCAPE (1 << 1) /* Backslashes don't quote special chars.  */
#define PATH_FNM_PERIOD (1 << 2)   /* Leading .' is matched only explicitly.  */

int    path_absolute(const char*, stralloc* sa);
int    path_absolute_sa(stralloc*);
char*  path_basename(const char*);
int    path_canonical(const char*, stralloc* out);
int    path_canonicalize(const char*, stralloc* sa, int symbolic);
int    path_canonical_sa(stralloc*, stralloc* out);
int    path_collapse(const char*, stralloc* out);
char*  path_dirname(const char*, stralloc* dir);
int    path_exists(const char*);
int    path_find(const char*, const char* name, stralloc* out);
int    path_fnmatch(const char*, unsigned int plen, const char* string, unsigned int slen, int flags);
void   path_getcwd(stralloc*);
char*  path_gethome(int);
int    path_getsep(const char*);
int    path_is_absolute(const char*);
int    path_is_directory(const char*);
int    path_is_separator(char);
size_t path_len(const char*, size_t n);
size_t path_len_s(const char*);
size_t path_num(const char*, size_t len, int n);
size_t path_num_sa(const char*, size_t len, stralloc* sa, int n);
int    path_readlink(const char*, stralloc* sa);
int    path_realpath(const char*, stralloc* sa, int symbolic, stralloc* cwd);
int    path_relative(const char*, const char* relative_to, stralloc* out);
size_t path_right(const char*, size_t n);
size_t path_skip(const char*, size_t n);
size_t path_skips(const char*);
char*  path_skip_separator(const char*);

#ifdef STRLIST_H
int path_split(const char* p, strlist* sl);
#endif

#ifndef PATH_MAX
#if WINDOWS
#include <windows.h>
#endif
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#ifdef __cplusplus
}
#endif

#endif /* _PATH_H__ */
