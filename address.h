#ifndef _ADDRESS_H
#define _ADDRESS_H

#include <stdbool.h>
#include "lib/uint32.h"

typedef struct {
  char ip[16];
  bool ip6;
  uint32 scope_id;
} address_t;

int address_init(address_t*, char[16], bool ip6, uint32 scope_id);
size_t address_fmt(char*, const address_t*);
int address_lookup(stralloc*, address_t*, bool ip6);
size_t address_scan(const char*, address_t*);

#endif /* defined _ADDRESS_H */
