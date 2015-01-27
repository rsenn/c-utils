/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef UINT64_H
#define UINT64_H

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
typedef unsigned __int64 uint64;
typedef signed __int64 int64;
#else
typedef unsigned long long uint64;
typedef signed long long int64;
#endif

#if (defined(__i386__) || defined(__x86_64__)) && !defined(NO_UINT64_MACROS)
#define uint64_pack(out,in) (*(uint64*)(out)=(in))
#define uint64_unpack(in,out) (*(out)=*(uint64*)(in))
#define uint64_read(in) (*(uint64*)(in))
void uint64_pack_big(char *out,uint64 in);
void uint64_unpack_big(const char *in,uint64* out);
uint64 uint64_read_big(const char *in);
#else

void uint64_pack(char *out,uint64 in);
void uint64_pack_big(char *out,uint64 in);
void uint64_unpack(const char *in,uint64* out);
void uint64_unpack_big(const char *in,uint64* out);
uint64 uint64_read(const char *in);
uint64 uint64_read_big(const char *in);

#endif

#ifdef __cplusplus
}
#endif

#endif
