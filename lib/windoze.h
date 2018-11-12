#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(WIN32) || defined(MSVC) || defined(__CYGWIN__) || defined(__MSYS__) || defined(__MINGW32__)
# if !(defined(__MSYS__) || defined(__CYGWIN__))
#  define WINDOWS_NATIVE 1
# endif
# define WINDOWS 1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define MINGW 1
#endif

#define _FILE_OFFSET_BITS 64

#if WINDOWS_NATIVE
#if USE_WS2_32
#include <winsock2.h>
#endif
#include <windows.h>
#include <io.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if WINDOWS && !defined(PATH_MAX)
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif /* defined(WINDOZE_H) */
