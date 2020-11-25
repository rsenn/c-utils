#include "../hmap.h"
#include "../buffer.h"
#include "../stralloc.h"
#include <assert.h>

int
hmap_set_stralloc(HMAP_DB** hmap_db, const stralloc* key, const stralloc* data) {
  assert(key->s);
  assert(key->len);

  if(!data->s || !data->len) {
    buffer_putm_internal(buffer_2, "hmap_set_stralloc", " ", __FILE__, ":", NULL);
    buffer_putlong(buffer_2, __LINE__);
    buffer_puts(buffer_2, ": No data for key '");
    buffer_put(buffer_2, key->s, key->len);
    buffer_puts(buffer_2, "'");
    buffer_putnlflush(buffer_2);
    assert(data->s);
    assert(data->len);
  }

#ifdef HMAP_DEBUG
  buffer_putm_internal(buffer_2, "hmap_set_stralloc", "(", NULL);
  buffer_putptr(buffer_2, hmap_db);
  buffer_puts(buffer_2, ", \"");
  buffer_putsa(buffer_2, key);
  buffer_puts(buffer_2, "\", \"");
  buffer_putsa(buffer_2, data);
  buffer_puts(buffer_2, "\")");
  buffer_putnlflush(buffer_2);
#endif

  return hmap_add(
      hmap_db, (void*)key->s, key->len, 1, HMAP_DATA_TYPE_CHARS, (char*)data->s, (size_t)data->len);
}
