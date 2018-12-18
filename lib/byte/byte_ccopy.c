#include "../byte.h"

size_t
byte_ccopy(void* dst, size_t count, const void* src, char c) {
  char* a = dst;
  const char* b = src;
  while(count--) {
    *a++ = *b;
    if(*b == c) { return b + 1 - (const char*)src; }
    ++b;
  }
  return b - (const char*)src;
}
