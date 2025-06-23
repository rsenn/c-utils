/**
 * @defgroup   path
 * @brief      PATH module.
 * @{
 */
#ifndef _PATH_H__
#define _PATH_H__

#include "windoze.h"
#include "stralloc.h"
#include "str.h"
#include "set.h"

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t sz1, sz2;
} SizePair;

typedef enum path_format {
  MIX = 0,
  UNX,
  WIN,
} path_format;

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
#define PATH_FNM_NOESCAPE \
  (1 << 1)                       /* Backslashes don't quote special chars. \
                                  */
#define PATH_FNM_PERIOD (1 << 2) /* Leading .' is matched only explicitly.  */

 
#if WINDOWS_NATIVE
#define PATHSEP_S_MIXED "\\/"
#define path_issep(c) ((c) == '/' || (c) == '\\')
#elif WINDOWS
#define PATHSEP_S_MIXED "\\/"
#define path_issep(c) ((c) == '/' || (c) == '\\')
#else
#define PATHSEP_S_MIXED "/"
#define path_issep(c) ((c) == '/')
#endif


int path_access(const char*, int rights);
int path_absolute(const char* path, stralloc* sa);
char* path_absolute_s(const char* path);
int path_absolute_sa(stralloc* sa);
char* path_basename2(const char*, size_t);
char* path_basename(const char*);
size_t path_basepos2(const char*, size_t);
int path_canonical(const char* path, stralloc* out);
int path_canonicalize(const char* path, stralloc* sa, int symbolic);
int path_canonical_sa(stralloc* sa);
size_t path_collapse(char*, size_t);
size_t path_collapse_sa(stralloc*);
size_t path_dirlen_b(const char*, size_t);
size_t path_dirlen(const char*);
char* path_dirname_b(const char*, size_t, stralloc* dir);
char* path_dirname(const char*, stralloc*);
char* path_dirname_sa(stralloc* dir);
int path_exists(const char* p);
int path_find(const char* path, const char* name, stralloc* out);
int path_fnmatch(const char* pattern, unsigned int plen, const char* string, unsigned int slen, int flags);
void path_getcwd(stralloc* sa);
char* path_getcwd_s(void);
char* path_gethome(int uid);
int path_getsep(const char* path);
int path_is_absolute(const char* p);
int path_is_absolute_b(const char*, size_t);
int path_is_absolute_sa(const stralloc*);
int path_is_directory(const char* p);
int path_is_separator(const char*);
size_t path_len(const char* s, size_t n);
size_t path_len_s(const char* s);
size_t path_num(const char* p, size_t len, int n);
size_t path_num_sa(const char* p, size_t len, stralloc* sa, int n);
int path_readlink(const char* path, stralloc* sa);
int path_realpath(const char* path, stralloc* sa, int symbolic, stralloc* cwd);
int path_relative_b(const char*, size_t, stralloc*);
int path_relative(const char*, stralloc*);
int path_relative_to_b(const char*, size_t, const char*, size_t n2, stralloc* out);
int path_relative_to(const char*, const char*, stralloc*);
int path_relative_to_sa(const stralloc* path, const stralloc* relative_to, stralloc* out);

size_t path_normalize(char* path);
size_t path_normalize2(char* path, size_t nb);

size_t path_right(const char* s, size_t n);
size_t path_skip(const char* s, size_t n);
size_t path_skips(const char* s);
size_t path_skip_separator(const char* p, size_t n);
size_t path_skip_component(const char* p, size_t n);

SizePair path_common_prefix(const char* s1, size_t n1, const char* s2, size_t n2);
int path_relative_to_b(const char* s1, size_t n1, const char* s2, size_t n2, stralloc* out);

void path_concat(const char* a, const char* b, stralloc* out);
void path_concatb(const char* a, size_t alen, const char* b, size_t blen, stralloc* out);
void path_concat_sa(const stralloc* a, const stralloc* b, stralloc* out);

void path_append(const char* x, size_t len, stralloc* out);
void path_appends(const char* s, stralloc* out);
void path_prepend(const char*, size_t, stralloc*);
void path_prepends(const char*, stralloc*);
char* path_join(const char*, const char*);

#ifdef STRLIST_H
int path_split(const char*, strlist*, int);
#endif

size_t path_trim_b(const char*, size_t);

static inline size_t
path_is_dotslash(char* s) {
  size_t l = str_len(s);
  size_t n = path_skip_component(s, l);

  if(n == 1 && *s == '.') {
    n += path_skip_separator(s + n, l - n);
    return n;
  }

  return 0;
}

static inline size_t
path_component1(const char* p) {
  const char* s = p;

  while(*s && !path_issep(*s))
    ++s;

  return s - p;
}

static inline size_t
path_component2(const char* p, size_t len) {
  const char *start = p, *end = p + len;

  while(p < end && !path_issep(*p))
    ++p;

  return p - start;
}

static inline size_t
path_component3(const char* p, size_t len, size_t pos) {
  const char *start = p, *end = p + len;

  if(pos > len)
      pos = len;

  p += pos;

  while(p < end && !path_issep(*p))
    ++p;

  return p - start;
}

static inline size_t
path_separator1(const char* p) {
  const char* s = p;

  while(*s && path_issep(*s))
    ++s;

  return s - p;
}

static inline size_t
path_separator2(const char* p, size_t len) {
  const char *start = p, *end = p + len;

  while(p < end && path_issep(*p))
    ++p;

  return p - start;
}


static inline size_t
path_skip3(const char* s, size_t* len, size_t n) {
  const char *p = s, *e = s + n;

  p += path_component3(s, e - p, 0);

  if(len)
    *len = p - s;

  p += path_separator2(p, e - p);

  return p - s;
}

static inline size_t
path_skip1(const char* s) {
  const char* p = s;
  while(*p && !path_issep(*p))
    ++p;
  while(*p && path_issep(*p))
    ++p;
  return p - s;
}

static inline size_t
path_skip2(const char* s, size_t n) {
  const char *p = s, *e = s + n;
  p += path_component3(s, n, 0);
  p += path_separator2(p, e - p);
  return p - s;
}

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
