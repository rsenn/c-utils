#include "sources.h"
#include "is.h"
#include "ansi.h"
#include "../../lib/rdir.h"
#include "../../debug.h"
#include "../../genmakefile.h"

set_t sources_set = {0};
stralloc sources_dir = {0, 0, 0};
dlist sources_list = {0};

int main_present(const char*);

/**
 * @brief sources_new  Create new source
 * file entry.
 * @param name
 * @return
 */
sourcefile*
sources_new(const char* name, const char* binext, strarray* progs, strarray* bins) {
  sourcefile* ret;
  if((ret = (sourcefile*)malloc(sizeof(sourcefile)))) {
    byte_zero(ret, sizeof(sourcefile));
    ret->name = str_dup(name);
    ret->has_main = is_source(ret->name) && main_present(ret->name) == 1;
    if(ret->has_main) {

#ifdef DEBUG_OUTPUT
      debug_str("Source has main()", ret->name);
#endif

      {
        stralloc bin;
        size_t n = str_len(ret->name);
        stralloc_init(&bin);
        while(n > 0 && ret->name[n - 1] != '.')
          n--;
        path_extension(ret->name, &bin, binext);
        strarray_push_unique(progs, ret->name);
        strarray_push_sa(bins, &bin);

#ifdef DEBUG_OUTPUT
        debug_sa("bin", &bin);
#endif

        stralloc_free(&bin);
      }
    }
    set_init(&ret->pptoks, 0);
    // slist_pushb(&sources, &ret, sizeof(ret));
    return ret;
  }
  return 0;
}

/**
 * @brief sources_add  Adds a source
 * file to the given list.
 * @param filename
 * @param sources
 */
int
sources_add(const char* source) {
  return sources_add_b(source, str_len(source));
}

int
sources_add_b(const char* x, size_t len) {

  if(byte_chr(x, len, '/') == len && byte_ends(x, len, "strlist_shift.c")) {

#ifdef SIGTRAP
    raise(SIGTRAP);
#endif
  }
  if(is_source_b(x, len) || is_include_b(x, len)) {
    if(len > dirs.this.sa.len && byte_startb(x, len, dirs.this.sa.s, dirs.this.sa.len)) {
      size_t dirlen = dirs.this.sa.len + 1;
      x += dirlen;
      len -= dirlen;
    }
    if(set_add(&sources_set, x, len)) {
#ifdef DEBUG_OUTPUT_
      debug_byte("sources_add", x, len);
#endif
    }
  }
  return 0;
}

int
sources_sort(const char** a, const char** b) {
  size_t alen = str_rchrs(*a, "/\\", 2);
  size_t blen = str_rchrs(*b, "/\\", 2);
  int er, rdir, rfile;
  const char *ext_a, *ext_b;
  ext_a = *a + str_rchr(*a, '.');
  ext_b = *b + str_rchr(*b, '.');
  er = str_diff(ext_a, ext_b);
  if(er)
    return er;
  rdir = str_diffn(*a, *b, alen < blen ? blen : alen);
  rfile = str_diff(path_basename(*a), path_basename(*b));
  if(rdir == 0)
    return rfile;
  return rdir;
}

/**
 * @brief sources_get Searches all
 * source files in the given directory
 * and creates a string-array.
 * @param basedir
 * @param sources
 */
void
sources_get(const char* basedir) {
  rdir_t rdir;

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "sources_get: ");
  buffer_puts(buffer_2, basedir);
  buffer_putnlflush(buffer_2);
#endif

  if(!rdir_open(&rdir, basedir)) {
    const char* s;
    while((s = rdir_read(&rdir))) {
      size_t len = str_len(s);
      if(len + 1 > dirs.this.sa.len && byte_equal(s, dirs.this.sa.len, dirs.this.sa.s) && path_is_separator(s[dirs.this.sa.len])) {
        s += dirs.this.sa.len + 1;
      }
      if(sources_add(s)) {
      }
      {

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "Added source: ");
        buffer_puts(buffer_2, s);
        buffer_putnlflush(buffer_2);
#endif
      }
    }
  }
}

const char*
sources_find(const char* name, size_t len, size_t* cptr) {
  char* x;
  const char* ret = 0;
  set_iterator_t it;
  size_t i, n, count = 0;

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "Find source: ");
  buffer_put(buffer_2, name, len);
  buffer_putnlflush(buffer_2);
#endif

  set_foreach(&sources_set, it, x, n) {
    i = byte_findb(x, n, name, len);
    if(i == n)
      i = byte_findb(x, n, path_basename(name), len);
    if(i < n) {
      if(!ret && is_source(x))
        ret = x;
      count++;
    }
  }
  if(cptr)
    *cptr = count;
  if(ret) {

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Found source: ");
    buffer_puts(buffer_2, ret);
    buffer_puts(buffer_2, "(query = '");
    buffer_put(buffer_2, name, len);
    buffer_puts(buffer_2, " ', ");
    buffer_putulong(buffer_2, count);
    buffer_puts(buffer_2, " results)");
    bufferfer_putnlflush(buffer_2);
#endif
  }
  return ret;
}

bool
sources_iscplusplus() {
  char* x;
  size_t n;
  set_iterator_t it;
  set_foreach(&sources_set, it, x, n) {
    if(byte_ends(x, n, "pp"))
      return true;
    if(byte_ends(x, n, "xx"))
      return true;
  }
  return false;
}

void
sources_deps(sourcefile* file, strlist* out) {
  const char* x;
  size_t len;
  set_iterator_t it, it2;
  sourcefile* src;
  sourcedir* dir;
  strlist_foreach(&file->includes, x, len) { strlist_pushb_unique(out, x, len); }
  set_foreach(&file->deps, it, x, len) {
    stralloc_nul(&sources_dir);

#ifdef DEBUG_OUTPUT_I_q
    buffer_puts(buffer_2, "sources_deps '");
    buffer_puts(buffer_2, file->name);
    buffer_puts(buffer_2, "' ");
    buffer_put(buffer_2, x, len);
    buffer_putnlflush(buffer_2);
#endif

    if((dir = sourcedir_findb(x, len))) {
      const char* s;
      size_t n;
      set_foreach(&dir->deps, it2, s, n) { strlist_pushb_unique(out, s, n); }
      slist_foreach(dir->sources, src) {

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "sources_deps");
        buffer_puts(buffer_2, " s: ");
        buffer_puts(buffer_2, src->name);
        buffer_putnlflush(buffer_2);
#endif

        strlist_push_unique(out, src->name);
      }
    }
  }
}

void
sources_readdir(stralloc* dir, strarray* out) {
  rdir_t d;
  stralloc srcdir;
  stralloc_init(&srcdir);
  stralloc_copy(&srcdir, dir);
  stralloc_nul(&srcdir);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "sources_readdir: ");
  buffer_putsa(buffer_2, dir);
  buffer_putnlflush(buffer_2);
#endif

  // path_concatb(dirs.this.sa.s, dirs.this.sa.len, dir->s, dir->len, &srcdir);
  if(!rdir_open(&d, srcdir.s)) {
    const char* s;
    while((s = rdir_read(&d))) {
      if(!is_source(s) && !is_include(s))
        continue;
      if(str_start(s, dirs.out.sa.s))
        s += dirs.out.sa.len;
      else
        s += dir->len;
      if(*s == PATHSEP_C)
        s++;
      /*  buffer_puts(buffer_2,
        "rdir_read: ");
        buffer_puts(buffer_2, s);
        buffer_putnlflush(buffer_2);*/
      strarray_push_unique(out, s);
    }
  }
  stralloc_free(&srcdir);
}

void
sources_addincludes(sourcefile* file, sourcedir* sdir, const strlist* includes, strarray* sources) {
  const char* x;
  size_t n;
  stralloc basedir, dir, path, real, relative;
  strlist directories;

  stralloc_init(&real);
  stralloc_init(&dir);
  stralloc_init(&basedir);
  stralloc_init(&path);
  stralloc_init(&relative);
  strlist_init(&directories, '\0');

  path_dirname(file->name, &basedir);
  path_absolute_sa(&basedir);

  strlist_init(&file->includes, '\0');

  stralloc_nul(&basedir);

  if(stralloc_starts(&basedir, dirs.this.sa.s))
    stralloc_remove(&basedir, 0, dirs.this.sa.len + 1);

  stralloc_copy(&relative, &basedir);
  stralloc_nul(&relative);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "[1]", YELLOW256, "sources_addincludes(", NC, file->name, YELLOW256, ")", NC, "(2) file=", file->name, " relative=", 0);
  buffer_putsa(buffer_2, &relative);
  buffer_puts(buffer_2, "\nIncludes: ");
  strlist_dump(buffer_2, includes);
  buffer_putnlflush(buffer_2);
#endif

  relative.s = path_clean_b(relative.s, &relative.len);

  strlist_foreach(includes, x, n) {
    size_t len = n;
    bool exists = false;

    path_concatb(relative.s, relative.len, x, len, &path);
    path_collapse_sa(&path);
    path_concatb(dirs.this.sa.s, dirs.this.sa.len, path.s, path.len, &real);

    path_canonical_sa(&real);
    path_collapse_sa(&real);

    exists = path_exists(real.s);

    if(!exists) {
      if((exists = includes_find_sa(x, n, &path))) {
        stralloc_copy(&real, &path);
        path_canonical_sa(&real);
        path_collapse_sa(&real);
      }
    }

    strlist_pushb_unique(&file->includes, path.s, path.len);
    strarray_pushb_unique(sources, path.s, path.len);

    if(len >= 2 && real.s[real.len - 2] == '.') {
      stralloc_copyb(&dir, path.s, path.len - 2);
      stralloc_nul(&dir);
      real.len -= 2;
      path.len -= 2;
      stralloc_nul(&path);
      stralloc_nul(&real);

      if(stralloc_diff(&basedir, &dir)) {
        if(exists) {
          strarray a;
          strarray_init(&a);
          sources_readdir(&real, &a);
          sources_get(real.s);
          set_addsa(&file->deps, &path);
          set_addsa(&sdir->deps, &path);
        }
      }

      stralloc_cats(&real, exts.src);
      stralloc_nul(&real);

      if(path_exists(real.s) && !path_is_directory(real.s)) {
        stralloc_cats(&path, exts.src);
        stralloc_nul(&path);

#ifdef DEBUG_OUTPUT_
        buffer_putm_internal(buffer_2, "[4]", YELLOW256, "sources_addincludes(", NC, file->name, YELLOW256, ")", NC, ": ", "Adding ", 0);
        buffer_putsa(buffer_2, &path);
        buffer_putnlflush(buffer_2);
#endif

        strarray_pushb_unique(sources, path.s, path.len);
      }
    }
  }
}

/**
 * @}
 */
