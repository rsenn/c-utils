#include <assert.h>
#include "sources.h"
#include "is.h"
#include "ansi.h"
#include "includes.h"
#include "../../genmakefile.h"

MAP_T sourcedir_map;
const char* sourcedir_varname = "DISTDIR";

static const char tok_charset[] = {
    '_', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
};

/**
 * @brief      Determines whether the specified ch is newline.
 *
 * @param[in]  ch    { parameter_description }
 *
 * @return     True if the specified ch is newline, False otherwise.
 */
static inline bool
is_newline(const char ch) {
  return ch == '\r' || ch == '\n';
}

/**
 * @brief      Extract tokens from a buffer
 *
 * @param[in]  x       Buffer
 * @param[in]  n       Buffer length
 * @param      tokens  Set that receives tokens
 */
static void
extract_tokens(const char* x, size_t n, set_t* tokens) {
  size_t i;

  for(; n; x += i, n -= i) {
    if(is_newline(*x))
      break;

    if((i = scan_noncharsetnskip(x, tok_charset, n)) == n)
      break;

    x += i;
    n -= i;

    if(is_newline(*x))
      break;

    if((i = scan_charsetnskip(x, tok_charset, n)) > 0 && !(i == 7 && byte_equal(x, 7, "defined")))

      if(!(*x >= '0' && *x <= '9')) {
        if(set_add(tokens, x, i) == 1) {

#ifdef DEBUG_OUTPUT_
          debug_byte("added tok", x, i);
#endif
        }
      }

    /*if(i == n)
      break;*/
  }
}

/**
 * @brief     Extracts preprocessor tokens from a buffer
 *
 * @param[in]  x       Buffer
 * @param[in]  n       Buffer length
 * @param      tokens  Set that receives tokens
 */
static void
extract_pptok(const char* x, size_t n, set_t* tokens) {
  size_t i, len, pos;

  while(n) {
    if((i = scan_charsetnskip(x, " \t\r\n", n)) == n)
      break;

    x += i;
    n -= i;

    if(*x == '#') {
      x += 1;
      n -= 1;

      if((i = scan_charsetnskip(x, " \t\r", n)) == n)
        break;

      x += i;
      n -= i;

      if((i = scan_noncharsetnskip(x, " \t\r\n<\"", n)) == n)
        break;

      if(!(i == 7 && byte_equal(x, 7, "include"))) {
        if((i >= 2 && byte_equal(x, 2, "if"))) {
          x += i;
          n -= i;

          len = byte_chrs(x, n, "\r\n", 2);
          pos = byte_findb(x, n, "//", 2);

          while(len > 0 && len < n) {
            if(x[len - 1] == '\\') {
              if(x[len] == '\r' && x[len + 1] == '\n')
                len++;

              if(len + 1 < n) {
                len += 1;
                len += byte_chrs(&x[len], n - len, "\r\n", 2);
                continue;
              }
            }
            break;
          }

          if(pos < len)
            len = pos;

#ifdef DEBUG_OUTPUT_
          buffer_puts(buffer_2, "pptoks: ");
          buffer_put(buffer_2, x, len);
          buffer_putnlflush(buffer_2);
#endif
          extract_tokens(x, len, tokens);
        }
      }
    }

    if((i = byte_chr(x, n, '\n')) >= n)
      break;

    x += i;
    n -= i;
  }
}

/**
 * @defgroup   Source directory functions
 * @{
 *
 * @brief      Add source file to source directory
 *
 * @param[in]  source   Source file
 * @param      sources  Global sources array
 * @param      progs    Programs array
 * @param      bins     Binaries array
 * @param[in]  psm      Path separator for makefile
 */
struct sourcefile_s*
sourcedir_addsource(const char* source, strarray* sources, strarray* progs, strarray* bins, char psm) {
  stralloc r, dir, tmp;
  strlist list;
  size_t n, dlen;
  const char *x, *s;
  set_iterator_t it;
  sourcedir* srcdir;
  sourcefile* file;
  struct dnode* node;

  if(!path_exists(source)) {
#ifdef DEBUG_OUTPUT
    buffer_puts(debug_buf, "Path doesn't exist: ");
    buffer_puts(debug_buf, source);
    buffer_putnlflush(debug_buf);
#endif

    return 0;

#if defined(__x86_64__) || defined(__i386__)
    __asm__("int3");
#endif
  }

  if(!(file = sources_new(source, exts.bin, progs, bins)))
    return 0;

  stralloc_init(&dir);
  stralloc_init(&tmp);
  stralloc_init(&r);
  strlist_init(&list, '\0');
  strlist_zero(&list);

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(debug_buf, "[1]", BLUE256, "sourcedir_addsource(", NC, source, BLUE256, ") ", NC, NULL);
  buffer_putnlflush(debug_buf);
#endif

  path_dirname(source, &dir);
  stralloc_nul(&dir);
  dlen = dir.len;
  source = path_clean_s(source);
  srcdir = sourcedir_getsa(&dir);

  slist_add(&srcdir->sources, &file->link);

  node = alloc(sizeof(struct dnode) + sizeof(sourcefile*));

  dlist_data(node, sourcefile*) = file;
  dlist_push(&sources_list, node);

  ++srcdir->n_sources;

  if((x = path_mmap_read(source, &n, psm)) != 0) {
    includes_extract(x, n, &list, 0);
    extract_pptok(x, n, &file->pptoks);
    mmap_unmap(x, n);
  }

  sources_addincludes(file, srcdir, &list, sources);

  set_foreach_it(&file->pptoks, it) {
    x = set_iterator_value(&it, &n);

    if(n >= 5 && (byte_equal(x, 4, "USE_") || byte_equal(x, 5, "HAVE_")))
      set_add(&srcdir->pptoks, x, n);
  }

  stralloc_replacec(&list.sa, PATHSEP_C == '\\' ? '/' : '\\', PATHSEP_C);

  strlist_foreach(&list, s, n) {
    dir.len = dlen;

    stralloc_catc(&dir, PATHSEP_C);
    stralloc_catb(&dir, s, n);
    stralloc_nul(&dir);
    stralloc_copy(&r, &dir);

    path_canonical_sa(&r);

    if(path_exists(r.s) || includes_find_sa(s, n, &r)) {
#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "[2]", GREEN256, "sourcedir_addsource(", NC, source, GREEN256, ") ", NC, "Adding include ", 0);
      buffer_putsa(buffer_2, &r);
      buffer_putnlflush(buffer_2);
#endif

      set_addsa(&srcdir->includes, &r);
    }
  }

  dir.len = dlen;

  strlist_free(&list);
  stralloc_free(&dir);
  stralloc_free(&tmp);
  stralloc_free(&r);

  return file;
}

/**
 * @brief      Searches for a source directory
 *
 * @param      path  Path string
 *
 * @return     Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_find(const char* path) {
  sourcedir** ptr;

  if((ptr = MAP_GET(sourcedir_map, path, str_len(path) + 1)))
    return *ptr;

  return 0;
}

/**
 * @brief      Searches for a source directory
 *
 * @param      path  Path stralloc
 *
 * @return     Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_findsa(stralloc* path) {
  sourcedir** ptr;

  stralloc_nul(path);

  if((ptr = MAP_GET(sourcedir_map, path->s, path->len + 1)))
    return *ptr;

  return 0;
}

/**
 * @brief      Searches for a source directory
 *
 * @param      x     Path buffer
 * @param      n     Length of path
 *
 * @return     Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_findb(const char* x, size_t n) {
  sourcedir* ret;
  stralloc p;

  stralloc_init(&p);
  stralloc_copyb(&p, x, n);
  ret = sourcedir_findsa(&p);
  stralloc_free(&p);
  return ret;
}

/**
 * @brief      Gets a source directory (searches first, if not found, create it)
 *
 * @param[in]  x     Source directory name
 * @param[in]  n     Source directory name length
 *
 * @return     Pointer to sourcedir structure
 */
sourcedir*
sourcedir_getb(const char* x, size_t n) {
  sourcedir *s, **ptr;

  if(!(s = sourcedir_findb(x, n))) {
    sourcedir* newdir = alloc_zero(sizeof(sourcedir));

    set_init(&newdir->pptoks, 0);
    MAP_INSERT2(sourcedir_map, x, n + 1, &newdir, sizeof(newdir));

    if((ptr = (sourcedir**)MAP_GET(sourcedir_map, x, n + 1)))
      s = *ptr;
  }

  return s;
}

/**
 * @brief      Gets a source directory (searches first, if not found, create it)
 *
 * @param      path  Source directory path
 *
 * @return     Pointer to sourcedir structure
 */
sourcedir*
sourcedir_getsa(stralloc* path) {
  stralloc_nul(path);

  return sourcedir_getb(path->s, path->len);
}

/**
 * @brief      Creates a hash-map of all source directories
 *
 * @param      sources_set  The sources set
 */
void
sourcedir_populate(strarray* sources_set) {
  MAP_PAIR_T t;
  strlist d;
  const char* x;
  size_t n;

  strlist_init(&d, '\0');

  MAP_FOREACH(sourcedir_map, t) {
    sourcedir* dir = *(sourcedir**)MAP_ITER_VALUE(t);
    sourcefile* file;

#ifdef DEBUG_OUTPUT_
    const char* name = MAP_ITER_KEY(t);
    buffer_puts(buffer_2, "sourcedir: ");
    buffer_puts(buffer_2, name);
    buffer_putnlflush(buffer_2);
#endif

    strlist_zero(&d);

    slist_foreach(dir->sources, file) {
      sources_deps(file, &d);
      strlist_foreach(&d, x, n) { set_add(&file->deps, x, n); }
    }
  }

  MAP_FOREACH(sourcedir_map, t) {
    sourcedir* dir = *(sourcedir**)MAP_ITER_VALUE(t);

    strlist_zero(&d);
    sourcedir_deps(dir, &d);
    strlist_foreach(&d, x, n) { set_add(&dir->deps, x, n); }
  }

  strlist_free(&d);
}

/**
 * @brief      Dumps all source directories
 *
 * @param      b     Output buffer
 */
void
sourcedir_dump_all(buffer* b) {
  MAP_PAIR_T t;

  MAP_FOREACH(sourcedir_map, t) {
    sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
    sourcefile* pfile;

    buffer_puts(b, " '");
    buffer_put(b, MAP_ITER_KEY(t), str_len(MAP_ITER_KEY(t)));
    buffer_puts(b, "' (");
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): [");

    slist_foreach(srcdir->sources, pfile) {
      buffer_putspace(b);
      buffer_puts(b, pfile->name);
    }

    buffer_puts(b, " ]");
    buffer_putnlflush(b);
  }
}

/**
 * @brief      Gets recursive dependencies of a source directory
 *
 * @param      srcdir       The source directory
 * @param      out          Output list
 * @param[in]  serial       Serial number
 * @param      parent       Parent directory
 */
void
sourcedir_dep_recursive(sourcedir* srcdir, strlist* out, uint32 serial, sourcedir* parent) {
  const char* s;
  size_t n;
  set_iterator_t it;
  sourcedir* sdir;

  if(srcdir->serial == serial)
    return;

  set_foreach(&srcdir->deps, it, s, n) {
    if(!strlist_containsb(out, s, n)) {
      if((sdir = sourcedir_findb(s, n)) && sdir != srcdir) {
        if(sdir->serial == serial)
          continue;

        if(sdir == parent)
          continue;

        srcdir->serial = serial;
        sourcedir_dep_recursive(sdir, out, serial, srcdir);
        strlist_pushb(out, s, n);
      }
    }
  }
}

/**
 * @brief      Get all dependencies of a source directory
 *
 * @param      srcdir  The source directory
 * @param      out          Output list
 */
void
sourcedir_deps(sourcedir* srcdir, strlist* out) {
  uint32 serial = uint32_random();

  return sourcedir_dep_recursive(srcdir, out, serial, 0);
}

/**
 * @brief      Get all dependencies of a source directory
 *
 * @param[in]  name  Source directory name
 * @param      out   Output list
 */
void
sourcedir_deps_s(const char* name, strlist* out) {
  sourcedir* sdir = sourcedir_getb(name, str_len(name) + 1);
  assert(sdir);
  return sourcedir_deps(sdir, out);
}

/**
 * @brief      Get all dependencies of a source directory
 *
 * @param[in]  name     Source directory name
 * @param[in]  namelen  Source directory name length
 * @param      out      Output list
 */
void
sourcedir_deps_b(const char* name, size_t namelen, strlist* out) {
  stralloc sa;

  stralloc_init(&sa);
  stralloc_copyb(&sa, name, namelen);

  sourcedir_deps_s(sa.s, out);

  stralloc_free(&sa);
}

/**
 * @}
 */
