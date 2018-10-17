#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/* for size_t & ptrdiff_t */
#include <stddef.h>

/* for ssize_t: */
#include <sys/types.h>
/*
#if defined(HAVE_INTTYPES_H) || defined(__LCC__) || (!defined(_MSC_VER) && !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(__BORLANDC__))
#include <inttypes.h>
#endif

#if defined(HAVE_STDINT_H) || defined(__LCC__) || (CYGWIN_VERSION_API_MINOR > 100) || (!defined(_MSC_VER) && !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(__BORLANDC__))
#include <stdint.h>
#endif*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED 1
typedef ptrdiff_t ssize_t;
#endif

#ifdef __BORLANDC__
typedef ptrdiff_t intptr_t;
#endif

#if defined(_WIN32) || defined(_WIN64) && !(defined(__CYGWIN__) || defined(__MSYS__))
typedef intptr_t fd_t;
#else
typedef int fd_t,
#endif

#ifdef __cplusplus
}
#endif
#endif /* TYPEDEFS_H */