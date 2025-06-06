#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/hashmap.h"
#include "../lib/alloc.h"
#include "../lib/cpp_internal.h"
#include <assert.h>

/* Takes a printf-style cpp_format string and returns a formatted string.*/
static inline char*
cpp_format(char* fmt, ...) {
  char* buf;
  size_t buflen;
  FILE* out = open_memstream(&buf, &buflen);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(out, fmt, ap);
  va_end(ap);
  fclose(out);
  return buf;
}


TEST(hashmap_test) {
  hashmap* map = alloc_zero(sizeof(hashmap));

  for(int i = 0; i < 5000; i++)
    hashmap_put(map, cpp_format("key %d", i), (void*)(size_t)i);
  for(int i = 1000; i < 2000; i++)
    hashmap_delete(map, cpp_format("key %d", i));
  for(int i = 1500; i < 1600; i++)
    hashmap_put(map, cpp_format("key %d", i), (void*)(size_t)i);
  for(int i = 6000; i < 7000; i++)
    hashmap_put(map, cpp_format("key %d", i), (void*)(size_t)i);

  for(int i = 0; i < 1000; i++)
    assert((size_t)hashmap_get(map, cpp_format("key %d", i)) == i);
  for(int i = 1000; i < 1500; i++)
    assert(hashmap_get(map, "no such key") == NULL);
  for(int i = 1500; i < 1600; i++)
    assert((size_t)hashmap_get(map, cpp_format("key %d", i)) == i);
  for(int i = 1600; i < 2000; i++)
    assert(hashmap_get(map, "no such key") == NULL);
  for(int i = 2000; i < 5000; i++)
    assert((size_t)hashmap_get(map, cpp_format("key %d", i)) == i);
  for(int i = 5000; i < 6000; i++)
    assert(hashmap_get(map, "no such key") == NULL);
  for(int i = 6000; i < 7000; i++)
    hashmap_put(map, cpp_format("key %d", i), (void*)(size_t)i);

  assert(hashmap_get(map, "no such key") == NULL);
  buffer_putsflush(buffer_2, "OK\n");
}
