	/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef UINT32_H
#define UINT32_H

#if defined(__BORLANDC__)
#include <systypes.h>
#elif !defined(_MSC_VER) && !defined(__MSYS__) && !defined(__CYGWIN__)
#include <inttypes.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __BORLANDC__
#ifdef __MSYS__
# ifndef __MS_types__
#  define __MS_types__
# endif
# include <sys/types.h>
# ifdef __BIT_TYPES_DEFINED__
#  define uint32_t u_int32_t
#  define int32_t int
# endif
#endif

#if defined(_MSC_VER)
# include <windows.h>
# define uint32_t UINT32
# define int32_t INT32
#endif

#ifdef __GNUC__
#ifndef uint32_t
typedef __UINT32_TYPE__ uint32_t;
#endif
#ifndef int32_t
typedef __INT32_TYPE__ int32_t;
#endif
#endif

typedef uint32_t uint32;
typedef int32_t int32;
#endif

#if !defined(NO_UINT32_MACROS)

inline static void
uint32_pack(char* out, uint32 in) {
  *(uint32 *)out = in;
}

inline static void
uint32_unpack(const char* in, uint32* out) {
  *out = *(uint32 *)in;
}

inline static uint32
uint32_get(const void* ptr) {
  const char* in = ptr;
  return *(uint32 *)in;
}

inline static uint32
uint32_read(const char* in) {
  return *(uint32 *)in;
}

void uint32_pack_big(char* out, uint32 in);
void uint32_unpack_big(const char* in, uint32* out);
uint32 uint32_read_big(const char* in);
#else

#if !defined(NO_UINT32_MACROS)
inline static uint32
uint32_get(const void* ptr) {
  const char* in = ptr;
  return (in[0] << 24) | (in[1] << 16) | (in[2] << 8) | (in[3]);
}

inline static uint32
uint32_read(const char* in) {
  return (in[0] << 24) | (in[1] << 16) | (in[2] << 8) | (in[3]);
}
#endif

void uint32_pack(char* out, uint32 in);
void uint32_pack_big(char* out, uint32 in);
void uint32_unpack(const char* in, uint32* out);
void uint32_unpack_big(const char* in, uint32* out);
uint32 uint32_read(const char* in);
uint32 uint32_read_big(const char* in);

#endif

#ifdef __cplusplus
}
#endif
#endif
