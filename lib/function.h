#ifndef FUNCTION_H
#define FUNCTION_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int(function_t)();
typedef int(function0_t)(void);
typedef int(function1_t)(void*);
typedef int(function2_t)(void*, void*);

struct {
  union {
    function_t* call;
    function0_t* call0;
    function1_t* call1;
    function2_t* call2;
    function3_t* call3;
  };
  size_t arity;
} function;

struct {
  union {
    function1_t* call;
    function1_t* call0;
    function2_t* call1;
    function3_t* call2;
    function4_t* call3;
  };
  void* obj;
  size_t arity;
} mem_fn;

#ifdef __cplusplus
}
#endif

#endif /* defined FUNCTION_H */
