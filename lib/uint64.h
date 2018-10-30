/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef UINT64_H
#define UINT64_H

#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MSYS__
# ifndef __MS_types__
#  define __MS_types__
# endif
# undef __BSD_VISIBLE
# define __BSD_VISIBLE 0
# include <sys/types.h>
# ifdef __BIT_TYPES_DEFINED__
#  define uint64_t u_int64_t
#  define int64_t long long
# else
typedef unsigned long long uint64_t;
typedef long long int64_t;
# endif
#endif

#if defined(___int64_t_defined)
typedef u_int64_t uint64;
typedef int64_t int64;

#elif defined(__UINT64_TYPE__) && defined(__INT64_TYPE__)
typedef __UINT64_TYPE__ uint64;
typedef __INT64_TYPE__ int64;

#elif defined(_MSC_VER) || defined(__BORLANDC__)
#include <windows.h>
typedef UINT64 uint64;
typedef INT64 int64;
#else 

/*
#ifdef __GNUC__
# ifndef uint64_t
#  ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#  elif defined __INT64_TYPE__
typedef unsigned __INT64_TYPE__ uint64_t;
#  endif
# endif
# ifndef int64_t
//#  define int64_t __int64
#  if !defined(_INT64_T_DECLARED) && !defined(__int64_t_defined) && !defined(__int8_t_defined) && !defined(_BITS_STDINT_INTN_H) && (!defined(__dietlibc__) && !defined(_INTTYPES_H))
#   define __int64_t_defined
#   define _INT64_T_DECLARED
typedef long long int64_t;
#  endif
# endif
#endif
*/
typedef uint64_t uint64;
typedef int64_t int64;
#endif

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT64_MACROS)
#define uint64_pack(out, in) ( * (uint64 *)(out) = (in))
#define uint64_unpack(in, out) ( * (out) = *(uint64 *)(in))
#define uint64_read(in) ( * (uint64 *)(in))
void uint64_pack_big(char* out, uint64 in);
void uint64_unpack_big(const char* in, uint64* out);
uint64 uint64_read_big(const char* in);
#else

void uint64_pack(char* out, uint64 in);
void uint64_pack_big(char* out, uint64 in);
void uint64_unpack(const char* in, uint64* out);
void uint64_unpack_big(const char* in, uint64* out);
uint64 uint64_read(const char* in);
uint64 uint64_read_big(const char* in);

#endif

inline static uint64
uint64_get(const void* ptr) {
  const char* in = ptr;
  return *(uint64 *)in;
}

#if defined(_WIN32) && defined(_MSC_VER)
// for older MSVC
# ifndef PRId64
#  define PRId64 "I64d"
# endif
# ifndef PRIu64
#  define PRIu64 "I64u"
# endif
# ifndef PRIx64
#  define PRIx64 "I64x"
# endif
#endif /* _WIN32 && _MSC_VER */

#if defined(_WIN32) && defined(_MSC_VER)
// for older MSVC: "unsigned __int64 -> double" conversion not implemented (why?-)
__inline double uint64_to_double(uint64 ull) {
  return ((int64)ull >= 0 ? (double)(int64)ull :
    ((double)(int64)(ull - 9223372036854775808UI64)) + 9223372036854775808.0);
}
#else
# define uint64_to_double(ull) ((double)(ull))
#endif /* _WIN32 && _MSC_VER && TSCI2_OS_WIN32 */

#ifdef __cplusplus
}
#endif

#endif
