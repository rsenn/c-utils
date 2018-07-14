#include "../cb_internal.h"

void
cb_get_kv_ex(void* kv, void** value) {
  char* key = (char*)kv;
  size_t keylen = strlen(key) + 1;
  *value = key + keylen;
}
