#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/* for ssize_t: */
#define __POCC__OLDNAMES
//#include <sys/types.h>

/* for size_t & ptrdiff_t */
#include <stddef.h>

/* for time_t */
//#include <time.h>

#ifdef __LCC__
#include <stdint.h>
#endif

//#include <time.h>

/*
#if defined(HAVE_INTTYPES_H) || defined(__LCC__) || (!defined(_MSC_VER) &&
!defined(__MSYS__) && !defined(__CYGWIN__) && !defined(__BORLANDC__))
#include <inttypes.h> #endif

#if defined(HAVE_STDINT_H) || defined(__LCC__) || (CYGWIN_VERSION_API_MINOR
> 100) || (!defined(_MSC_VER) && !defined(__MSYS__) && !defined(__CYGWIN__)
&& !defined(__BORLANDC__)) #include <stdint.h> #endif
*/

/*#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__ORANGEC__) || defined(__DMC__) || defined(__STDC_IEC_559__)
#include <stdint.h>
#endif*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SSIZE_T_DEF
#define _SSIZE_T_DEFINED 1
#endif
#ifdef _SSIZE_T_DEFINED_
#define _SSIZE_T_DEFINED 1
#endif

#if !defined(__LCC__)
#if defined(__BORLANDC__)
/*#if __BORLANDC__ >= 0x570
typedef int ssize_t;
#else
typedef long ssize_t;
#endif*/

#elif defined(_MSC_VER)
#include <intsafe.h>
typedef SSIZE_T ssize_t;

#elif defined(__SSIZE_TYPE__) && !defined(_SSIZE_T_DEFINED) && !defined(_SSIZE_T_)
#define _SSIZE_T_DEFINED 1
#define _SSIZE_T_ 1
typedef __SSIZE_TYPE__ ssize_t;

#elif !defined(MSYS) && defined(__PTRDIFF_TYPE__) && !defined(_SSIZE_T_DEFINED) && !defined(_SSIZE_T_)
#define _SSIZE_T_DEFINED 1
#define _SSIZE_T_ 1
typedef __PTRDIFF_TYPE__ ssize_t;

#elif !(defined(_SYS_TYPES_H) && defined(__MSYS__)) && !defined(_SSIZE_T_DEFINED) && !defined(_SSIZE_T_)
#undef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
#define _SSIZE_T_ 1
typedef __PTRDIFF_TYPE__ ssize_t;

#elif !defined(_SSIZE_T_DEFINED) && !defined(_SSIZE_T_)
#define _SSIZE_T_DEFINED 1
#define _SSIZE_T_ 1
typedef long ssize_t;
#endif
#endif

#if !defined(__dietlibc__) && !defined(_INTTYPES_H) && !defined(__clang__) || defined(__BCPLUSPLUS__)
#ifndef __LCC__
#ifdef __INTPTR_TYPE__
typedef __INTPTR_TYPE__ intptr_t;
#elif defined(__BORLANDC__) || defined(__POCC__) || (defined(_SYS_TYPES_H) && defined(__MSYS__))
typedef ptrdiff_t intptr_t;
#endif
#endif
#endif

#if defined(_WIN32) || defined(__MINGW32__) || defined(_WIN64) && !(defined(__CYGWIN__) || defined(__MSYS__))
typedef intptr_t fd_type;
#else
typedef int fd_type;
#endif

#ifndef __unaligned
#ifdef __GNUC__
#define __unaligned __attribute__((packed))
#else
#define __unaligned
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif /* TYPEDEFS_H */
