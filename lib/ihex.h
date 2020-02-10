#ifndef IHEX_H_
#define IHEX_H_

#include "uint8.h"
#include "uint16.h"
#include "uint32.h"
#include "slist.h"

struct ihex_record_s;
struct ihex_record_s {
  struct ihex_record_s* next;
  uint8 length;
  uint16 offset;
  uint8 type;
  uint8 checksum;
  uint8 data[];
};

typedef struct ihex_record_s ihex_record;

typedef struct {
  ihex_record* records;
} ihex_file;

ssize_t ihex_load_buf(ihex_file* ihf, const char* in, size_t n);
ssize_t ihex_load_buf(ihex_file* ihf, const char* in, size_t n);
ssize_t ihex_load_record(ihex_record** pihr, const char* in, size_t n);
size_t ihex_read_at(ihex_file* ihf, uint32 at, char* x, size_t n);
ssize_t ihex_read_record(ihex_record** pihr, const char* in, size_t n);
ihex_record* ihex_record_at(ihex_file* ihf, uint32 at, uint32* roffs);

#endif /* IHEX_H_ */
