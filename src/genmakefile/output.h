#ifndef GENMAKEFILE_OUTPUT_H
#define GENMAKEFILE_OUTPUT_H

#include "types.h"
#include "rule.h"

void output_all_rules(buffer*, bool, bool batch, bool shell, const char quote_args[], char psa, char psm, const char* make_sep_inline);
void output_all_vars(buffer*, MAP_T*, strlist* varnames, bool ninja, bool batch, bool shell);
void output_build_rules(buffer*, const char*, const stralloc* cmd);
void output_make_rule(buffer*, target*, bool batch, bool shell, const char quote_args[], char psa, char psm, const char* make_sep_inline);
void output_ninja_rule(buffer*, target*, char psa);
void output_script(buffer*, target*, bool shell, bool batch, const char quote_args[], char psa, const char* make_sep_inline);
void output_var(buffer*, MAP_T*, const char* name, int serial, bool ninja, bool batch, bool shell);

#endif /* defined(GENMAKEFILE_OUTPUT_H) */
