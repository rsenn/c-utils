#ifndef GENMAKEFILE_OUTPUT_H
#define GENMAKEFILE_OUTPUT_H

#include "types.h"
#include "rule.h"
#include "../../lib/buffer.h"
#include "../../lib/strlist.h"
#include "../../lib/map.h"

extern int output_width;

#define OUTPUT_PREREQ_SEPARATOR " \\\n  "

void output_all_rules(buffer*, build_tool_t, const char quote_args[], char psa, char psm, const char* make_sep_inline);
void output_all_vars(buffer*, MAP_T*, strlist* varnames, build_tool_t);
void output_ninja_rule(buffer*, const char*, const stralloc* cmd);
void output_make_rule(buffer*, target*, build_tool_t, const char quote_args[], char psa, char psm, const char* make_sep_inline);
void output_ninja_target(buffer*, target*, char psa);
void output_script(buffer*, target*, build_tool_t, const char quote_args[], char psa, const char* make_sep_inline);
void output_var(buffer*, MAP_T*, const char* name, int serial, build_tool_t);

#endif /* defined(GENMAKEFILE_OUTPUT_H) */
