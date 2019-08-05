#include "../cb_internal.h"
#include "../byte.h"

size_t
cb_new_kv(const char* key, size_t keylen, const void* value, size_t len, void* out) {
  char* dst = (char*)out;
  if(dst != key) {
    byte_copyr(dst, keylen, key);
  }
  dst[keylen] = 0;
  byte_copyr(dst + keylen + 1, len, value);
  return len + keylen + 1;
}

