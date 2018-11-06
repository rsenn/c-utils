#ifndef BINFMT_H
#define BINFMT_H 1

#include "typedefs.h"
#include "uint8.h"
#include "uint32.h"
#include "uint64.h"
#include "range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PE,
	ELF,
	MACH_O
} binfmt_type;

typedef enum {
	B32,
	B64
} binfmt_bits;

typedef enum {
	B_R = 4,
	B_W = 2,
	B_X = 1
} binfmt_prot;

typedef struct {
	const char* name;
	uint64 addr;
	uint64 offs;
	uint64 size;
	uint32 flags;
} binfmt_entry;

typedef struct {
  uint8* baseaddr;
  size_t filesize;
  binfmt_type type;
  binfmt_bits bits;
  range sections;
  range imports;
  range exports;
} binfmt_file;

int binfmt_open(binfmt_file*, const char* filename);

#ifdef __cplusplus
}
#endif
#endif

