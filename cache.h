#ifndef DNSCACHE_CACHE_H
#define DNSCACHE_CACHE_H

#include "lib/uint32.h"
#include "lib/uint64.h"

extern uint64 cache_motion;

char* cache_find(const char*, unsigned int, unsigned int*, struct tai*);
char* cache_get(const char*, unsigned int, unsigned int*, uint32*);
void cache_set(const char*, unsigned int, const char*, unsigned int, uint32);
void cache_update(void);
void cache_free(void);
int cache_init(size_t);
int cache_open(const char*, size_t);

#endif
