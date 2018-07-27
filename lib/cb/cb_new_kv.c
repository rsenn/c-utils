#include <string.h>
#include "../cb.h"

size_t
cb_new_kv(const char* key, size_t keylen, const void* value, size_t len, void* out) {
  char* dst = (char*)out;
  if(dst != key) {
    memmove(dst, key, keylen);
  }
  dst[keylen] = 0;
  memmove(dst + keylen + 1, value, len);
  return len + keylen + 1;
}
