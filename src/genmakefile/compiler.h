#ifndef GENMAKEFILE_COMPILER_H
#define GENMAKEFILE_COMPILER_H

#include <stdbool.h>
#include "rule.h"

/* how the generated makefile is written for the selected make program */
extern const char *make_begin_inline, *make_sep_inline, *make_end_inline, *comment, *cross_compile, *quote_args;
extern int ilink_by_hand;
extern bool batchmode, cygming;
extern build_tool_t build_tool;

/* set the target machine ("x86_64", "arm", ...), chip and operating system */
int set_machine(const char*);
int set_chip(const char*);
int set_system(const char*);

/* make program type (tools.make): command separators, comments, quoting */
int set_make_type(void);

/* compiler type: tool names, flags and the compile, lib and link commands */
int set_compiler_type(const char* compiler);

/* resolve tools.make and tools.compiler, apply both, set the build type */
int resolve_toolchain(const char* argv0);

/* Macros the compiler defines itself ("NAME=VALUE", NULL terminated), so a
   source can be preprocessed for it. NULL when they are not known. */
const char* const* compiler_predefs(const char* compiler);

#endif /* defined(GENMAKEFILE_COMPILER_H) */
