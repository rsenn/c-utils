#ifndef GENMAKEFILE_OUTPUT_H
#define GENMAKEFILE_OUTPUT_H

#define MAP_USE_HMAP 1
#include "rule.h"
#include "../../lib/buffer.h"
#include "../../lib/strlist.h"
#include "../../lib/map.h"

extern int output_width;

typedef enum {
  MAKE_IMPLICIT_RULES = (1 << 0), /**< Standard make implicit rules: .c.o: */
  MAKE_PATTERN_RULES = (1 << 1),  /**< GNU make like rules: %.o: %.c */
} make_capabilities_t;

#define OUTPUT_PREREQ_SEPARATOR " \\\n  "

void output_all_rules(buffer*, build_tool_t, const char*, char, char, const char*);
void output_all_vars(buffer*, MAP_T*, strlist*, build_tool_t);
void output_ninja_rule(buffer*, const char*, const stralloc*);
void output_make_rule(buffer*, target*, build_tool_t, const char*, char, char, const char*);
void output_ninja_target(buffer*, target*, char);
void output_script(buffer*, target*, build_tool_t, const char*, char, const char*);
void output_var(buffer*, MAP_T*, const char*, int, build_tool_t);

#endif /* defined(GENMAKEFILE_OUTPUT_H) */
