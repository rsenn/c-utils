/**
 * @defgroup   path
 * @brief      PATH module: lexical path functions over (pointer, length) views, shared with shish
 *             (doc/path-lib.md there). Writers are snprintf-shaped, see path_cat().
 * @{
 */
#ifndef _PATH_H__
#define _PATH_H__

#include "windoze.h"
#include "stralloc.h"
#include "str.h"

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum path_format {
  MIX = 0,
  UNX,
  WIN,
} path_format;

/* every separator character as a string: programs that split on it, not the library */
#if WINDOWS
#define PATHSEP_S_MIXED "\\/"
#else
#define PATHSEP_S_MIXED "/"
#endif

#if WINDOWS_NATIVE
#define PATHSEP_C '\\'
#define PATHSEP_S "\\"
#define PATHLISTSEP_C ';'
#define PATHLISTSEP_S ";"
#define PATHLISTSEP_CHARS ";"
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#define PATHLISTSEP_C ':'
#define PATHLISTSEP_S ":"
#define PATHLISTSEP_CHARS ":;"
#endif

#define PATH_FNM_NOMATCH 1
#define PATH_FNM_PATHNAME (1 << 0) /* No wildcard can ever match /'.  */
#define PATH_FNM_NOESCAPE \
  (1 << 1)                       /* Backslashes don't quote special chars. \
                                  */
#define PATH_FNM_PERIOD (1 << 2) /* Leading .' is matched only explicitly.  */

const char* path_basename(const char*, size_t, size_t* baselen);
size_t path_cat(char* out, size_t cap, const char* a, size_t an, const char* b, size_t bn);
int path_cat_sa(stralloc* out, const char* a, size_t an, const char* b, size_t bn);
size_t path_canon(char* out, size_t cap, const char* p, size_t n);
size_t path_abs(char* out, size_t cap, const char* p, size_t n, const char* cwd, size_t cwdn);
int path_abs_sa(stralloc* out, const char* p, size_t n);
size_t path_rel(char* out, size_t cap, const char* p, size_t pn, const char* base, size_t bn);
int path_rel_sa(stralloc* out, const char* p, size_t pn, const char* base, size_t bn);
size_t path_common(const char* a, size_t an, const char* b, size_t bn, size_t* blen);
size_t path_extoff(const char* p, size_t n);
int path_isname(const char* p, size_t n);
int path_isabs(const char* p, size_t n);
int path_isrel(const char* p, size_t n);
size_t path_next(const char* p, size_t n, size_t pos);
size_t path_parentlen(const char* p, size_t n);
size_t path_dirlen(const char*, size_t);
size_t path_listlen(const char* l, size_t n);
size_t path_listnext(const char* l, size_t n, size_t pos);
size_t path_search(char* out, size_t cap, const char* l, size_t ln, const char* name, size_t nl, int (*ok)(const char*, void*), void* arg);
int path_fnmatch(const char* pattern, size_t plen, const char* string, size_t slen, int flags);
int path_getcwd(stralloc*);
char* path_gethome(int uid);
int path_readlink(const char*, stralloc*);
int path_resolve(const char* p, size_t n, stralloc* sa, int symbolic, const char* cwd, size_t cwdn);
size_t path_rootlen(const char*, size_t);

/* PATH_LIST_FOREACH(l, n, elem, elen) visits every element of a PATH-style list, empty ones too
 * (an empty element is the current directory):  "a::b:" -> "a", "", "b", ""
 *
 *   const char*  l     the list           size_t  n     its length
 *   const char*  elem  receives the start  size_t  elen  receives the length
 * ----------------------------------------------------------------------- */
#define PATH_LIST_FOREACH(l, n, elem, elen) \
  for(size_t pl_pos_ = 0; pl_pos_ != (size_t)-1 && ((elem) = (l) + pl_pos_, (elen) = path_listlen((elem), (n)-pl_pos_), 1); pl_pos_ = path_listnext((l), (n), pl_pos_))

#define path_basename_s(s) path_basename((s), str_len((s)), 0)


/* c-utils conveniences on top of the core; not part of the shared library */
int path_canon_sa(stralloc* sa);
char* path_dirname(const char*, size_t, stralloc* dir);
char* path_dirname_s(const char*, stralloc*);
char* path_dirname_sa(stralloc* dir);
char* path_join(const char*, const char*);
int path_lookup(stralloc* out, const char* list, const char* cmd);
int path_find(const char* path, const char* name, stralloc* out);
int path_exists(const char* p);
int path_access(const char*, int rights);
int path_is_directory(const char* p);

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

#ifndef R_OK
#define R_OK 4
#endif
#ifndef W_OK
#define W_OK 2
#endif
#ifndef X_OK
#define X_OK 1
#endif
#ifndef F_OK
#define F_OK 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* _PATH_H__ */
/** @} */
