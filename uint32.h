/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef UINT32_H
#define UINT32_H

#include <sys/types.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /* defined HAVE_STDINT_H */

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif /* defined HAVE_INTTYPES_H */

#ifdef _MSC_VER
#include <windows.h>
#define uint32_t UINT32
#define int32_t INT32
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t uint32;
typedef int32_t int32;

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT32_MACROS)

static inline void uint32_pack(char* out,uint32 in) {
  *(uint32*)out=in;
}

static inline void uint32_unpack(const char *in,uint32* out) {
  *out=*(uint32*)in;
}

static inline uint32 uint32_read(const char* in) {
  return *(uint32*)in;
}

void uint32_pack_big(char *out,uint32 in);
void uint32_unpack_big(const char *in,uint32* out);
uint32 uint32_read_big(const char *in);
#else

void uint32_pack(char *out,uint32 in);
void uint32_pack_big(char *out,uint32 in);
void uint32_unpack(const char *in,uint32* out);
void uint32_unpack_big(const char *in,uint32* out);
uint32 uint32_read(const char *in);
uint32 uint32_read_big(const char *in);

#endif

#ifdef __cplusplus
}
#endif

#endif
