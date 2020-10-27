#ifndef DNSCACHE_RESPONSE_H
#define DNSCACHE_RESPONSE_H

#include "lib/uint32.h"
#include "lib/stralloc.h"

#define RESPONSE_ANSWER 6
#define RESPONSE_AUTHORITY 8
#define RESPONSE_ADDITIONAL 10

extern int response_hidettl;

typedef struct response {
#if __STDC_VERSION__ >= 201103L && !defined(__TINYC__)
  union {
    stralloc stra;
    struct {
#endif
      char* buf;
      size_t pos;
      size_t len;
#if __STDC_VERSION__ >= 201103L && !defined(__TINYC__)
    };
  };
#endif
  uint32 dpos, tctarget;
} response;

int response_addbytes(response*, const char*, size_t);
int response_addname(response*, const char*);
int response_query(response*, const char*, const char[2], const char[2]);
int response_rstart(response*, const char*, const char[2], uint32);
void response_rfinish(response*, int);
char* response_getdata(response*, size_t*);
int response_send(response*, int, char[], uint16);
int response_cname(response*, const char*, const char*, uint32);
void response_nxdomain(response*);
void response_servfail(response*);
void response_id(response*, const char[2]);
void response_tc(response*);

#endif
