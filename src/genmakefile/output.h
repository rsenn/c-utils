#ifndef GENMAKEFILE_OUTPUT_H
#define GENMAKEFILE_OUTPUT_H

#define MAP_USE_HMAP 1
#include "rule.h"
#include "../../lib/buffer.h"
#include "../../lib/strlist.h"
#include "../../lib/map.h"

extern int output_width;

typedef enum {
  MAKE_RULE_EXPLICIT = 0b00, /**< Standard make implicit rules: .c.o: */
  MAKE_RULE_IMPLICIT = 0b01, /**< Standard make implicit rules: .c.o: */
  MAKE_RULE_PATTERN = 0b10,  /**< GNU make like rules: %.o: %.c */

  MAKE_RULE_MASK = 0b11,

  // MAKE_SUBST_TARGET = 0b100,  /**< Target substitution: $@ */
  // MAKE_SUBST_DEPENDS = 0b1000, /**< Dependencies substitution: $^ */
  // MAKE_SUBST_SOURCE = 0b10000,  /**< Source file substitution: $< */

  // MAKE_SUBST_MASK = 0b11100,
} make_capabilities_t;

typedef struct {

  const char* target;  /**< Target substitution: $@ */
  const char* depends; /**< Dependencies substitution: $^ */
  const char* source;  /**< Source file substitution: $< */

} make_substitutions_t;

#define OUTPUT_PREREQ_SEPARATOR " \\\n  "

void output_all_rules(buffer*, build_tool_t, const char*, char, char, const char*);
void output_all_vars(buffer*, MAP_T*, strlist*, build_tool_t);
void output_ninja_rule(buffer*, const char*, const stralloc*);
void output_make_rule(buffer*, target*, build_tool_t, const char*, char, char, const char*);
void output_ninja_target(buffer*, target*, char);
void output_script(buffer*, target*, build_tool_t, const char*, char, const char*);
void output_var(buffer*, MAP_T*, const char*, int, build_tool_t);

extern make_capabilities_t make_capabs;

#endif /* defined(GENMAKEFILE_OUTPUT_H) */
