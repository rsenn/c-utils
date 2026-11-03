#ifndef DNSCACHE_ROOTS_H
#define DNSCACHE_ROOTS_H

#include "../../lib/dirlist.h"
#include "../../lib/bool.h"

int roots_find(char*);
int roots_search(char*);
int roots(char[64], char*);
int roots_same(char*, char*);
int roots_init2(struct dirlist*, bool);
int roots_init1(void);
int roots_init(void);

#endif
