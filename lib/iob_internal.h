#ifndef IOB_INTERNAL_H
#define IOB_INTERNAL_H

#include "iob.h"
#include "array.h"

typedef enum { FROMBUF, FROMFILE } iob_entry_type;

typedef struct iob_entry {
  iob_entry_type type;
  fd_type fd;
  const char* buf;
  uint64 n;
  uint64 offset;
  void (*cleanup)(struct iob_entry* x);
#ifdef __MINGW32__
  OVERLAPPED os;
#endif
} iob_entry;

int iob_addbuf_internal(io_batch* b, const void* buf, uint64 n, void (*cleanup)(struct iob_entry* x));

#endif
