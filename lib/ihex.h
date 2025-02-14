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
#include "list.h"
#include "buffer.h"

enum ihex_type_e {
  IHEX_DATA = 0,
  IHEX_EOF = 1,
  IHEX_EXTSEGADDR = 2,
  IHEX_EXTLINADDR = 4
};

typedef union ihex_addr_u {
  uint32 off32;
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

typedef union {
  struct list_head* link;
  struct ihex_record_s* rec;
} ihex_recptr;

struct ihex_record_s;
struct ihex_record_s {
  struct list_head link;
  uint8 length;
  uint16 offset;
  uint8 type;
  uint8 checksum;
  uint8 data[];
};

typedef struct ihex_record_s ihex_record;

typedef struct {
  union {
    struct list_head records;
  };
} ihex_file;

void ihex_init(ihex_file*);
ihex_record* ihex_put(ihex_file*, uint32 offset, const char* x, size_t n);
size_t ihex_read_at(ihex_file*, uint32 at, char* x, size_t n);
ssize_t ihex_read_buf(ihex_file*, const char* in, size_t n);
int ihex_record_address(ihex_record*, ihex_addr* addr);
ihex_record* ihex_record_at(ihex_file*, uint32 at, uint32* roffs);
uint8 ihex_record_checksum(const ihex_record*);
void ihex_record_dump(ihex_record*, uint16 hi16, buffer* b);
ihex_recptr ihex_record_find(ihex_file*, uint32 at, uint32* roffs);
ihex_record* ihex_record_insert(ihex_file*, uint32 at, uint8 len);
ihex_record* ihex_record_next(ihex_file*, ihex_record* rec, int type);
ihex_record* ihex_record_prev(ihex_file*, ihex_record* rec, int type);
ssize_t ihex_record_read(ihex_record**, const char* in, size_t n);
void ihex_record_write(ihex_record*, buffer* b);
int ihex_write(ihex_file*, buffer* b);

static inline const char*
ihex_typestr(enum ihex_type_e type) {
  static const char* const types[] = {
      "DATA", "EOF", "EXTSEGADDR", 0, "EXTLINADDR"};
  return types[type];
}

#endif /* IHEX_H_ */
/** @} */
