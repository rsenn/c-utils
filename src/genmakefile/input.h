#ifndef GENMAKEFILE_INPUT_H
#define GENMAKEFILE_INPUT_H

#include "rule.h"
#include "../../lib/stralloc.h"

int input_process_command(stralloc*, int, char* argv[], const char* file, size_t line);
int input_process_file(const char*, target*);
int input_process_line(const char*, size_t, const char* file, size_t line);
void input_process_path_b(const char*, size_t, stralloc* out);
void input_process_path(const char*, stralloc*);
void input_process_rules(target*);
void builddir_enter(const char* x, size_t len);
void builddir_leave(const char* x, size_t len);

extern set_t common_flags;
extern strlist common_flags_list;

#endif /* defined(GENMAKEFILE_INPUT_H) */
