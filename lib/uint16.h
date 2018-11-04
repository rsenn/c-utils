/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef UINT16_H
#define UINT16_H

#if defined(__BORLANDC__)
#include <systypes.h>
#elif !defined(_MSC_VER) && !defined(__MSYS__) && !defined(__CYGWIN__)
#include <inttypes.h>
#include <stdint.h>
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
# include <windows.h>
# define uint16_t UINT16
# define int16_t INT16

typedef uint16_t uint16;
typedef int16_t int16;

#elif defined(__GNUC__) && !defined(__MINGW32__)
typedef __UINT16_TYPE__ uint16;
typedef __INT16_TYPE__ int16;

#elif !defined(__BORLANDC__)
typedef uint16_t uint16;
typedef int16_t int16;
#endif

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT16_MACROS)

inline static void uint16_pack(char* out, uint16 in) {
  *(uint16 *)out = in;
}

inline static void uint16_unpack(const char* in, uint16* out) {
  *out = *(uint16 *)in;
}

inline static uint16 uint16_get(const void* ptr) {
  const char* in = ptr;
  return *(uint16 *)in;
}

inline static uint16 uint16_read(const char* in) {
  return *(uint16 *)in;
}

void uint16_pack_big(char* out, uint16 in);
void uint16_unpack_big(const char* in, uint16* out);
uint16 uint16_read_big(const char* in);
#else

#if !defined(NO_UINT16_MACROS)
inline static uint16
uint16_get(const void* ptr) {
  const unsigned char* in = ptr;
  return ((uint16)in[0] << 8) | (in[1]);
}

inline static uint16
uint16_read(const char* in) {
  return ((uint16)in[0] << 8) | (in[1]);
}
#endif

void uint16_pack(char* out, uint16 in);
void uint16_pack_big(char* out, uint16 in);
void uint16_unpack(const char* in, uint16* out);
void uint16_unpack_big(const char* in, uint16* out);
uint16 uint16_read_big(const char*);

#ifdef NO_UINT16_MACROS
uint16 uint16_read(const char*);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
