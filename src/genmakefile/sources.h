#ifndef GENMAKEFILE_SOURCES_H
#define GENMAKEFILE_SOURCES_H

#include "../../lib/bool.h"
#include "sourcedir.h"
#include "../../lib/dlist.h"

typedef struct sourcefile_s {
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

sourcefile* sources_new(const char*, const char* binext, strarray* progs, strarray* bins);
int sources_add(const char*);
int sources_add_b(const char*, size_t len);
int sources_sort_callback(const char**, const char** b);
void sources_get(const char*);
const char* sources_find(const char*, size_t len, size_t* cptr);
bool sources_iscplusplus(void);
void sources_deps(sourcefile*, strlist* out);
void sources_readdir(stralloc*, strarray* out);
void sources_addincludes(sourcefile*, sourcedir* sdir, const strlist* includes, strarray* sources);

extern set_t sources_set;
extern stralloc sources_dir;
extern dlist sources_list;

#endif /* defined(GENMAKEFILE_SOURCES_H) */
