#ifndef DNSCACHE_RESPONSE_H
#define DNSCACHE_RESPONSE_H

#include "lib/uint32.h"
#include "lib/stralloc.h"

#define RESPONSE_ANSWER 6
#define RESPONSE_AUTHORITY 8
#define RESPONSE_ADDITIONAL 10

extern int response_hidettl;

int  response_addbytes(stralloc*, const char*, size_t);
int  response_addname(stralloc*, const char*);
int  response_query(stralloc*, const char*, const char[2], const char[2]);
int  response_rstart(stralloc*, const char*, const char[2], uint32);
void response_rfinish(stralloc*, int, uint32);
int  response_cname(stralloc*, const char*, const char*, uint32);
void response_nxdomain(stralloc*);
void response_servfail(stralloc*);
void response_id(stralloc*, const char[2]);
void response_tc(stralloc*, int);


#endif
