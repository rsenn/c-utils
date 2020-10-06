#ifndef DNSCACHE_LOG_H
#define DNSCACHE_LOG_H

#include "lib/uint64.h"

void log_number(uint64);
void log_hex(unsigned char);
void log_string(const char*);
void log_line(void);
void log_space(void);
void log_ip(const char[16]);
void log_logid(const char[2]);
void log_logtype(const char[2]);
void log_name(const char*);
void log_startup(void);
void log_query(uint64*, const char[], unsigned int, const char[2], const char*, const char[2]);
void log_querydone(uint64*, unsigned int);
void log_querydrop(uint64*);
void log_tcpopen(const char[], unsigned int);
void log_tcpclose(const char[], unsigned int);
void log_tx(const char*, const char[2], const char*, const char[64], unsigned int);
void log_cachedanswer(const char*, const char[2]);
void log_cachedcname(const char*, const char*);
void log_cachedns(const char*, const char*);
void log_cachednxdomain(const char*);
void log_nxdomain(const char[], const char*, unsigned int);
void log_nodata(const char[], const char*, const char[2], unsigned int);
void log_lame(const char[], const char*, const char*);
void log_servfail(const char*);
void log_rr(const char[], const char*, const char[2], const char*, unsigned int, unsigned int);
void log_rrns(const char[], const char*, const char*, unsigned int);
void log_rrcname(const char[], const char*, const char*, unsigned int);
void log_rrptr(const char[], const char*, const char*, unsigned int);
void log_rrmx(const char[], const char*, const char*, const char[2], unsigned int);
void log_rrsoa(const char[], const char*, const char*, const char*, const char[20], unsigned int);
void log_stats(void);

#endif
