#ifndef DNSCACHE_QUERY_H
#define DNSCACHE_QUERY_H

#include "lib/dns.h"
#include "lib/uint32.h"
#include "lib/iopause.h"

#define QUERY_MAXLEVEL 5
#define QUERY_MAXALIAS 16
#define QUERY_MAXNS 16

struct query {
  unsigned int loop;
  unsigned int level;
  char* name[QUERY_MAXLEVEL];
  char* control[QUERY_MAXLEVEL];
  char* ns[QUERY_MAXLEVEL][QUERY_MAXNS];
  char servers[QUERY_MAXLEVEL][64];
  char servers6[QUERY_MAXLEVEL][256];
  char* alias[QUERY_MAXALIAS];
  uint32 aliasttl[QUERY_MAXALIAS];
  char localip[16];
  char type[2];
  char class[2];
  struct dns_transmit dt;
};

void query_forwardonly(void);
void query_cleanup(struct query*);
int query_aliases(struct query*);
int query_run(struct query*, int);
int query_start(struct query*, char*, char[2], char[2], char[4]);
int query_get(struct query*, iopause_fd*, struct taia*);
void query_io(struct query*, iopause_fd*, struct taia*);

#endif
