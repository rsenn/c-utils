#ifndef GENMAKEFILE_SOURCES_H
#define GENMAKEFILE_SOURCES_H

#include <stdbool.h>
#include "sourcedir.h"
#include "../../lib/dlist.h"

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
extern dlist sourcelist;

#endif /* defined(GENMAKEFILE_SOURCES_H) */
