#ifndef GENMAKEFILE_INPUT_H
#define GENMAKEFILE_INPUT_H

#include "types.h"
#include "rule.h"

int  input_process_command(stralloc*, int, char* argv[], const char* file, size_t line);
void input_process(const char*, target*, char pathsep_args);
int  input_process_line(const char*, size_t, const char* file, size_t line);
void input_process_path_b(const char*, size_t, stralloc* out);
void input_process_path(const char*, stralloc*);
void input_process_rules(target*, char);

 
#endif /* defined(GENMAKEFILE_GEN_H) */
