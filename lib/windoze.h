#ifndef WINDOZE_H
#define WINDOZE_H 1

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MSYS__)
#if !(defined(__MSYS__) || defined(__CYGWIN__))
#define WINDOWS_NATIVE 1
#endif
#define WINDOWS 1
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define MINGW 1
#endif

#if WINDOWS
#include <windows.h>
#endif

#if WINDOWS_NATIVE
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

