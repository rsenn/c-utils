/* the part of C99 stdint.h that Borland C++ 5.5 lacks */
#ifndef COMPAT_STDINT_H
#define COMPAT_STDINT_H 1

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned int uintptr_t;
typedef int intptr_t;

#define INT32_MAX 2147483647
#define UINT32_MAX 0xffffffffU
#define UINT64_MAX 0xffffffffffffffffui64
#define SIZE_MAX 0xffffffffU

#endif
