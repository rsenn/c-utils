#ifndef READLINK_H
#define READLINK_H 1

#include "windoze.h"

#if WINDOWS_NATIVE
ssize_t readlink(const char*, char* buf, size_t maxlen);
#endif

#if WINDOWS
char is_junction(const char*);
int  is_symlink(const char*);
#endif

#endif
