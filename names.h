#ifndef DNSCACHE_NAMES_H
#define DNSCACHE_NAMES_H

#include "lib/array.h"

void names_get(const char*, size_t, array*);
array names_array(const char*, size_t);
char* names_vector(const char*, size_t);
void names_print(const array*);
int names_compare(const void*, const void*);
void names_shuffle(char*, size_t);

#endif
