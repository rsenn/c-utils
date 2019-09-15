#ifndef IHEX_H_
#define IHEX_H_

#include "uint8.h"
#include "uint16.h"
#include "slist.h"

struct ihex_record_s;

typedef struct {
  struct ihex_record_s* next;
  uint8 reclen;
  uint16 offset;
  uint8 rectyp;
  uint8 checksum;
  uint8 data[];
} ihex_record;

typedef struct {
  ihex_record* records;
} ihex_file;

ssize_t ihex_read_record(ihex_record*, const char*, size_t n);

#endif /* IHEX_H_ */
