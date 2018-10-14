#ifndef READLINK_H
#define READLINK_H 1

#include "windoze.h"

#if WINDOWS

_ssize_t readlink(const char*, char* buf, size_t maxlen);
char is_junction(const char*);
int  is_symlink(const char*);

#endif

#endif
