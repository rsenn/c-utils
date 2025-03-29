#include "../hmap.h"
#include "../buffer.h"
#include "../str.h"
#include <assert.h>

int
hmap_set_chars(HMAP_DB** hmap_db, const char* key, const char* data) {
  /*assert(key);
  assert(str_len(key));
  assert(data);
  assert(str_len(data));*/

#ifdef HMAP_DEBUG
  buffer_putm_internal(buffer_2, __FILE__, ": ", "hmap_set_chars(", NULL);
  buffer_putptr(buffer_2, hmap_db);
  buffer_putm(buffer_2, ", \"", key, "\", \"", data, "\")");
  buffer_putnlflush(buffer_2);
#endif

  return hmap_add(hmap_db, (void*)key, str_len(key), 1, HMAP_DATA_TYPE_CHARS, (char*)data, (size_t)str_len(data));
}
