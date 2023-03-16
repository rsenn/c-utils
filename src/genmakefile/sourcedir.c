#include <assert.h>
#include "sources.h"
#include "is.h"
#include "../../lib/dlist.h"
#include "../../lib/mmap.h"

MAP_T sourcedirs;
const char* srcdir_varname = "DISTDIR";

void extract_includes(const char*, size_t, strlist*, int);
void extract_pptok(const char*, size_t, set_t*);

void
sourcedir_addsource(const char* source,
                    strarray* srcs,
                    const char* binext,
                    const char* srcext,
                    stralloc* thisdir,
                    stralloc* outdir,
                    strarray* progs,
                    strarray* bins,
                    char pathsep_make) {
  stralloc r, dir, tmp;
  strlist l;
  size_t n, dlen;
  const char *x, *s;
  set_iterator_t it;
  sourcedir* srcdir;
  sourcefile* file = sources_new(source, binext, progs, bins);
  stralloc_init(&dir);
  stralloc_init(&tmp);
  strlist_init(&l, '\0');
  stralloc_init(&r);
  strlist_zero(&l);
  path_dirname(source, &dir);
  stralloc_nul(&dir);
  dlen = dir.len;
  source = path_clean_s(source, thisdir);
  srcdir = sourcedir_getsa(&dir);
  slist_add(&srcdir->sources, &file->link);
  struct dnode* node = alloc(sizeof(struct dnode) + sizeof(sourcefile*));
  dlist_data(node, sourcefile*) = file;
  dlist_push(&sourcelist, node);
  ++srcdir->n_sources;
  if(!path_exists(source)) {

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Path doesn't exist: ");
    buffer_puts(buffer_2, source);
    buffer_putnlflush(buffer_2);
#endif

    return;

#if defined(__x86_64__) || defined(__i386__)
    __asm__("int3");
#endif
  }
  if((x = path_mmap_read(source, &n, thisdir, pathsep_make)) != 0) {
    extract_includes(x, n, &l, 0);
    extract_pptok(x, n, &file->pptoks);
    mmap_unmap(x, n);
  }
  sources_addincludes(file, srcdir, &l, srcs, srcext, thisdir, outdir);
  set_foreach_it(&file->pptoks, it) {
    x = set_iterator_value(&it, &n);
    if(n >= 5 && (byte_equal(x, 4, "USE_") || byte_equal(x, 5, "HAVE_")))
      set_add(&srcdir->pptoks, x, n);
  }
  stralloc_replacec(&l.sa, PATHSEP_C == '\\' ? '/' : '\\', PATHSEP_C);
  strlist_foreach(&l, s, n) {
    dir.len = dlen;
    stralloc_catc(&dir, PATHSEP_C);
    stralloc_catb(&dir, s, n);
    stralloc_nul(&dir);
    stralloc_copy(&r, &dir);
    path_canonical_sa(&r);
    set_addsa(&srcdir->includes, &r);
  }
  dir.len = dlen;
  stralloc_free(&r);
  strlist_free(&l);
  stralloc_free(&dir);
}

/**
 * @defgroup source dir functions
 * @{
 */
/**
 * @brief sourcedir_find  Searches for a source directory
 * @param path           Path string
 * @return               Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_find(const char* path) {
  sourcedir** ptr;
  if((ptr = MAP_GET(sourcedirs, path, str_len(path) + 1)))
    return *ptr;
  return 0;
}

/**
 * @brief sourcedir_findsa Searches for a source directory
 * @param path             Path stralloc
 * @return                 Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_findsa(stralloc* path) {
  sourcedir** ptr;
  stralloc_nul(path);
  if((ptr = MAP_GET(sourcedirs, path->s, path->len + 1)))
    return *ptr;
  return 0;
}

/**
 * @brief sourcedir_findb  Searches for a source directory
 * @param x                Path buffer
 * @param n                Length of path
 * @return               Pointer to sourcedir structure or NULL
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

sourcedir*
sourcedir_getb(const char* x, size_t n) {
  sourcedir *s, **ptr;
  if(!(s = sourcedir_findb(x, n))) {
    sourcedir* newdir;
    newdir = alloc_zero(sizeof(sourcedir));
    set_init(&newdir->pptoks, 0);
    MAP_INSERT(sourcedirs, x, n + 1, &newdir, sizeof(newdir));
    if((ptr = (sourcedir**)MAP_GET(sourcedirs, x, n + 1)))
      s = *ptr;
  }
  return s;
}

sourcedir*
sourcedir_getsa(stralloc* path) {
  stralloc_nul(path);
  return sourcedir_getb(path->s, path->len);
}

/**
 * @brief sourcedir_populate  Creates a hash-map of all source directories
 * @param srcs
 * @param sourcedirs
 */
void
sourcedir_populate(strarray* srcs) {
  MAP_PAIR_T t;
  strlist d;
  const char* x;
  size_t n;
  strlist_init(&d, '\0');

  MAP_FOREACH(sourcedirs, t) {
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
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* dir = *(sourcedir**)MAP_ITER_VALUE(t);
    strlist_zero(&d);
    sourcedir_deps(dir, &d);
    strlist_foreach(&d, x, n) { set_add(&dir->deps, x, n); }
  }
}

/**
 * @brief sourcedir_dump_all
 * @param b
 * @param sourcedirs
 */
void
sourcedir_dump_all(buffer* b) {
  MAP_PAIR_T t;
  MAP_FOREACH(sourcedirs, t) {
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

void
sourcedir_deps(sourcedir* srcdir, strlist* out) {
  uint32 serial = uint32_random();
  return sourcedir_dep_recursive(srcdir, out, serial, 0);
}

void
sourcedir_deps_s(const char* srcdir, strlist* out) {
  sourcedir* sdir = sourcedir_getb(srcdir, str_len(srcdir) + 1);
  assert(sdir);
  return sourcedir_deps(sdir, out);
}

void
sourcedir_deps_b(const char* sdir, size_t sdirlen, strlist* out) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copyb(&sa, sdir, sdirlen);
  return sourcedir_deps_s(sa.s, out);
}

void
sourcedir_printdeps(sourcedir* srcdir, buffer* b, int depth) {
  const char* s;
  size_t n;
  set_iterator_t it;
  sourcedir* sdir;
  strlist deps;
  strlist_init(&deps, '\0');
  set_foreach(&srcdir->deps, it, s, n) {
    if(strlist_pushb_unique(&deps, s, n)) {
      buffer_putnspace(buffer_2, depth * 2);
      buffer_put(buffer_2, s, n);
      buffer_putnlflush(buffer_2);
      if((sdir = sourcedir_findb(s, n)))
        sourcedir_printdeps(sdir, b, depth + 1);
    }
  }
  strlist_free(&deps);
}
