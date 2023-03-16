#ifndef GENMAKEFILE_GEN_H
#define GENMAKEFILE_GEN_H

#include <stdbool.h>
#include "types.h"
#include "rule.h"
#include "sourcedir.h"

void gen_clean_rule(char);
target* gen_install_rules(void);
void gen_lib_rules(bool, bool, bool batchmode, char pathsep_args, char pathsep_make);
int gen_link_rules(const char*, char, char pathsep_make);
target* gen_mkdir_rule(stralloc*);
target* gen_program_rule(const char*, const char*, char pathsep_args);
target* gen_simple_compile_rules(
    sourcedir*, const char*, const char* fromext, const char* toext, stralloc* cmd, char pathsep_args);
target* gen_single_rule(stralloc*, stralloc*);
target* gen_srcdir_compile_rules(
    sourcedir*, const char*, bool shell, bool batch, bool batchmode, char pathsep_args, char pathsep_make);
target* gen_srcdir_lib_rule(
    sourcedir*, const char*, bool shell, bool batch, bool batchmode, char pathsep_args, char pathsep_make);
void gen_srcdir_rule(sourcedir*, const char*, bool batchmode, char pathsep_make);

extern strarray bins, progs;
extern stralloc output_name;

#endif /* defined(GENMAKEFILE_GEN_H) */
