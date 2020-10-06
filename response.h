#ifndef DNSCACHE_RESPONSE_H
#define DNSCACHE_RESPONSE_H

#include "lib/uint32.h"

extern char response[];
extern unsigned int response_len;

#define RESPONSE_ANSWER 6
#define RESPONSE_AUTHORITY 8
#define RESPONSE_ADDITIONAL 10

int response_addbytes(const char*, unsigned int);
int response_addname(const char*);
int response_query(const char*, const char[2], const char[2]);
void response_hidettl(void);
int response_rstart(const char*, const char[2], uint32);
void response_rfinish(int);
int response_cname(const char*, const char*, uint32);
void response_nxdomain(void);
void response_servfail(void);
void response_id(const char[2]);
void response_tc(void);

#endif
