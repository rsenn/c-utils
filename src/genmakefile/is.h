#ifndef GENMAKEFILE_IS_H
#define GENMAKEFILE_IS_H

#include "../../lib/strlist.h"
#include "../../lib/path_internal.h"
#include "../../lib/str.h"
#include "../../lib/byte.h"
#include "../../lib/stralloc.h"
#include "../../lib/env.h"
#include <ctype.h>

#include "types.h"
#include "path.h"

extern exts_t exts;
extern config_t cfg;

/**
 * @defgroup source functions
 * @{
 */
static inline int
is_b(const char* x, size_t n, int (*fn_b)(const char*, size_t)) {
  return fn_b(x, n);
}

static inline int
is_s(const char* s, int (*fn_b)(const char*, size_t)) {
  return is_b(s, str_len(s), fn_b);
}

static inline int
is_sa(const stralloc* sa, int (*fn_b)(const char*, size_t)) {
  return is_b(sa->s, sa->len, fn_b);
}

static inline int
is_source_b(const char* filename, size_t len) {
  if(byte_ends(filename, len, exts.src))
    return 1;

  if(byte_ends(filename, len, exts.inc))
    return 1;

  if(byte_ends(filename, len, ".c"))
    return 1;

  if(byte_ends(filename, len, ".S"))
    return 1;

  if(byte_ends(filename, len, ".s"))
    return 1;

  if(byte_ends(filename, len, ".asm"))
    return 1;

  if(byte_ends(filename, len, ".inc"))
    return 1;

  /*if(cfg.lang != LANG_CXX)
    return 0;*/

  if(byte_ends(filename, len, ".C"))
    return 1;

  if(byte_ends(filename, len, ".cc"))
    return 1;

  if(byte_ends(filename, len, ".cpp"))
    return 1;

  if(byte_ends(filename, len, ".cxx"))
    return 1;

  if(byte_ends(filename, len, ".c++"))
    return 1;

  return 0;
}

static inline int
is_source_sa(stralloc* sa) {
  return is_sa(sa, &is_source_b);
}

static inline int
is_source(const char* s) {
  return is_s(s, &is_source_b);
}

static inline int
is_filename_b(const char* filename, size_t len) {
  size_t dirpos = byte_rchrs(filename, len, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);
  size_t extpos = byte_rchr(filename, len, '.');
 
  if(dirpos < len)
    return 1;
  if(extpos + 1 < len) {
    size_t i;
    for(i = extpos + 1; i < len; i++)
      if(!isalnum(filename[i]))
        return 0;
    return 1;
  }
  return 0;
}

static inline int
is_filename_sa(stralloc* sa) {
  return is_sa(sa, &is_filename_b);
}

static inline int
is_filename(const char* s) {
  return is_s(s, &is_filename_b);
}

static inline int
is_include_b(const char* filename, size_t len) {
  if(byte_ends(filename, len, ".h"))
    return 1;

  if(cfg.lang != LANG_CXX)
    return 0;

  if(byte_ends(filename, len, ".H"))
    return 1;

  if(byte_ends(filename, len, ".hh"))
    return 1;

  if(byte_ends(filename, len, ".hpp"))
    return 1;

  if(byte_ends(filename, len, ".hxx"))
    return 1;

  if(byte_ends(filename, len, ".h++"))
    return 1;

  return 0;
}

static inline int
is_include_sa(stralloc* sa) {
  return is_sa(sa, &is_include_b);
}

static inline int
is_include(const char* s) {
  return is_s(s, &is_include_b);
}

static inline int
is_object_b(const char* filename, size_t len) {
  return byte_ends(filename, len, exts.obj);
}

static inline int
is_object_sa(stralloc* sa) {
  return is_sa(sa, &is_object_b);
}

static inline int
is_object(const char* s) {
  return is_s(s, &is_object_b);
}

static inline int
is_lib_b(const char* filename, size_t len) {
  return byte_ends(filename, len, exts.lib);
}

static inline int
is_lib_sa(stralloc* sa) {
  return is_sa(sa, &is_lib_b);
}

static inline int
is_lib(const char* s) {
  return is_s(s, &is_lib_b);
}

static inline int
is_command_b(const char* filename, size_t len) {
  size_t n;
  const char* x;
  stralloc path;
  strlist system_path;

  if(path_is_absolute(filename))
    return path_exists(filename);

  stralloc_init(&path);
  strlist_init(&system_path, ':');

  if((x = env_get("PATH")) == 0)
    x = "/usr/local/bin:/usr/bin:/bin";

  path_split(x, &system_path, PATHSEP_C);

  strlist_foreach(&system_path, x, n) {
    stralloc_copyb(&path, x, n);
    stralloc_catc(&path, PATHSEP_C);
    stralloc_catb(&path, filename, len);
    stralloc_nul(&path);

    if(path_exists(path.s)) {
      stralloc_free(&path);
      return 1;
    }
  }

  stralloc_free(&path);
  return 0;
}

static inline int
is_version_b(const char* x, size_t n) {
  if(*x == 'v') {
    x++;
    n--;
  }

  if(!n || !isdigit(*x))
    return 0;

  x++;
  n--;

  while(n > 0) {
    if(byte_chr("0123456789.-_", 13, *x) == 13)
      return 0;

    x++;
    n--;
  }

  return 1;
}

static inline int
is_var_b(const char* x, size_t n) {
  if(n == 0 || !(isalpha(*x) || *x == '_'))
    return 0;

  x++;
  n--;

  while(n > 0) {
    if(byte_chr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_", 63, *x) == 63) {
      if(*x == '=')
        return 1;

      break;
    }

    x++;
    n--;
  }

  return 0;
}

static inline int
is_var(const char* s) {
  return is_var_b(s, str_len(s));
}

/**
 * @}
 */

#endif /* defined(GENMAKEFILE_IS_H) */
