/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef UINT64_H
#define UINT64_H

#if !defined(_MSC_VER) && !defined(__MSYS__)
#include <inttypes.h>
#endif
#if !defined(_MSC_VER) && !defined(__MSYS__)
#include <stdint.h>
#endif // !defined(_MSC_VER)

#ifdef __MSYS__
#define __MS_types__
#include <sys/types.h>
#ifdef __BIT_TYPES_DEFINED__
#define uint64_t u_int64_t
#endif
#endif

#ifdef _MSC_VER
#include <windows.h>
#define uint64_t UINT64
#define int64_t INT64
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t uint64;
typedef int64_t int64;

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
#endif // _WIN32 && _MSC_VER

#if defined(_WIN32) && defined(_MSC_VER)
// for older MSVC: "unsigned __int64 -> double" conversion not implemented (why?-)
__inline double uint64_to_double(uint64_t ull) {
  return ((int64_t)ull >= 0 ? (double)(int64_t)ull :
    ((double)(int64_t)(ull - 9223372036854775808UI64)) + 9223372036854775808.0);
}
#else
# define uint64_to_double(ull) ((double)(ull))
#endif // _WIN32 && _MSC_VER && TSCI2_OS_WIN32

#ifdef __cplusplus
}
#endif

#endif
