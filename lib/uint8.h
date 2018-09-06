#ifndef UINT8_H
#define UINT8_H

#if !defined(_MSC_VER) && !defined(__MSYS__)
#include <inttypes.h>
#endif /* !defined(_MSC_VER) */
#if !defined(_MSC_VER) && !defined(__MSYS__)
#include <stdint.h>
#endif

#ifdef __MSYS__
# ifndef __MS_types__
#  define __MS_types__
# endif
# include <sys/types.h>
# ifdef __BIT_TYPES_DEFINED__
#  define uint8_t u_int8_t
#  define int8_t short
# endif
#endif

#ifdef _MSC_VER
#include <windows.h>
#define uint8_t UINT8
#define int8_t INT8
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t uint8;
typedef int8_t int8;

#ifdef __cplusplus
}
#endif

#endif
