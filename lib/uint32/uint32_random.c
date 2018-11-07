#include "../uint32.h"

#define UINT32_POOLSIZE 16

uint32 uint32_pool[UINT32_POOLSIZE];
unsigned long uint32_bytes_seeded = 0;

uint32
uint32_random(void) {
  int i;
  uint32 r = 0;

  /* seed if not seeded */
  if(uint32_bytes_seeded == 0)
    uint32_seed(0, 0);

  for(i = 0; i < sizeof(uint32_pool) / sizeof(uint32); i++) {
    r += uint32_prng(uint32_pool[i], r);
    uint32_pool[i] = r;
  }

  if(uint32_bytes_seeded)
    uint32_bytes_seeded--;

  return r;
}
