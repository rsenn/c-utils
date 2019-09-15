#ifndef IHEX_H_
#define IHEX_H_

#include "uint8.h"

typedef struct {
  uint8 reclen;
  uint16 offset;
  uint8 rectyp;
  
} ihex_record;

#endif /* IHEX_H_ */
