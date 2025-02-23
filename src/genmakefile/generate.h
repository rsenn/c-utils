#ifndef GENMAKEFILE_GENERATE_H
#define GENMAKEFILE_GENERATE_H

#include "../../lib/bool.h"
#include "types.h"
#include "rule.h"
#include "sourcedir.h"

target* generate_single_rule(stralloc* output, stralloc* cmd);
void generate_clean_rule(char psm);
target* generate_mkdir_rule(stralloc* dir);
target* generate_srcdir_compile_rules(sourcedir* srcdir, const char* dir, bool shell, bool batch, bool batchmode, char psa, char psm);
target* generate_simple_compile_rules(sourcedir* srcdir, const char* dir, const char* fromext, const char* toext, stralloc* cmd, char psa);
target* generate_srcdir_lib_rule(sourcedir* srcdir, const char* name, bool shell, bool batch, bool batchmode, char psa, char psm);
void generate_srcdir_rule(sourcedir* sdir, bool batchmode, char psm);
void generate_lib_rules(bool shell, bool batch, bool batchmode, char psa, char psm);
target* generate_program_rule(const char* filename, char psa);
int generate_link_rules(char psa, char psm);
target* generate_install_rules(void);

extern strarray bins, progs;
extern stralloc output_name;

#endif /* defined(GENMAKEFILE_GENERATE_H) */
