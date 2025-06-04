#ifndef GENMAKEFILE_SOURCEDIR_H
#define GENMAKEFILE_SOURCEDIR_H

#define MAP_USE_HMAP 1
#include "../../lib/array.h"
#include "../../lib/buffer.h"
#include "../../lib/map.h"
#include "../../lib/set.h"
#include "../../lib/slist.h"
#include "../../lib/strarray.h"
#include "../../lib/strlist.h"
#include "../../lib/uint32.h"

typedef struct {
  int n_sources;
  slink* sources;
  set_t includes;
  array rules;
  set_t pptoks;
  set_t deps;
  uint32 serial;
} sourcedir;

struct sourcefile_s* sourcedir_addsource(const char*, strarray*, strarray*, strarray*, char);
void sourcedir_dep_recursive(sourcedir*, strlist*, uint32, sourcedir*);
void sourcedir_deps_b(const char*, size_t, strlist*);
void sourcedir_deps_s(const char*, strlist*);
void sourcedir_deps(sourcedir*, strlist*);
void sourcedir_dump_all(buffer*);
sourcedir* sourcedir_findb(const char*, size_t);
sourcedir* sourcedir_find(const char*);
sourcedir* sourcedir_findsa(stralloc*);
sourcedir* sourcedir_getb(const char*, size_t);
sourcedir* sourcedir_getsa(stralloc*);
void sourcedir_populate(strarray*);

extern MAP_T srcdir_map;
extern const char* srcdir_varname;

#endif /* defined(GENMAKEFILE_SOURCEDIR_H) */
