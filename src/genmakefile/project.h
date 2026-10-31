#ifndef GENMAKEFILE_PROJECT_H
#define GENMAKEFILE_PROJECT_H

#include "../../lib/strarray.h"

/* directory with the lib modules (lib/<name>.h and lib/<name>/ with the sources) */
extern const char* project_libdir;

/* -D options, applied to the preprocessor run that finds the dependencies */
extern strarray project_defs;

/* The names that the sources ask about in a conditional without any macro of
   that name existing, found by project_generate: tested with #ifdef, #ifndef
   or defined(), and used by value in an #if or #elif. */
extern strarray project_tested, project_valued;

/* only names that begin like this are switches (default USE_; empty: all) */
extern const char* project_switch_prefix;
extern strarray project_dep_names, project_headers;
extern const char* project_libname;

/* The names of those two lists that a build can be given, sorted. */
void project_switches(strarray* out);
void project_deps(strarray* out);

/* Finds the programs (sources with a main()) and lib modules below args, scans
   each source with the macros of the selected compiler and adds the compile,
   library and link rules to the rule map. */
int project_generate(strarray* args, strarray* includes, int bins, int libs);

#endif /* defined(GENMAKEFILE_PROJECT_H) */
