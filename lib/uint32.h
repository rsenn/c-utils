/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef UINT32_H
#define UINT32_H

#if !defined(_MSC_VER) && !defined(__MSYS__)
#include <inttypes.h>
#endif
#if !defined(_MSC_VER) && !defined(__MSYS__)
#include <stdint.h>
#endif // !defined(_MSC_VER)

#ifdef __MSYS__
# define __MS_types__
# include <sys/types.h>
# ifdef __BIT_TYPES_DEFINED__
#  define uint32_t u_int32_t
# endif
#endif

#ifdef _MSC_VER
# include <windows.h>
# define uint32_t UINT32
# define int32_t INT32
#endif

#ifdef __GNUC__
#define uint32_t __UINT32_TYPE__
#define int32_t __INT32_TYPE__
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t uint32;
typedef int32_t int32;

#if(defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT32_MACROS)

static inline void uint32_pack(char* out, uint32 in) {
  *(uint32 *)out = in;
}

static inline void uint32_unpack(const char* in, uint32* out) {
  *out = *(uint32 *)in;
}

static inline uint32 uint32_read(const char* in) {
  return *(uint32 *)in;
}

void uint32_pack_big(char* out, uint32 in);
void uint32_unpack_big(const char* in, uint32* out);
uint32 uint32_read_big(const char* in);
#else

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
