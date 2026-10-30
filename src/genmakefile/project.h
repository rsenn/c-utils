#ifndef GENMAKEFILE_PROJECT_H
#define GENMAKEFILE_PROJECT_H

#include "../../lib/strarray.h"

/* directory with the lib modules (lib/<name>.h and lib/<name>/ with the sources) */
extern const char* project_libdir;

/* -D options, applied to the preprocessor run that finds the dependencies */
extern strarray project_defs;

/* Finds the programs (sources with a main()) and lib modules below args, scans
   each source with the macros of the selected compiler and adds the compile,
   library and link rules to the rule map. */
int project_generate(strarray* args, strarray* includes, int bins, int libs);

#endif /* defined(GENMAKEFILE_PROJECT_H) */
