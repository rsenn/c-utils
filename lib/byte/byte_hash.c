#include "../uint32.h"

uint32
byte_hash(const void* x, size_t len) {
  const char* key = x;
  unsigned char c;
  size_t i;
  uint32 hash = 5381;

  for(i = 0; i < len; i++) {
    c = key[i];
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}
