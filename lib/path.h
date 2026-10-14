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

#define path_is_absolute(p, len) (path_rootlen((p), (len)) & 1)
#define path_is_absolute_s(p) (path_rootlen_s((p)) & 1)
#define path_is_absolute_sa(sa) path_is_absolute((sa)->s, (sa)->len)

int path_access(const char*, int rights);
const char* path_basename(const char* p, size_t n, size_t* len);
#define path_basename_s(s) path_basename((s), str_len(s), 0)
int path_canonicalize(const char* path, stralloc* sa, int symbolic);
size_t path_dirlen(const char*, size_t);
size_t path_dirlen_s(const char*);
char* path_dirname(const char*, size_t, stralloc* dir);
char* path_dirname_s(const char*, stralloc*);
char* path_dirname_sa(stralloc* dir);
int path_exists(const char* p);
size_t path_listlen(const char* l, size_t n);
size_t path_listnext(const char* l, size_t n, size_t pos);
size_t path_search(char* out, size_t cap, const char* l, size_t ln, const char* name, size_t nl, int (*ok)(const char*, void*), void* arg);
int path_lookup(stralloc* out, const char* list, const char* cmd);
#define PATH_LIST_FOREACH(l, n, elem, elen) \
  for(size_t pl_pos_ = 0; pl_pos_ != (size_t)-1 && ((elem) = (l) + pl_pos_, (elen) = path_listlen((elem), (n)-pl_pos_), 1); pl_pos_ = path_listnext((l), (n), pl_pos_))
int path_find(const char* path, const char* name, stralloc* out);
int path_fnmatch(const char* pattern, unsigned int plen, const char* string, unsigned int slen, int flags);
void path_getcwd(stralloc* sa);
char* path_getcwd_s(void);
char* path_gethome(int uid);
int path_getsep(const char* path);
size_t path_rootlen(const char*, size_t);
int path_rootlen_s(const char* p);
int path_is_directory(const char* p);
int path_is_separator(const char*);
size_t path_len(const char* s, size_t n);
size_t path_len_s(const char* s);
size_t path_num(const char* p, size_t len, int n);
size_t path_num_sa(const char* p, size_t len, stralloc* sa, int n);
int path_readlink(const char* path, stralloc* sa);
int path_realpath(const char* path, stralloc* sa, int symbolic, stralloc* cwd);
int path_relative(const char*, size_t, stralloc*);
int path_relative_s(const char*, stralloc*);


size_t path_skip(const char* s, size_t n);
size_t path_skips(const char* s);
size_t path_skip_separator(const char* p, size_t n);
size_t path_skip_component(const char* p, size_t n);



char* path_join(const char*, const char*);

size_t path_cat(char* out, size_t cap, const char* a, size_t an, const char* b, size_t bn);
int path_cat_sa(stralloc* out, const char* a, size_t an, const char* b, size_t bn);
size_t path_canon(char* out, size_t cap, const char* p, size_t n);
int path_canon_sa(stralloc* sa);
size_t path_abs(char* out, size_t cap, const char* p, size_t n, const char* cwd, size_t cwdn);
int path_abs_sa(stralloc* out, const char* p, size_t n);
size_t path_rel(char* out, size_t cap, const char* p, size_t pn, const char* base, size_t bn);
int path_rel_sa(stralloc* out, const char* p, size_t pn, const char* base, size_t bn);
size_t path_common(const char* a, size_t an, const char* b, size_t bn, size_t* blen);
size_t path_extoff(const char* p, size_t n);
#define path_isname(p, n) (path_component2((p), (n)) == (n))

#ifdef STRLIST_H
int path_split(const char*, strlist*, int);
#endif

size_t path_trim(const char*, size_t);

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

  while(*s && !path_is_separator(s))
    ++s;

  return s - p;
}

static inline size_t
path_component2(const char* p, size_t len) {
  const char *start = p, *end = p + len;

  while(p < end && !path_is_separator(p))
    ++p;

  return p - start;
}

static inline size_t
path_component3(const char* p, size_t len, size_t pos) {
  const char *start = p, *end = p + len;

  if(pos > len)
    pos = len;

  p += pos;

  while(p < end && !path_is_separator(p))
    ++p;

  return p - start;
}

static inline size_t
path_separator1(const char* p) {
  const char* s = p;

  while(*s && path_is_separator(s))
    ++s;

  return s - p;
}

static inline size_t
path_separator2(const char* p, size_t len) {
  const char *start = p, *end = p + len;

  while(p < end && path_is_separator(p))
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
  while(*p && !path_is_separator(p))
    ++p;
  while(*p && path_is_separator(p))
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
