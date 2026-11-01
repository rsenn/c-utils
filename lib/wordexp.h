#ifndef CUTILS_WORDEXP_H
#define CUTILS_WORDEXP_H

/* The system's header where there is one: the replacement below has a different
 * wordexp_t and different flag values, mixing the two reads the wrong fields. */
#if !defined(_WIN32) && !defined(__MSYS__) && !defined(__CYGWIN__) && defined(__has_include_next)
#if __has_include_next(<wordexp.h>)
#include_next <wordexp.h>
#define WORDEXP_SYSTEM 1
#endif
#endif
#ifndef WORDEXP_SYSTEM

#ifdef __cplusplus
extern "C" {
#endif

// #include <features.h>

#define __NEED_size_t

#include <sys/types.h>

#define WRDE_DOOFFS 1
#define WRDE_APPEND 2
#define WRDE_NOCMD 4
#define WRDE_REUSE 8
#define WRDE_SHOWERR 16
#define WRDE_UNDEF 32

typedef struct {
  size_t we_wordc;
  char** we_wordv;
  size_t we_offs;
} wordexp_t;

#define WRDE_NOSYS -1
#define WRDE_NOSPACE 1
#define WRDE_BADCHAR 2
#define WRDE_BADVAL 3
#define WRDE_CMDSUB 4
#define WRDE_SYNTAX 5

int wordexp(const char*, wordexp_t*, int);
void wordfree(wordexp_t*);

#ifdef __cplusplus
}
#endif

#endif /* WORDEXP_SYSTEM */

#endif
