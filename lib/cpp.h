/**
 * @defgroup   cpp
 * @brief      CPP module.
 * @{
 */
#ifndef CPP_H
#define CPP_H

#include "buffer.h"

typedef struct cpp_s cpp_t;

cpp_t* cpp_new(void);
void cpp_free(cpp_t*);
void cpp_add_includedir(cpp_t* cpp, const char* includedir);
int cpp_add_define(cpp_t* cpp, const char* mdecl);
int cpp_run(cpp_t* cpp, buffer* in, buffer* out, const char* inname);

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#pragma RcB2 DEP "cpp.c"

#endif
/** @} */
