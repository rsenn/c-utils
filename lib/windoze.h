#ifndef WINDOZE_H
#define WINDOZE_H 1

#ifndef _WIN32
#if defined(WIN32) || defined(__MINGW32__)
#define _WIN32 1
#endif
#endif

#ifndef _WIN64
#if defined(WIN64) || defined(__MINGW64__)
#define _WIN64 1
#endif
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__MINGW64__) || defined(MSVC) || \
    defined(__CYGWIN__) || defined(__MSYS__)
#if !(defined(__MSYS__) || defined(__CYGWIN__))
#define WINDOWS_NATIVE 1
#endif
#ifndef WINDOWS
#define WINDOWS 1
#endif
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define MINGW 1
#endif

#define _FILE_OFFSET_BITS 64

#ifdef __cplusplus
extern "C" {
#endif

#if !WINDOWS_NATIVE
#define WINDOWS_NATIVE 0
#endif

#ifdef __cplusplus
}
#endif
#endif /* defined(WINDOZE_H) */
