#ifndef BINFMT_H
#define BINFMT_H 1

#include "uint8.h"
#include "uint64.h"

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

typedef struct {
  uint8* baseaddr;
  uint64 filesize;
  binfmt_type type;
  binfmt_bits bits;
} binfmt_file;

int binfmt_open(binfmt_file*, const char* filename);

#ifdef __cplusplus
}
#endif

#endif /* defined(BINFMT_H) */
