#ifndef DUMP_H
#define DUMP_H

#include "lib/strlist.h"

void dump_bytes(const char*, size_t n);
void dump_char(char);
void dump_ints(const int*, size_t n);
void dump_int(long);
void dump_key(const char*);
void dump_long_n(long, int n);
void dump_newline(void);
void dump_quote(void);
void dump_sep(void);
void dump_strlist(const strlist*);
void dump_str(const char*);
void dump_ulong(long);

#endif