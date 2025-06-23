#include "is.h"
#include "../../genmakefile.h"

/**
 * @defgroup path functions
 * @{
 */

/**
 * @brief       Adds a prefix to the specified path
 *
 * @param      prefix  Prefix to add
 * @param      x       The path buffer
 * @param      n       Length of path buffer
 * @param      out     Write output here
 * @param[in]  psm     The psm
 */
void
path_prefix_b(const stralloc* prefix, const char* x, size_t n, stralloc* out, char psm) {
  if(prefix->len && !stralloc_equals(prefix, ".")) {
    stralloc_cat(out, prefix);

    if(!stralloc_endb(prefix, &psm, 1) && !stralloc_endb(prefix, ")", 1))
      stralloc_catc(out, psm);
  }

  stralloc_catb(out, x, n);
  stralloc_nul(out);
}

/**
 * @brief       Adds a prefix to the specified path
 *
 * @param      prefix  Prefix to add
 * @param      path    The path string
 * @param      out     Write output here
 * @param[in]  psm     Path separator for makefile
 */
void
path_prefix_s(const stralloc* prefix, const char* path, stralloc* out, char psm) {
  path_prefix_b(prefix, path, str_len(path), out, psm);
  stralloc_nul(out);
}

/**
 * @brief      Adds a prefix to the specified path
 *
 * @param      prefix  Prefix to add
 * @param      sa      Path string
 * @param[in]  psm     Path separator for makefile
 */
void
path_prefix_sa(const stralloc* prefix, stralloc* sa, char psm) {
  if(prefix->len && !stralloc_equals(prefix, ".")) {
    stralloc_insertb(sa, prefix->s, 0, prefix->len);

    if(!stralloc_endb(prefix, &psm, 1))
      stralloc_insertb(sa, &psm, prefix->len, 1);
  }

  stralloc_nul(sa);
}

/**
 * @brief      Changes extension of a path
 *
 * @param[in]  in    Input path
 * @param      out   Output path
 * @param[in]  ext   Extension
 *
 * @return     Path string
 */
char*
path_extension(const char* in, stralloc* out, const char* ext) {
  size_t extpos = str_rchr(in, '.');

  stralloc_catb(out, in, extpos);
  stralloc_cats(out, ext);
  stralloc_nul(out);

  return out->s;
}

/**
 * @brief      Returns an output path
 *
 * @param[in]  in    Input path
 * @param      out   Output path
 * @param[in]  ext   Extension
 * @param[in]  psa    Path separator for arguments
 *
 * @return     Path string
 */
char*
path_output2(const char* in, stralloc* out, const char* ext, char psa) {
  stralloc_copy(out, &dirs.build.sa);
  stralloc_catc(out, psa);

  return path_extension(in, out, ext);
}

/**
 * @brief      Returns an output path
 *
 * @param[in]  in    Input path
 * @param      out   Output path
 * @param[in]  ext   Extension
 * @param[in]  psa    Path separator for arguments
 *
 * @return     Path string
 */
char*
path_output(const char* in, stralloc* out, const char* ext, char psa) {
  return path_output2(str_basename(in), out, ext, psa);
}

/**
 * @brief       Replaces the path basename (without extensions)  with a wildcard
 *
 * @param      sa        Write output here
 * @param[in]  wildchar  The wildchar
 *
 * @return     Output string
 */
char*
path_wildcard(stralloc* sa, const char* wildchar) {
  const char* x;
  size_t n, e;

  stralloc_nul(sa);
  x = sa->s;

  if(sa->s[(n = str_rchrs(sa->s, "/\\ ", 3))]) {
    x += n;

    if(*x)
      x++;
  }

  n = x - sa->s;
  e = byte_rchr(x, sa->len - n, '.');

  if(e != sa->len - n) {
    stralloc_remove(sa, n, e);
    stralloc_insertb(sa, wildchar, n, str_len(wildchar));
  }

  stralloc_nul(sa);
  return sa->s;
}

/**
 * @brief      Cleans a path
 *
 * @param[in]  path  Path string
 *
 * @return     Cleaned path string
 */
char*
path_clean_s(const char* path) {
  if(str_startb(path, dirs.this.sa.s, dirs.this.sa.len)) {
    path += dirs.this.sa.len;

    if(path[0] == PATHSEP_C)
      path++;
  }

  while(str_start(path, "./"))
    path += 2;

  return (char*)path;
}

/**
 * @brief      Cleans a path
 *
 * @param[in]  path  Path string
 * @param      len   Path length
 *
 * @return     Cleaned path string
 */
char*
path_clean_b(const char* path, size_t* len) {
  if(byte_startb(path, *len, dirs.this.sa.s, dirs.this.sa.len)) {
    path += dirs.this.sa.len;
    *len -= dirs.this.sa.len;

    if(*len > 0 && path[0] == PATHSEP_C) {
      path++;
      (*len)--;
    }
  }

  while(byte_starts(path, *len, "./")) {
    path += 2;
    (*len) -= 2;
  }

  return (char*)path;
}

/**
 * @brief     Reads a path into a buffer
 *
 * @param[in]  path  Path sting
 * @param      n     Pointer to buffer length
 * @param[in]  psm   Path separator for makefile
 *
 * @return    Pointer to buffer or NULL
 */
const char*
path_mmap_read(const char* path, size_t* n, char psm) {
  const char* x;
  stralloc sa;

  stralloc_init(&sa);

  if(dirs.this.sa.s) {
    path_prefix_s(&dirs.this.sa, path, &sa, psm);
    stralloc_replacec(&sa, psm, PATHSEP_C);
  }

  if((x = mmap_read(sa.s, n)) == NULL) {
    errmsg_warnsys("error opening '", path, "'", 0);
    buffer_putnlflush(buffer_2);
  }

  stralloc_free(&sa);
  return x;
}

/**
 * @brief      Normalizes a path
 *
 * @param[in]  dir   Directory path
 * @param      out   Output buffer
 */
void
path_normalize_sa(const char* dir, stralloc* out) {
  stralloc tmp;

  stralloc_init(&tmp);
  stralloc_zero(out);

  if(str_start(dir, "./"))
    dir += 2;

  if(!path_is_absolute(dir)) {
    stralloc_copy(&tmp, &dirs.this.sa);
    path_absolute_sa(&tmp);

    path_appends(dir, &tmp);
    path_canonical_sa(&tmp);
  } else {
    path_canonical(dir, &tmp);
  }

  /*if(dirs.out.sa.s) {
    stralloc outdir;
    stralloc_init(&outdir);
    stralloc_copy(&outdir, &dirs.out.sa);
    path_absolute_sa(&outdir);
    path_relative_to_b(tmp.s, tmp.len, outdir.s, outdir.len, out);
    stralloc_free(&outdir);
  } else*/
  stralloc_copy(out, &tmp);

  stralloc_free(&tmp);
}

/**
 * @brief      Normalizes a path
 *
 * @param[in]  x     Input path
 * @param[in]  len   Input path length
 * @param      out   Output path
 */
void
path_normalize_sa_b(const char* x, size_t len, stralloc* out) {
  stralloc tmp;

  stralloc_init(&tmp);
  stralloc_copyb(&tmp, x, len);
  stralloc_nul(&tmp);

  path_normalize_sa(tmp.s, out);

  if(stralloc_ends(out, "/."))
    out->len -= 2;

  stralloc_nul(out);
  stralloc_free(&tmp);
}

/**
 * @brief      Gets directory name from a file path (allocated).
 *
 * @param      p     Path string
 *
 * @return     Directory string
 */
char*
path_dirname_alloc(const char* p) {
  size_t len = str_len(p);
  size_t pos = str_rchrs(p, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);

  if(pos < len)
    return str_ndup(p, pos);

  return str_dup(".");
}

/**
 * @brief      Adds a path to a set
 *
 * @param      out     Output set
 * @param[in]  path  Path string
 * @param[in]  len   Path length
 */
int
add_path_b(set_t* out, const char* path, size_t len) {
  if(set_has(out, path, len))
    return 0;

  set_insert(out, path, len);
  return 1;
}

/**
 * @brief      Adds a path to a set
 *
 * @param      out     Output set
 * @param[in]  path  Path string
 */
int
add_path(set_t* out, const char* path) {
  return add_path_b(out, path, str_len(path));
}

/**
 * @brief      Adds a path to a set
 *
 * @param      out   Output set
 * @param      path  Path string
 */
int
add_path_sa(set_t* out, stralloc* path) {
  return add_path_b(out, path->s, path->len);
}
