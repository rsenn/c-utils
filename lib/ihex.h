/**
 * @defgroup   ihex
 * @brief      IHEX module.
 * @{
 */
#ifndef IHEX_H_
#define IHEX_H_

#include "uint8.h"
#include "uint16.h"
#include "uint32.h"
#include "slist.h"
#include "buffer.h"

enum ihex_type_e { IHEX_DATA = 0, IHEX_EOF = 1, IHEX_EXTSEGADDR = 2, IHEX_EXTLINADDR = 4 };

typedef union ihex_addr_u {
  uint32 ptr32;
  struct {
#if(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    uint16 lo16;
    uint16 hi16;
#else
    uint16 hi16;
    uint16 lo16;
#endif
  };
} ihex_addr;

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

size_t       ihex_read_at(ihex_file*, uint32 at, char* x, size_t n);
ssize_t      ihex_read_buf(ihex_file*, const char* in, size_t n);
ssize_t      ihex_read_record(ihex_record**, const char* in, size_t n);
ihex_record* ihex_record_at(ihex_file*, uint32 at, uint32* roffs);
int          ihex_write(ihex_file*, buffer* b);
void         ihex_write_record(ihex_record*, buffer* b);

#endif /* IHEX_H_ */
/** @} */
