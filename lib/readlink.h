#ifndef READLINK_H
#define READLINK_H 1

#include "typedefs.h"
#include "windoze.h"

#if WINDOWS
ssize_t readlink(const char*, char* buf, size_t maxlen);
#endif

#if WINDOWS
char is_junction(const char*);
int is_symlink(const char*);
#endif

#endif
