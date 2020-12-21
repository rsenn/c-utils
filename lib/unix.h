#ifndef UNIX_H
#define UNIX_H 1

#include "typedefs.h"
#include "windoze.h"
#include "getopt.h"
#include "readlink.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern const short __spm[13];

int fnmatch(const char* pattern, const char* string, int flags);
int isleap(int year);
int pipe2(int fd[2], int flags);

int unix_getopt(int, char* const[], const char*);
int
unix_getopt_long(int, char* const[], const char*, const struct longopt*, int*);

extern char* unix_optarg;
extern int unix_optind;
extern int unix_opterr;
extern int unix_optopt;
#ifdef BUFFER_H
extern buffer* unix_optbuf;
#endif

#if WINDOWS_NATIVE
size_t getpagesize();
#endif

#ifdef __cplusplus
}
#endif
#endif /* defined(UNIX_H) */
