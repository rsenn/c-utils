#ifndef IHEX_H_
#define IHEX_H_

#include "uint8.h"
#include "uint16.h"
#include "slist.h"

struct ihex_record_s;
struct ihex_record_s {
  struct ihex_record_s* next;
  uint8 reclen;
  uint16 offset;
  uint8 rectyp;
  uint8 checksum;
  uint8 data[];
};

typedef struct ihex_record_s ihex_record;

typedef struct {
  ihex_record* records;
} ihex_file;

ssize_t ihex_read_record(ihex_record*, const char*, size_t n);
ssize_t ihex_read_data(ihex_record**, const char*, size_t n);
ssize_t ihex_read_buf(ihex_file*, const char*, size_t n);

#endif /* IHEX_H_ */
