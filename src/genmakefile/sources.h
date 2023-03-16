#ifndef GENMAKEFILE_SOURCES_H
#define GENMAKEFILE_SOURCES_H

#include <stdbool.h>
#include "types.h"
#include "../../lib/set.h"
#include "../../lib/slist.h"
#include "../../lib/strlist.h"
#include "../../lib/strarray.h"

typedef struct {
  int n_sources;
  slink* sources;
  set_t includes;
  array rules;
  set_t pptoks;
  set_t deps;
  uint32 serial;
} sourcedir;

typedef struct {
  union {
    struct slink link;
    struct sourcefile* next;
  };
  const char* name;
  int has_main;
  strlist includes;
  set_t deps;
  set_t pptoks;
} sourcefile;

bool sources_iscplusplus(void);

int sources_add_b(const char*, size_t, stralloc*);
int sources_add(const char*, stralloc*);
void sources_addincludes(sourcefile*, sourcedir*, const strlist*, strarray*, const char*, stralloc*, stralloc*);
void sources_deps(sourcefile*, strlist*);
const char* sources_find(const char*, size_t, size_t*);
void sources_get(const char*, stralloc*);
sourcefile* sources_new(const char*, const char*, strarray*, strarray*);
void sources_readdir(stralloc*, strarray*, stralloc*, stralloc*);
int sources_sort(const char**, const char**);

extern set_t srcs;
extern stralloc srcdir;

#endif /* defined(GENMAKEFILE_SOURCES_H) */
