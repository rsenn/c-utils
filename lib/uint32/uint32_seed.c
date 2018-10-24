#include "../open.h"
#include "../uint32.h"

#define UINT32_POOLSIZE 16

size_t uint32_bytes_seeded;

extern uint32
uint32_pool[UINT32_POOLSIZE];

/* feed data to the prng */
int
uint32_seed(const void* p, unsigned long n) {
  int fd = -1;

  if(n == 0) {
    int i;
    if((fd = open_read("/dev/urandom")) != -1) {
      i = read(fd, uint32_pool, sizeof(uint32_pool));
      if(i > 0) uint32_bytes_seeded += i;
      close(fd);
    }
  } else {
    const char* b = (const char*)p;
    char* x = (char*)uint32_pool;

    while(n) {
      x[n % sizeof(uint32_pool)] ^= *b;
      n--;
      b++;
      uint32_bytes_seeded++;
    }
  }

  return fd;
}
