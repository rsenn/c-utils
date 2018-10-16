#ifndef	_WORDEXP_H
#define	_WORDEXP_H

#include <stdlib.h>
#include <sys/types.h>

#include "windoze.h"

#if WINDOWS_NATIVE
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define WRDE_DOOFFS  1
#define WRDE_APPEND  2
#define WRDE_NOCMD   4
#define WRDE_REUSE   8
#define WRDE_SHOWERR 16
#define WRDE_UNDEF   32

typedef struct {
	size_t we_wordc;
	char **we_wordv;
	ssize_t we_offs;
} wordexp_t;

#define WRDE_NOSYS   -1
#define WRDE_NOSPACE 1
#define WRDE_BADCHAR 2
#define WRDE_BADVAL  3
#define WRDE_CMDSUB  4
#define WRDE_SYNTAX  5

int wordexp (const char *, wordexp_t *, int);
void wordfree (wordexp_t *);

#ifdef __cplusplus
}
#endif

#endif

